/* mbed Microcontroller Library
 * Copyright (c) 2016 ARM Limited
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

#include <errno.h>

#define tr_debug(...) printf(__VA_ARGS__) //dummies if feature common pal is not added
#define tr_info(...)  printf(__VA_ARGS__)//dummies if feature common pal is not added
#define tr_error(...) printf(__VA_ARGS__) //dummies if feature common pal is not added

#include "stdio.h"
#include "lwip/tcpip.h"
#include "lwip/tcp.h"
#include "lwip/ip.h"
#include "lwip/dns.h"
#include "lwip/pbuf.h"

#include "netif/ppp/pppapi.h"
#include "nsapi_ppp.h"
#include "ppp_lwip.h"
#include "lwip_stack.h"
#include "lib/ampm_filestream.h"
#include "comm.h"
#if LWIP_PPP_API

//static EventQueue *event_queue;
//static Thread *event_thread;
//static volatile bool event_queued;
static nsapi_error_t connect_error_code;

// Just one interface for now
static FILE *my_stream_in;
static FILE *my_stream_out;
static ppp_pcb *my_ppp_pcb;
static bool ppp_active = false;
static const char *login;
static const char *pwd;
static sys_sem_t ppp_close_sem;

static void  (*connection_status_cb)(nsapi_error_t err);
/*
static EventQueue *prepare_event_queue()
{
    if (event_queue) {
        return event_queue;
    }

    // Should be trying to get a global shared event queue here!
    // Shouldn't have to be making a private thread!

    // Only need to queue 2 events. new blows on failure.
    event_queue = new EventQueue(2 * EVENTS_EVENT_SIZE, NULL);
    event_thread = new Thread(osPriorityNormal, PPP_THREAD_STACK_SIZE);

    if (event_thread->start(callback(event_queue, &EventQueue::dispatch_forever)) != osOK) {
        delete event_thread;
        delete event_queue;
        return NULL;
    }

    return event_queue;
}
*/
static u32_t ppp_output(ppp_pcb *pcb, u8_t *data, u32_t len, void *ctx)
{
		AMPM_FILEHANDLE *fh = (AMPM_FILEHANDLE *)my_stream_out;
    if (my_stream_out == NULL) {
        return 0;
    }

    // pollfh fhs;
    // fhs.fh = stream;
    // fhs.events = POLLOUT;

    // LWIP expects us to block on write
    // File handle will be in non-blocking mode, because of read events.
    // Therefore must use poll to achieve the necessary block for writing.

    uint32_t written = 0;
    while (len != 0) {
        // Block forever until we're selected - don't care about reason we wake;
        // return from write should tell us what's up.
        //poll(&fhs, 1, -1);
        // This write will be non-blocking, but blocking would be fine.
				fflush(my_stream_out);
				size_t ret;
				if(len > fh->size)
					ret = fwrite(data,1,fh->size, my_stream_out);
				else
					ret = fwrite(data,1,len, my_stream_out);
        if (ret == 0) {
            break;
        }
				fseek(my_stream_out,0,SEEK_SET);
				fh->callback();
        written += ret;
        data += ret;
        len -= ret;
    }
//			for(written = 0;written < len;written++)
//			{
//				COMM_Putc(data[written]);
//			}
//    /tr_debug("> %ld bytes of data written\n", (long) written);

    return written;
}

