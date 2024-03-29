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

#ifndef LWIP_STACK_H
#define LWIP_STACK_H

#include <stdbool.h>
#include "nsapi_types.h"
#include "emac_api.h"
#include "lwip/opt.h"
#ifdef __cplusplus
extern "C" {
#endif

extern struct netif lwip_netif;
// Access to lwip through the nsapi - be wary of API changes as external 1st-generation EMAC
// drivers attach through these.
nsapi_error_t mbed_lwip_init(emac_interface_t *emac);
nsapi_error_t mbed_lwip_ppp_bringup(void);
nsapi_error_t mbed_lwip_ppp_bringdown(void);
nsapi_error_t mbed_lwip_ppp_link_status(void);
void mbed_lwip_ppp_link_set_connected(void);
void mbed_lwip_core_init(void);
const char *mbed_lwip_get_mac_address(void);
char *mbed_lwip_get_ip_address(char *buf, nsapi_size_t buflen);
char *mbed_lwip_get_netmask(char *buf, nsapi_size_t buflen);
char *mbed_lwip_get_gateway(char *buf, nsapi_size_t buflen);

extern nsapi_stack_t lwip_stack;

#ifdef __cplusplus
}
#endif

#endif
