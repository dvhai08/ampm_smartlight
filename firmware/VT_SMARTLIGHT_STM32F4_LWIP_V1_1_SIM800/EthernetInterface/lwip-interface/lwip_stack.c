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

#include "eth_arch.h"
#include "lwip/opt.h"
#include "lwip/api.h"
#include "lwip/inet.h"
#include "lwip/netif.h"
#include "lwip/dhcp.h"
#include "lwip/tcpip.h"
#include "lwip/tcp.h"
#include "lwip/ip.h"
#include "lwip/mld6.h"
#include "lwip/dns.h"
#include "lwip/udp.h"
#include "netif/lwip_ethernet.h"
#include "emac_api.h"
#include "ppp_lwip.h"

static nsapi_error_t mbed_lwip_err_remap(err_t err);



static bool lwip_inited = false;
static bool lwip_connected = false;
static bool netif_inited = false;



/* TCP/IP and Network Interface Initialisation */
struct netif lwip_netif;
#if LWIP_DHCP
static bool lwip_dhcp = false;
#endif
static char lwip_mac_address[NSAPI_MAC_SIZE];






#if LWIP_IPV4
static const ip_addr_t *mbed_lwip_get_ipv4_addr(const struct netif *netif)
{
    if (!netif_is_up(netif)) {
        return NULL;
    }

    if (!ip4_addr_isany(netif_ip4_addr(netif))) {
        return netif_ip_addr4(netif);
    }

    return NULL;
}
#endif

#if LWIP_IPV6
static const ip_addr_t *mbed_lwip_get_ipv6_addr(const struct netif *netif)
{

    if (!netif_is_up(netif)) {
        return NULL;
    }

    for (int i = 0; i < LWIP_IPV6_NUM_ADDRESSES; i++) {
        if (ip6_addr_isvalid(netif_ip6_addr_state(netif, i)) &&
                !ip6_addr_islinklocal(netif_ip6_addr(netif, i))) {
            return netif_ip_addr6(netif, i);
        }
    }

    return NULL;
}
#endif

const ip_addr_t *mbed_lwip_get_ip_addr(bool any_addr, const struct netif *netif)
{
    const ip_addr_t *pref_ip_addr = 0;
    const ip_addr_t *npref_ip_addr = 0;

#if LWIP_IPV4 && LWIP_IPV6
#if IP_VERSION_PREF == PREF_IPV4
    pref_ip_addr = mbed_lwip_get_ipv4_addr(netif);
    npref_ip_addr = mbed_lwip_get_ipv6_addr(netif);
#else
    pref_ip_addr = mbed_lwip_get_ipv6_addr(netif);
    npref_ip_addr = mbed_lwip_get_ipv4_addr(netif);
#endif
#elif LWIP_IPV6
    pref_ip_addr = mbed_lwip_get_ipv6_addr(netif);
#elif LWIP_IPV4
    pref_ip_addr = mbed_lwip_get_ipv4_addr(netif);
#endif

    if (pref_ip_addr) {
        return pref_ip_addr;
    } else if (npref_ip_addr && any_addr) {
        return npref_ip_addr;
    }

    return NULL;
}

static void add_dns_addr_to_dns_list_index(const u8_t addr_type, const u8_t index)
{
#if LWIP_IPV6
    if (addr_type == IPADDR_TYPE_V6) {
        /* 2001:4860:4860::8888 google */
        ip_addr_t ipv6_dns_addr = IPADDR6_INIT(
                PP_HTONL(0x20014860UL),
                PP_HTONL(0x48600000UL),
                PP_HTONL(0x00000000UL),
                PP_HTONL(0x00008888UL));
        dns_setserver(index, &ipv6_dns_addr);
    }
#endif
#if LWIP_IPV4
    if (addr_type == IPADDR_TYPE_V4) {
        /* 8.8.8.8 google */
        ip_addr_t ipv4_dns_addr = IPADDR4_INIT(0x08080808);
        dns_setserver(index, &ipv4_dns_addr);
    }
#endif
}

static int get_ip_addr_type(const ip_addr_t *ip_addr)
{
#if LWIP_IPV6
    if (IP_IS_V6(ip_addr)) {
        return IPADDR_TYPE_V6;
    }
#endif
#if LWIP_IPV4
    if (IP_IS_V4(ip_addr)) {
        return IPADDR_TYPE_V4;
    }
#endif
    return IPADDR_TYPE_ANY;
}

