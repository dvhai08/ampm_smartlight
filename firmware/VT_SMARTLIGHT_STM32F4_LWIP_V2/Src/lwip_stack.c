/* LWIP implementation of NetworkInterfaceAPI
 * Copyright (c) 2015 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "lwip_stack.h"
#include "lwip/opt.h"
#include "lwip/api.h"
#include "lwip/inet.h"
#include "lwip/netif.h"
#include "lwip/dhcp.h"
#include "lwip/tcpip.h"
#include "lwip/tcp.h"
#include "lwip/ip.h"
#include "lwip/dns.h"
#include "lwip/udp.h"
#include "nsapi_types.h"
#include "lwip/sys.h" 

static nsapi_error_t mbed_lwip_err_remap(err_t err);


/* Static arena of sockets */
static struct lwip_socket {
    bool in_use;

    struct netconn *conn;
    struct netbuf *buf;
    u16_t offset;

    void (*cb)(void *);
    void *data;
} lwip_arena[MEMP_NUM_NETCONN];


static bool lwip_connected = false;

static struct lwip_socket *mbed_lwip_arena_alloc(void)
{
    sys_prot_t prot = sys_arch_protect();

    for (int i = 0; i < MEMP_NUM_NETCONN; i++) {
        if (!lwip_arena[i].in_use) {
            struct lwip_socket *s = &lwip_arena[i];
            memset(s, 0, sizeof *s);
            s->in_use = true;
            sys_arch_unprotect(prot);
            return s;
        }
    }

    sys_arch_unprotect(prot);
    return 0;
}

static void mbed_lwip_arena_dealloc(struct lwip_socket *s)
{
    s->in_use = false;
}

static void mbed_lwip_socket_callback(struct netconn *nc, enum netconn_evt eh, u16_t len)
{
    // Filter send minus events
    if (eh == NETCONN_EVT_SENDMINUS && nc->state == NETCONN_WRITE) {
        return;
    }

    sys_prot_t prot = sys_arch_protect();

    for (int i = 0; i < MEMP_NUM_NETCONN; i++) {
        if (lwip_arena[i].in_use
            && lwip_arena[i].conn == nc
            && lwip_arena[i].cb) {
            lwip_arena[i].cb(lwip_arena[i].data);
        }
    }

    sys_arch_unprotect(prot);
}


/* TCP/IP and Network Interface Initialisation */
static struct netif lwip_netif;

static char lwip_mac_address[NSAPI_MAC_SIZE];


static bool convert_mbed_addr_to_lwip(ip_addr_t *out, const nsapi_addr_t *in)
{
		
    if (in->version == NSAPI_IPv4) {
			IP4_ADDR(out,in->bytes[0],in->bytes[1],in->bytes[2],in->bytes[3]);
       return true;
    }
    return false;
}

static bool convert_lwip_addr_to_mbed(nsapi_addr_t *out, const ip_addr_t *in)
{
		out->version = NSAPI_IPv4;
		MEMCPY(out->bytes, &in->addr, sizeof(ip_addr_t));
		return true;
}

static const ip_addr_t *mbed_lwip_get_ipv4_addr(const struct netif *netif)
{
    if (!netif_is_up(netif)) {
        return NULL;
    }
    if (!ip_addr_isany(&((netif)->ip_addr))) {
        return &((netif)->ip_addr);
    }
    return NULL;
}


const ip_addr_t *mbed_lwip_get_ip_addr(bool any_addr, const struct netif *netif)
{
    const ip_addr_t *pref_ip_addr = 0;
    const ip_addr_t *npref_ip_addr = 0;
	
    pref_ip_addr = mbed_lwip_get_ipv4_addr(netif);

    if (pref_ip_addr) {
        return pref_ip_addr;
    } else if (npref_ip_addr && any_addr) {
        return npref_ip_addr;
    }
    return NULL;
}