static void ppp_link_status(ppp_pcb *pcb, int err_code, void *ctx)
{
    nsapi_error_t mapped_err_code = NSAPI_ERROR_NO_CONNECTION;

    switch(err_code) {
        case PPPERR_NONE:
            mapped_err_code = NSAPI_ERROR_OK;
            tr_info("status_cb: Connected");
#if PPP_IPV4_SUPPORT
            tr_debug("   our_ipaddr  = %s", ipaddr_ntoa(&ppp_netif(pcb)->ip_addr));
            tr_debug("   his_ipaddr  = %s", ipaddr_ntoa(&ppp_netif(pcb)->gw));
            tr_debug("   netmask     = %s", ipaddr_ntoa(&ppp_netif(pcb)->netmask));
#if LWIP_DNS
            const ip_addr_t *ns;
            ns = dns_getserver(0);
            if (ns) {
                tr_debug("   dns1        = %s", ipaddr_ntoa(ns));
            }
            ns = dns_getserver(1);
            if (ns) {
                tr_debug("   dns2        = %s", ipaddr_ntoa(ns));
            }
#endif /* LWIP_DNS */
#endif /* PPP_IPV4_SUPPORT */
#if PPP_IPV6_SUPPORT
            tr_debug("   our6_ipaddr = %s", ip6addr_ntoa(netif_ip6_addr(ppp_netif(pcb), 0)));
#endif /* PPP_IPV6_SUPPORT */
            break;

        case PPPERR_PARAM:
            tr_info("status_cb: Invalid parameter");
            break;

        case PPPERR_OPEN:
            tr_info("status_cb: Unable to open PPP session");
            break;

        case PPPERR_DEVICE:
            tr_info("status_cb: Invalid I/O device for PPP");
            break;

        case PPPERR_ALLOC:
            tr_info("status_cb: Unable to allocate resources");
            break;

        case PPPERR_USER:
            tr_info("status_cb: User interrupt");
            break;

        case PPPERR_CONNECT:
            tr_info("status_cb: Connection lost");
            mapped_err_code = NSAPI_ERROR_CONNECTION_LOST;
            break;

        case PPPERR_AUTHFAIL:
            tr_info("status_cb: Failed authentication challenge");
            mapped_err_code = NSAPI_ERROR_AUTH_FAILURE;
            break;

        case PPPERR_PROTOCOL:
            tr_info("status_cb: Failed to meet protocol");
            break;

        case PPPERR_PEERDEAD:
            tr_info("status_cb: Connection timeout");
            mapped_err_code = NSAPI_ERROR_CONNECTION_TIMEOUT;
            break;

        case PPPERR_IDLETIMEOUT:
            tr_info("status_cb: Idle Timeout");
            break;

        case PPPERR_CONNECTTIME:
            tr_info("status_cb: Max connect time reached");
            break;

        case PPPERR_LOOPBACK:
            tr_info("status_cb: Loopback detected");
            break;

        default:
            tr_info("status_cb: Unknown error code %d", err_code);
            break;

    }

    if (err_code == PPPERR_NONE) {
        /* suppress generating a callback event for connection up
         * Because connect() call is blocking, why wait for a callback */
        return;
    }

    /* If some error happened, we need to properly shutdown the PPP interface  */
    if (ppp_active) {
        ppp_active = false;
        connect_error_code = mapped_err_code;
        sys_sem_signal(&ppp_close_sem);
    }

    /* Alright, PPP interface is down, we need to notify upper layer */
    if (connection_status_cb) {
        connection_status_cb(mapped_err_code);
    }
}

void handle_modem_hangup()
{
    if (my_ppp_pcb->phase != PPP_PHASE_DEAD) {
        ppp_close(my_ppp_pcb, 1);
    }
}

#if !PPP_INPROC_IRQ_SAFE
#error "PPP_INPROC_IRQ_SAFE must be enabled"
#endif
static void ppp_input()
{
    // Allow new events from now, avoiding potential races around the read
    //event_queued = false;

    if (!my_stream_in) {
        return;
    }

    // // Non-blocking error check handler
    // pollfh fhs;
    // fhs.fh = my_stream;
    // fhs.events = POLLIN;
    // poll(&fhs, 1, 0);
    // if (fhs.revents & (POLLHUP|POLLERR|POLLNVAL)) {
        // handle_modem_hangup();
        // return;
    // }

    // Infinite loop, but we assume that we can read faster than the
    // serial, so we will fairly rapidly hit -EAGAIN.
		fseek(my_stream_in,0,SEEK_SET);
    for (;;) {
        u8_t buffer[16];
        size_t len = fread(buffer,1, sizeof buffer,my_stream_in);
        if (len == 0) {
            break;
        }
        pppos_input(my_ppp_pcb, buffer, len);
    }
    return;
}

static void stream_cb() {
    if (my_stream_in /*&& !event_queued*/) {
        // event_queued = true;
        // if (event_queue->call(callback(ppp_input)) == 0) {
            // event_queued = false;
        // }
			ppp_input();
    }
}

err_t ppp_lwip_is_dead()
{
	if(my_ppp_pcb == NULL || my_ppp_pcb->phase == PPP_PHASE_DEAD)
		return ERR_OK;
	return -1;
}

err_t ppp_lwip_connect()
{
#if PPP_AUTH_SUPPORT
   ppp_set_auth(my_ppp_pcb, PPPAUTHTYPE_NONE, login, pwd);
#endif //PPP_AUTH_SUPPORT
   ppp_active = true;
   err_t ret = ppp_connect(my_ppp_pcb, 0);
   // lwIP's ppp.txt says input must not be called until after connect
   if (ret == ERR_OK) {
      // *((AMPM_FILEHANDLE *)&my_stream).callback = stream_cb;
   } else {
       ppp_active = false;
   }
   return ret;
}

