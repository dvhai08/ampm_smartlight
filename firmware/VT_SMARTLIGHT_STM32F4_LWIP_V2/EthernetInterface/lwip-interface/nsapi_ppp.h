/** \addtogroup netsocket */
/** @{*/
/* nsapi_ppp.h
 * Modified work Copyright (c) 2017 ARM Limited
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
#ifndef NSAPI_PPP_H_
#define NSAPI_PPP_H_
#include <stdio.h>
#include "nsapi_types.h"
/** Provide access to the NetworkStack object
 *
 *  @return The underlying NetworkStack object
 */
//NetworkStack *nsapi_ppp_get_stack();


/** Connect to a PPP pipe
 *
 *  @param stream       Pointer to a device type file handle (descriptor)
 *  @param status_cb    Optional, user provided callback for connection status
 *  @param uname        Optional, username for the connection
 *  @param pwd          Optional, password for the connection
 *  @param stack        Optional, stack for the connection
 *
 *  @return             0 on success, negative error code on failure
 */
nsapi_error_t nsapi_ppp_connect(FILE *stream_in,FILE *stream_out, void  (*cb)(nsapi_error_t err), const char *uname, const char *password, const nsapi_ip_stack_t stack);

/** Close a PPP connection
 *
 *  @param stream       Pointer to a device type file handle (descriptor)
 *
 *  @return             0 on success, negative error code on failure
 */
nsapi_error_t nsapi_ppp_disconnect(void);

/** Get IP address
 *
 *  After a successful connection, this API can be used to retrieve assigned IP address.
 *
 *  @param stream       Pointer to a device type file handle (descriptor)
 *
 *  @return             A string containing IP address or NULL
 */
const char *nsapi_ppp_get_ip_addr(FILE *stream);

/** Get network mask
 *
 *  After a successful connection, this API can be used to retrieve network mask
 *  in case of an IPv4 network.
 *
 *  @param stream       Pointer to a device type file handle (descriptor)
 *
 *  @return             A string containing network mask or NULL
 */
const char *nsapi_ppp_get_netmask(FILE *stream);

/** Get gateway address
 *
 *  After a successful connection, this API can be used to retrieve IP address
 *  of the default gateway in case of an IPv4 network.
 *
 *  @param stream       Pointer to a device type file handle (descriptor)
 *
 *  @return             A string containing gateway IP address or NULL
 */
const char *nsapi_ppp_get_gw_addr(FILE *stream);


#endif /* NSAPI_PPP_H_ */