static void add_dns_addr_to_dns_list_index(const u8_t addr_type, const u8_t index)
{
        ip_addr_t  ipv4_dns_addr;
				IP4_ADDR(&ipv4_dns_addr,8,8,8,8);
        dns_setserver(index, &ipv4_dns_addr);
}

void add_dns_addr(struct netif *lwip_netif)
{
    // Check for existing dns address
    for (char numdns = 0; numdns < DNS_MAX_SERVERS; numdns++) {
        ip_addr_t dns_ip_addr = dns_getserver(numdns);
        if (!ip_addr_isany(&dns_ip_addr)) {
            return;
        }
    }

    // Get preferred ip version
    const ip_addr_t *ip_addr = mbed_lwip_get_ip_addr(false, lwip_netif);
    u8_t addr_type = 0;
    add_dns_addr_to_dns_list_index(addr_type, 0);
}

static sys_sem_t lwip_tcpip_inited;
static void mbed_lwip_tcpip_init_irq(void *eh)
{
    sys_sem_signal(&lwip_tcpip_inited);
}

static sys_sem_t lwip_netif_linked;
static sys_sem_t lwip_netif_unlinked;
static void mbed_lwip_netif_link_irq(struct netif *lwip_netif)
{
    if (netif_is_link_up(lwip_netif)) {
        sys_sem_signal(&lwip_netif_linked);
    } else {
        sys_sem_signal(&lwip_netif_unlinked);
    }
}

static char lwip_has_addr_state = 0;

#define HAS_ANY_ADDR  1
static sys_sem_t lwip_netif_has_any_addr;
#if PREF_ADDR_TIMEOUT
#define HAS_PREF_ADDR 2
static sys_sem_t lwip_netif_has_pref_addr;
#endif
#if BOTH_ADDR_TIMEOUT
#define HAS_BOTH_ADDR 4
static sys_sem_t lwip_netif_has_both_addr;
#endif

static void mbed_lwip_netif_status_irq(struct netif *lwip_netif)
{
    if (netif_is_up(lwip_netif)) {
        if (!(lwip_has_addr_state & HAS_ANY_ADDR) && mbed_lwip_get_ip_addr(true, lwip_netif)) {
            sys_sem_signal(&lwip_netif_has_any_addr);
            lwip_has_addr_state |= HAS_ANY_ADDR;
        }
#if PREF_ADDR_TIMEOUT
        if (!(lwip_has_addr_state & HAS_PREF_ADDR) && mbed_lwip_get_ip_addr(false, lwip_netif)) {
            sys_sem_signal(&lwip_netif_has_pref_addr);
            lwip_has_addr_state |= HAS_PREF_ADDR;
        }
#endif
#if BOTH_ADDR_TIMEOUT
        if (!(lwip_has_addr_state & HAS_BOTH_ADDR) && mbed_lwip_get_ipv4_addr(lwip_netif) && mbed_lwip_get_ipv6_addr(lwip_netif)) {
            sys_sem_signal(&lwip_netif_has_both_addr);
            lwip_has_addr_state |= HAS_BOTH_ADDR;
        }
#endif
    }
}


char *mbed_lwip_get_ip_address(char *buf, nsapi_size_t buflen)
{
    const ip_addr_t *addr = mbed_lwip_get_ip_addr(true, &lwip_netif);
    return ipaddr_ntoa_r(addr, buf, buflen);
}



// Backwards compatibility with people using DEVICE_EMAC
void mbed_lwip_init(void)
{
   lwip_connected = true;
}

/* LWIP error remapping */
static nsapi_error_t mbed_lwip_err_remap(err_t err) {
    switch (err) {
        case ERR_OK:
        case ERR_CLSD:
            return 0;
        case ERR_MEM:
            return NSAPI_ERROR_NO_MEMORY;
        case ERR_CONN:
        case ERR_RST:
        case ERR_ABRT:
            return NSAPI_ERROR_NO_CONNECTION;
        case ERR_TIMEOUT:
        case ERR_RTE:
        case ERR_WOULDBLOCK:
            return NSAPI_ERROR_WOULD_BLOCK;
        case ERR_VAL:
        case ERR_USE:
        case ERR_ARG:
            return NSAPI_ERROR_PARAMETER;
        case ERR_INPROGRESS:
            return NSAPI_ERROR_IN_PROGRESS;
        case ERR_ISCONN:
            return NSAPI_ERROR_IS_CONNECTED;
        default:
            return NSAPI_ERROR_DEVICE_ERROR;
    }
}