err_t ppp_lwip_disconnect()
{
		if(my_ppp_pcb == NULL) return -1;
    err_t ret = ppp_close(my_ppp_pcb, 0);
    if (ret != ERR_OK) {
        return ret;
    }
		if(ppp_active)
		{
    /* close call made, now let's catch the response in the status callback */
			sys_arch_sem_wait(&ppp_close_sem, 10000);
		}
    /* Detach callbacks, and put handle back to default blocking mode */
    // my_stream->sigio(Callback<void()>());
    // my_stream->set_blocking(true);
    //my_stream_in = NULL;
		//my_stream_out = NULL;
    return ret;
}

nsapi_error_t ppp_lwip_if_init(struct netif *netif, const nsapi_ip_stack_t stack)
{
//    if (!prepare_event_queue()) {
//        return NSAPI_ERROR_NO_MEMORY;
//    }

    if (!my_ppp_pcb) {
        my_ppp_pcb = pppos_create(netif,
                               ppp_output, ppp_link_status, NULL);
        if (!my_ppp_pcb) {
            return NSAPI_ERROR_DEVICE_ERROR;
        }

        sys_sem_new(&ppp_close_sem, 0);
    }

#if LWIP_IPV4
    if (stack != IPV6_STACK) {
        ppp_set_usepeerdns(my_ppp_pcb, true);
    }
#endif

#if LWIP_IPV4 && LWIP_IPV6
    if (stack == IPV4_STACK) {
        my_ppp_pcb->ipv6cp_disabled = true;
    } else if (stack == IPV6_STACK) {
        my_ppp_pcb->ipcp_disabled = true;
    }
#endif

    return NSAPI_ERROR_OK;
}

nsapi_error_t nsapi_ppp_error_code()
{
    return connect_error_code;
}

nsapi_error_t nsapi_ppp_connect(FILE *stream_in,FILE *stream_out, void  (*cb)(nsapi_error_t err), const char *uname, const char *password, const nsapi_ip_stack_t stack)
{
		AMPM_FILEHANDLE *fh = (AMPM_FILEHANDLE *)stream_in;
    my_stream_in = stream_in;
		my_stream_out = stream_out;
		fh->callback = stream_cb;
    connection_status_cb = cb;
    login = uname;
    pwd = password;
		while(fgetc(my_stream_in) != EOF);
		return NSAPI_ERROR_OK;
//    // mustn't start calling input until after connect -
//    // attach deferred until ppp_lwip_connect, called from mbed_lwip_bringup
//    nsapi_error_t retcode = mbed_lwip_bringup_2(false, true, NULL, NULL, NULL, stack);

//    if (retcode != NSAPI_ERROR_OK && connect_error_code != NSAPI_ERROR_OK) {
//        return connect_error_code;
//    } else {
//        return retcode;
//    }
}

nsapi_error_t nsapi_ppp_disconnect(void)
{
    return mbed_lwip_ppp_bringdown(); 
}

//NetworkStack *nsapi_ppp_get_stack()
//{
//    return nsapi_create_stack(&lwip_stack);
//}

const char *nsapi_ppp_get_ip_addr(FILE *stream)
{
    static char ip_addr[IPADDR_STRLEN_MAX];

    if (stream == my_stream_in) {

        if (mbed_lwip_get_ip_address(ip_addr, IPADDR_STRLEN_MAX)) {
            return ip_addr;
        }
    }

    return NULL;
}
const char *nsapi_ppp_get_netmask(FILE *stream)
{
#if !LWIP_IPV4
    return NULL;
#endif

    static char netmask[IPADDR_STRLEN_MAX];
    if (stream == my_stream_in) {
        if (mbed_lwip_get_netmask(netmask, IPADDR_STRLEN_MAX)) {
            return netmask;
        }
    }

    return NULL;
}
const char *nsapi_ppp_get_gw_addr(FILE *stream)
{
#if !LWIP_IPV4
    return NULL;
#endif

    static char gwaddr[IPADDR_STRLEN_MAX];
    if (stream == my_stream_in) {
        if (mbed_lwip_get_gateway(gwaddr, IPADDR_STRLEN_MAX)) {
            return gwaddr;
        }
    }

    return NULL;
}

#endif /* LWIP_PPP_API */

