/*
 * mbed SDK
 * Copyright (c) 2017 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// Automatically generated configuration file.
// DO NOT EDIT, content will be overwritten.

#ifndef __MBED_CONFIG_DATA__
#define __MBED_CONFIG_DATA__

// Configuration parameters
#define MBED_CONF_LWIP_ENABLE_PPP_TRACE                0      // set by library:lwip
#define MBED_CONF_LWIP_ADDR_TIMEOUT                    5      // set by library:lwip
#define MBED_CONF_LWIP_IPV4_ENABLED                    1      // set by library:lwip
#define MBED_CONF_LWIP_DEFAULT_THREAD_STACKSIZE        512    // set by library:lwip
#define MBED_CONF_PPP_CELL_IFACE_APN_LOOKUP            1      // set by library:ppp-cell-iface
#define MBED_CONF_EVENTS_PRESENT                       1      // set by library:events
#define MBED_CONF_LWIP_TCPIP_THREAD_STACKSIZE          1200   // set by library:lwip
#define MBED_CONF_LWIP_PPP_THREAD_STACKSIZE            512    // set by library:lwip
#define MBED_CONF_PLATFORM_STDIO_FLUSH_AT_EXIT         1      // set by library:platform
#define MBED_CONF_DRIVERS_UART_SERIAL_RXBUF_SIZE       256    // set by library:drivers
#define MBED_CONF_NSAPI_PRESENT                        1      // set by library:nsapi
#define MBED_CONF_FILESYSTEM_PRESENT                   1      // set by library:filesystem
#define MBED_CONF_PPP_CELL_IFACE_BAUD_RATE             115200 // set by library:ppp-cell-iface
#define MBED_CONF_PPP_CELL_IFACE_AT_PARSER_BUFFER_SIZE 256    // set by library:ppp-cell-iface
#define MBED_CONF_PLATFORM_STDIO_BAUD_RATE             115200   // set by library:platform
#define MBED_CONF_LWIP_IPV6_ENABLED                    0      // set by library:lwip
#define MBED_CONF_LWIP_IP_VER_PREF                     4      // set by library:lwip
#define MBED_CONF_LWIP_TCP_SERVER_MAX                  1      // set by library:lwip
#define MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE    9600   // set by library:platform
#define MBED_CONF_LWIP_TCP_SOCKET_MAX                  2      // set by library:lwip
#define MBED_CONF_RTOS_PRESENT                         1      // set by library:rtos
#define MBED_CONF_LWIP_TCP_ENABLED                     1      // set by library:lwip
#define MBED_CONF_LWIP_DEBUG_ENABLED                   1      // set by library:lwip
#define MBED_CONF_PPP_CELL_IFACE_AT_PARSER_TIMEOUT     8000   // set by library:ppp-cell-iface
#define STM32_D11_SPI_ETHERNET_PIN                     PA_7   // set by target:NUCLEO_F207ZG
#define NSAPI_PPP_AVAILABLE                            1      // set by library:lwip
#define NSAPI_PPP_IPV4_AVAILABLE											 1
#define MBED_CONF_DRIVERS_UART_SERIAL_TXBUF_SIZE       512    // set by library:drivers
#define MBED_CONF_LWIP_UDP_SOCKET_MAX                  4      // set by library:lwip
#define MBED_CONF_LWIP_USE_MBED_TRACE                  0      // set by library:lwip
#define MBED_CONF_PLATFORM_STDIO_CONVERT_NEWLINES      0      // set by library:platform
#define MBED_CONF_LWIP_ETHERNET_ENABLED                0      // set by library:lwip
#define MBED_CONF_LWIP_SOCKET_MAX                      4      // set by library:lwip
#define MBED_CONF_EVENTS_SHARED_EVENTSIZE							128
#define MBED_CONF_EVENTS_SHARED_STACKSIZE							256
#define MBED_CONF_EVENTS_SHARED_HIGHPRIO_EVENTSIZE		128
#define MBED_CONF_EVENTS_SHARED_HIGHPRIO_STACKSIZE		256
#define MBED_CONF_EVENTS_SHARED_EVENTSIZE							128
#define MBED_HEAP_STATS_ENABLED													1
#define MBED_MEM_TRACING_ENABLED												1
// Macros
#define MBEDTLS_USER_CONFIG_FILE                    "mbedtls_entropy_config.h" // defined by application


//GPRS define
#define MBED_CONF_APP_DEFAULT_PIN											"1234"
#define MBED_CONF_APP_APN															"internet"
#define MBED_CONF_APP_USERNAME												"mms"
#define MBED_CONF_APP_PASSWORD												"mms"
#define MODEM_ON_BOARD 1
#define MODEM_ON_BOARD_UART 1
#define MBED_CONF_APP_MAX_RETRIES											3
#define MBED_CONF_APP_ECHO_SERVER											"42.119.2.208"
#define MBED_CONF_APP_ECHO_UDP_PORT										11111
#define MBED_CONF_APP_UDP_MAX_PACKET_SIZE							512
#define MBED_CONF_APP_ECHO_TCP_PORT										11111
#define MBED_CONF_APP_TCP_MAX_PACKET_SIZE							512

#define MDMTXD	GSM_TX
#define MDMRXD	GSM_RX
#define MDMDCD	PE_9
#define MDMRTS	PA_12
#define MDMCTS	PA_11
#define MDMPWRON	PB_4
//#define MDMRI
//#define MDMDTR
//#define MDMDSR

// Macros
#define UNITY_INCLUDE_CONFIG_H                                // defined by library:utest
//#define ENABLE_IOT_DEBUG									0
//#define ENABLE_IOT_TRACE									0
//#define ENABLE_IOT_INFO									0
#define ENABLE_IOT_WARN									1
#define ENABLE_IOT_ERROR									1

#endif