/* LWIP network stack implementation */
static nsapi_error_t mbed_lwip_gethostbyname(nsapi_stack_t *stack, const char *host, nsapi_addr_t *addr, nsapi_version_t version)
{
    ip_addr_t lwip_addr;

    if (version != NSAPI_IPv4 && version != NSAPI_UNSPEC) {
        return NSAPI_ERROR_DNS_FAILURE;
    }
		
    err_t err = netconn_gethostbyname(host, &lwip_addr);

    if (err != ERR_OK) {
        return NSAPI_ERROR_DNS_FAILURE;
    }
    convert_lwip_addr_to_mbed(addr, &lwip_addr);
    return 0;
}

static nsapi_error_t mbed_lwip_add_dns_server(nsapi_stack_t *stack, nsapi_addr_t addr)
{
    // Shift all dns servers down to give precedence to new server
		ip_addr_t addr_1;
    for (int i = DNS_MAX_SERVERS-1; i > 0; i--) {
				addr_1 = dns_getserver(i-1);
        dns_setserver(i, &addr_1);
    }

    ip_addr_t ip_addr;
    if (!convert_mbed_addr_to_lwip(&ip_addr, &addr)) {
        return NSAPI_ERROR_PARAMETER;
    }

    dns_setserver(0, &ip_addr);
    return 0;
}

static nsapi_error_t mbed_lwip_socket_open(nsapi_stack_t *stack, nsapi_socket_t *handle, nsapi_protocol_t proto)
{
    // check if network is connected
    if (!lwip_connected) {
        return NSAPI_ERROR_NO_CONNECTION;
    }

    // allocate a socket
    struct lwip_socket *s = mbed_lwip_arena_alloc();
    if (!s) {
        return NSAPI_ERROR_NO_SOCKET;
    }

    enum netconn_type lwip_proto = proto == NSAPI_TCP ? NETCONN_TCP : NETCONN_UDP;

#if LWIP_IPV6
    // Enable IPv6 (or dual-stack)
    lwip_proto |= NETCONN_TYPE_IPV6;
#endif

    s->conn = netconn_new_with_callback(lwip_proto, mbed_lwip_socket_callback);

    if (!s->conn) {
        mbed_lwip_arena_dealloc(s);
        return NSAPI_ERROR_NO_SOCKET;
    }

    netconn_set_recvtimeout(s->conn, 1);
    *(struct lwip_socket **)handle = s;
    return 0;
}

static nsapi_error_t mbed_lwip_socket_close(nsapi_stack_t *stack, nsapi_socket_t handle)
{
    struct lwip_socket *s = (struct lwip_socket *)handle;

    netbuf_delete(s->buf);
    err_t err = netconn_delete(s->conn);
    mbed_lwip_arena_dealloc(s);
    return mbed_lwip_err_remap(err);
}

static nsapi_error_t mbed_lwip_socket_bind(nsapi_stack_t *stack, nsapi_socket_t handle, nsapi_addr_t addr, uint16_t port)
{
    struct lwip_socket *s = (struct lwip_socket *)handle;
    ip_addr_t ip_addr;

    if (
#if LWIP_TCP
        (s->conn->type == NETCONN_TCP && s->conn->pcb.tcp->local_port != 0) ||
#endif
        (s->conn->type == NETCONN_UDP && s->conn->pcb.udp->local_port != 0)) {
        return NSAPI_ERROR_PARAMETER;
    }

    if (!convert_mbed_addr_to_lwip(&ip_addr, &addr)) {
        return NSAPI_ERROR_PARAMETER;
    }

    err_t err = netconn_bind(s->conn, &ip_addr, port);
    return mbed_lwip_err_remap(err);
}