void add_dns_addr(struct netif *lwip_netif)
{
    // Check for existing dns address
    for (char numdns = 0; numdns < DNS_MAX_SERVERS; numdns++) {
        const ip_addr_t *dns_ip_addr = dns_getserver(numdns);
        if (!ip_addr_isany(dns_ip_addr)) {
            return;
        }
    }

    // Get preferred ip version
    const ip_addr_t *ip_addr = mbed_lwip_get_ip_addr(false, lwip_netif);
    u8_t addr_type = IPADDR_TYPE_ANY;

    // Add preferred ip version dns address to index 0
    if (ip_addr) {
        addr_type = get_ip_addr_type(ip_addr);
        add_dns_addr_to_dns_list_index(addr_type, 0);
    }

#if LWIP_IPV4 && LWIP_IPV6
    if (!ip_addr) {
        // Get address for any ip version
        ip_addr = mbed_lwip_get_ip_addr(true, lwip_netif);
        if (!ip_addr) {
            return;
        }
        addr_type = get_ip_addr_type(ip_addr);
        // Add the dns address to index 0
        add_dns_addr_to_dns_list_index(addr_type, 0);
    }

    if (addr_type == IPADDR_TYPE_V4) {
        // If ipv4 is preferred and ipv6 is available add ipv6 dns address to index 1
        ip_addr = mbed_lwip_get_ipv6_addr(lwip_netif);
    } else if (addr_type == IPADDR_TYPE_V6) {
        // If ipv6 is preferred and ipv4 is available add ipv4 dns address to index 1
        ip_addr = mbed_lwip_get_ipv4_addr(lwip_netif);
    } else {
        ip_addr = NULL;
    }

    if (ip_addr) {
        addr_type = get_ip_addr_type(ip_addr);
        add_dns_addr_to_dns_list_index(addr_type, 1);
    }
#endif
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

#if LWIP_ETHERNET
static void mbed_lwip_set_mac_address(struct netif *netif)
{
#if (MBED_MAC_ADDRESS_SUM != MBED_MAC_ADDR_INTERFACE)
    netif->hwaddr[0] = MBED_MAC_ADDR_0;
    netif->hwaddr[1] = MBED_MAC_ADDR_1;
    netif->hwaddr[2] = MBED_MAC_ADDR_2;
    netif->hwaddr[3] = MBED_MAC_ADDR_3;
    netif->hwaddr[4] = MBED_MAC_ADDR_4;
    netif->hwaddr[5] = MBED_MAC_ADDR_5;
#else
    mbed_mac_address((char *)netif->hwaddr);
#endif

    netif->hwaddr_len = ETH_HWADDR_LEN;

    /* Use mac address as additional seed to random number generator */
    uint64_t seed = netif->hwaddr[0];
    for (uint8_t i = 1; i < 8; i++) {
        seed <<= 8;
        seed |= netif->hwaddr[i % 6];
    }
    lwip_add_random_seed(seed);
}

static void mbed_lwip_record_mac_address(const struct netif *netif)
{
    const u8_t *mac = netif->hwaddr;
    snprintf(lwip_mac_address, NSAPI_MAC_SIZE, "%02x:%02x:%02x:%02x:%02x:%02x",
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}
#endif // LWIP_ETHERNET

/* LWIP interface implementation */
const char *mbed_lwip_get_mac_address(void)
{
    return lwip_mac_address[0] ? lwip_mac_address : NULL;
}

char *mbed_lwip_get_ip_address(char *buf, nsapi_size_t buflen)
{
    const ip_addr_t *addr = mbed_lwip_get_ip_addr(true, &lwip_netif);
    if (!addr) {
        return NULL;
    }
#if LWIP_IPV6
    if (IP_IS_V6(addr)) {
        return ip6addr_ntoa_r(ip_2_ip6(addr), buf, buflen);
    }
#endif
#if LWIP_IPV4
    if (IP_IS_V4(addr)) {
        return ip4addr_ntoa_r(ip_2_ip4(addr), buf, buflen);
    }
#endif
#if LWIP_IPV6 && LWIP_IPV4
    return NULL;
#endif
}

char *mbed_lwip_get_netmask(char *buf, nsapi_size_t buflen)
{
#if LWIP_IPV4
    const ip4_addr_t *addr = netif_ip4_netmask(&lwip_netif);
    if (!ip4_addr_isany(addr)) {
        return ip4addr_ntoa_r(addr, buf, buflen);
    } else {
        return NULL;
    }
#else
    return NULL;
#endif
}

char *mbed_lwip_get_gateway(char *buf, nsapi_size_t buflen)
{
#if LWIP_IPV4
    const ip4_addr_t *addr = netif_ip4_gw(&lwip_netif);
    if (!ip4_addr_isany(addr)) {
        return ip4addr_ntoa_r(addr, buf, buflen);
    } else {
        return NULL;
    }
#else
    return NULL;
#endif
}

void mbed_lwip_core_init(void)
{

    // Check if we've already brought up lwip
    if (!lwip_inited) {
	// Seed lwip random
        lwip_seed_random();

        // Initialise TCP sequence number
        uint32_t tcp_isn_secret[4];
        for (int i = 0; i < 4; i++) {
            tcp_isn_secret[i] = LWIP_RAND();
        }
        lwip_init_tcp_isn(0, (u8_t *) &tcp_isn_secret);

        sys_sem_new(&lwip_tcpip_inited, 0);
        sys_sem_new(&lwip_netif_linked, 0);
        sys_sem_new(&lwip_netif_unlinked, 0);
        sys_sem_new(&lwip_netif_has_any_addr, 0);
#if PREF_ADDR_TIMEOUT
        sys_sem_new(&lwip_netif_has_pref_addr, 0);
#endif
#if BOTH_ADDR_TIMEOUT
        sys_sem_new(&lwip_netif_has_both_addr, 0);
#endif
        tcpip_init(mbed_lwip_tcpip_init_irq, NULL);
        sys_arch_sem_wait(&lwip_tcpip_inited, 0);

        lwip_inited = true;
    }
}


nsapi_error_t mbed_lwip_ppp_bringup(void)
{
    // Check if we've already connected
    if (lwip_connected) {
        return NSAPI_ERROR_PARAMETER;
    }

    mbed_lwip_core_init();

    nsapi_error_t ret;
    if (netif_inited) {
          ret = NSAPI_ERROR_OK;
    } else {
         ret = ppp_lwip_if_init(&lwip_netif, 0);
		}

    if (ret != NSAPI_ERROR_OK) {
        return ret;
    }

    netif_inited = true;

    netif_set_default(&lwip_netif);
    netif_set_link_callback(&lwip_netif, mbed_lwip_netif_link_irq);
    netif_set_status_callback(&lwip_netif, mbed_lwip_netif_status_irq);

		err_t err = ppp_lwip_connect();
		if (err) {
			 return mbed_lwip_err_remap(err);
		}
		 
    return NSAPI_ERROR_OK;
}

nsapi_error_t mbed_lwip_ppp_link_status(void)
{
		if (!netif_is_link_up(&lwip_netif)) {
         return NSAPI_ERROR_NO_CONNECTION;
    }
		
    // If doesn't have address
    if (!mbed_lwip_get_ip_addr(true, &lwip_netif)) {
        return NSAPI_ERROR_DHCP_FAILURE;
    }

		return NSAPI_ERROR_OK; 
}

void mbed_lwip_ppp_link_set_connected(void)
{
		add_dns_addr(&lwip_netif);
    lwip_connected = true;
}


#if LWIP_IPV6
void mbed_lwip_clear_ipv6_addresses(struct netif *lwip_netif)
{
    for (u8_t i = 0; i < LWIP_IPV6_NUM_ADDRESSES; i++) {
        netif_ip6_addr_set_state(lwip_netif, i, IP6_ADDR_INVALID);
    }
}
#endif

nsapi_error_t mbed_lwip_ppp_bringdown(void)
{
    // Check if we've connected
    if (!lwip_connected && (ppp_lwip_is_dead() == ERR_OK)) {
        return NSAPI_ERROR_PARAMETER;
    }

			/* this is a blocking call, returns when PPP is properly closed */
		 err_t err = ppp_lwip_disconnect();
		 if (err) {
				 return mbed_lwip_err_remap(err);
		 }

//		 if (netif_is_link_up(&lwip_netif)) {
//				 if (sys_arch_sem_wait(&lwip_netif_unlinked, 15000) == SYS_ARCH_TIMEOUT) {
//						 return NSAPI_ERROR_DEVICE_ERROR;
//				 }
//		 }

    sys_sem_free(&lwip_netif_has_any_addr);
    sys_sem_new(&lwip_netif_has_any_addr, 0);
#if PREF_ADDR_TIMEOUT
    sys_sem_free(&lwip_netif_has_pref_addr);
    sys_sem_new(&lwip_netif_has_pref_addr, 0);
#endif
#if BOTH_ADDR_TIMEOUT
    sys_sem_free(&lwip_netif_has_both_addr);
    sys_sem_new(&lwip_netif_has_both_addr, 0);
#endif
    lwip_has_addr_state = 0;
    lwip_connected = false;
    return 0;
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
        case ERR_ALREADY:
            return NSAPI_ERROR_ALREADY;
        case ERR_ISCONN:
            return NSAPI_ERROR_IS_CONNECTED;
        default:
            return NSAPI_ERROR_DEVICE_ERROR;
    }
}