static nsapi_error_t mbed_lwip_socket_listen(nsapi_stack_t *stack, nsapi_socket_t handle, int backlog)
{
    struct lwip_socket *s = (struct lwip_socket *)handle;

    err_t err = netconn_listen_with_backlog(s->conn, backlog);
    return mbed_lwip_err_remap(err);
}

static nsapi_error_t mbed_lwip_socket_connect(nsapi_stack_t *stack, nsapi_socket_t handle, nsapi_addr_t addr, uint16_t port)
{
    struct lwip_socket *s = (struct lwip_socket *)handle;
    ip_addr_t ip_addr;

    if (!convert_mbed_addr_to_lwip(&ip_addr, &addr)) {
        return NSAPI_ERROR_PARAMETER;
    }

    netconn_set_nonblocking(s->conn, false);
    err_t err = netconn_connect(s->conn, &ip_addr, port);
    netconn_set_nonblocking(s->conn, true);

    return mbed_lwip_err_remap(err);
}

static nsapi_error_t mbed_lwip_socket_accept(nsapi_stack_t *stack, nsapi_socket_t server, nsapi_socket_t *handle, nsapi_addr_t *addr, uint16_t *port)
{
    struct lwip_socket *s = (struct lwip_socket *)server;
    struct lwip_socket *ns = mbed_lwip_arena_alloc();
    if (!ns) {
        return NSAPI_ERROR_NO_SOCKET;
    }

    err_t err = netconn_accept(s->conn, &ns->conn);
    if (err != ERR_OK) {
        mbed_lwip_arena_dealloc(ns);
        return mbed_lwip_err_remap(err);
    }

    netconn_set_recvtimeout(ns->conn, 1);
    *(struct lwip_socket **)handle = ns;

    ip_addr_t peer_addr;
    (void) netconn_peer(ns->conn, &peer_addr, port);
    convert_lwip_addr_to_mbed(addr, &peer_addr);

    netconn_set_nonblocking(ns->conn, true);

    return 0;
}

static nsapi_size_or_error_t mbed_lwip_socket_send(nsapi_stack_t *stack, nsapi_socket_t handle, const void *data, nsapi_size_t size)
{
    struct lwip_socket *s = (struct lwip_socket *)handle;
    err_t err = netconn_write(s->conn, data, size, NETCONN_COPY);
    if (err != ERR_OK) {
        return mbed_lwip_err_remap(err);
    }

    return err;
}

static nsapi_size_or_error_t mbed_lwip_socket_recv(nsapi_stack_t *stack, nsapi_socket_t handle, void *data, nsapi_size_t size)
{
    struct lwip_socket *s = (struct lwip_socket *)handle;
		printf("\r\nmbed_lwip_socket_recv()");
    if (!s->buf) {
        err_t err = netconn_recv(s->conn, &s->buf);
        s->offset = 0;

        if (err != ERR_OK) {
            return mbed_lwip_err_remap(err);
        }
    }

    u16_t recv = netbuf_copy_partial(s->buf, data, (u16_t)size, s->offset);
    s->offset += recv;

    if (s->offset >= netbuf_len(s->buf)) {
        netbuf_delete(s->buf);
        s->buf = 0;
    }

    return recv;
}

static nsapi_size_or_error_t mbed_lwip_socket_sendto(nsapi_stack_t *stack, nsapi_socket_t handle, nsapi_addr_t addr, uint16_t port, const void *data, nsapi_size_t size)
{
    struct lwip_socket *s = (struct lwip_socket *)handle;
    ip_addr_t ip_addr;

    if (!convert_mbed_addr_to_lwip(&ip_addr, &addr)) {
        return NSAPI_ERROR_PARAMETER;
    }

    struct netbuf *buf = netbuf_new();
    err_t err = netbuf_ref(buf, data, (u16_t)size);
    if (err != ERR_OK) {
        netbuf_free(buf);
        return mbed_lwip_err_remap(err);
    }

    err = netconn_sendto(s->conn, buf, &ip_addr, port);
    netbuf_delete(buf);
    if (err != ERR_OK) {
        return mbed_lwip_err_remap(err);
    }

    return size;
}

static nsapi_size_or_error_t mbed_lwip_socket_recvfrom(nsapi_stack_t *stack, nsapi_socket_t handle, nsapi_addr_t *addr, uint16_t *port, void *data, nsapi_size_t size)
{
    struct lwip_socket *s = (struct lwip_socket *)handle;
    struct netbuf *buf;
		printf("\r\nmbed_lwip_socket_recvfrom()");
    err_t err = netconn_recv(s->conn, &buf);
    if (err != ERR_OK) {
        return mbed_lwip_err_remap(err);
    }

    convert_lwip_addr_to_mbed(addr, netbuf_fromaddr(buf));
    *port = netbuf_fromport(buf);

    u16_t recv = netbuf_copy(buf, data, (u16_t)size);
    netbuf_delete(buf);

    return recv;
}

static nsapi_error_t mbed_lwip_setsockopt(nsapi_stack_t *stack, nsapi_socket_t handle, int level, int optname, const void *optval, unsigned optlen)
{
    struct lwip_socket *s = (struct lwip_socket *)handle;

    switch (optname) {
#if LWIP_TCP
        case NSAPI_KEEPALIVE:
            if (optlen != sizeof(int) || s->conn->type != NETCONN_TCP) {
                return NSAPI_ERROR_UNSUPPORTED;
            }

            s->conn->pcb.tcp->so_options |= SOF_KEEPALIVE;
            return 0;

        case NSAPI_KEEPIDLE:
            if (optlen != sizeof(int) || s->conn->type != NETCONN_TCP) {
                return NSAPI_ERROR_UNSUPPORTED;
            }

            s->conn->pcb.tcp->keep_idle = *(int*)optval;
            return 0;

        case NSAPI_KEEPINTVL:
            if (optlen != sizeof(int) || s->conn->type != NETCONN_TCP) {
                return NSAPI_ERROR_UNSUPPORTED;
            }

            s->conn->pcb.tcp->keep_intvl = *(int*)optval;
            return 0;
#endif

        case NSAPI_REUSEADDR:
            if (optlen != sizeof(int)) {
                return NSAPI_ERROR_UNSUPPORTED;
            }

            if (*(int *)optval) {
                s->conn->pcb.ip->so_options |= SOF_REUSEADDR;
            } else {
                s->conn->pcb.ip->so_options &= ~SOF_REUSEADDR;
            }
            return 0;

        default:
            return NSAPI_ERROR_UNSUPPORTED;
    }
}

static void mbed_lwip_socket_attach(nsapi_stack_t *stack, nsapi_socket_t handle, void (*callback)(void *), void *data)
{
    struct lwip_socket *s = (struct lwip_socket *)handle;

    s->cb = callback;
    s->data = data;
}

/* LWIP network stack */
const nsapi_stack_api_t lwip_stack_api = {
    .gethostbyname      = mbed_lwip_gethostbyname,
    .add_dns_server     = mbed_lwip_add_dns_server,
    .socket_open        = mbed_lwip_socket_open,
    .socket_close       = mbed_lwip_socket_close,
    .socket_bind        = mbed_lwip_socket_bind,
    .socket_listen      = mbed_lwip_socket_listen,
    .socket_connect     = mbed_lwip_socket_connect,
    .socket_accept      = mbed_lwip_socket_accept,
    .socket_send        = mbed_lwip_socket_send,
    .socket_recv        = mbed_lwip_socket_recv,
    .socket_sendto      = mbed_lwip_socket_sendto,
    .socket_recvfrom    = mbed_lwip_socket_recvfrom,
    .setsockopt         = mbed_lwip_setsockopt,
    .socket_attach      = mbed_lwip_socket_attach,
};

nsapi_stack_t lwip_stack = {
    .stack_api = &lwip_stack_api,
};
