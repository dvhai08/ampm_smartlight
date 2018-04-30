
#ifndef LWIP_NET_SOCKET_H
#define LWIP_NET_SOCKET_H


#include <stddef.h>
#include <stdint.h>

#define LWIP_ERR_NET_SOCKET_FAILED                 -0x0042  /**< Failed to open a socket. */
#define LWIP_ERR_NET_CONNECT_FAILED                    -0x0044  /**< The connection to the given server / port failed. */
#define LWIP_ERR_NET_BIND_FAILED                       -0x0046  /**< Binding of the socket failed. */
#define LWIP_ERR_NET_LISTEN_FAILED                     -0x0048  /**< Could not listen on the socket. */
#define LWIP_ERR_NET_ACCEPT_FAILED                     -0x004A  /**< Could not accept the incoming connection. */
#define LWIP_ERR_NET_RECV_FAILED                       -0x004C  /**< Reading information from the socket failed. */
#define LWIP_ERR_NET_SEND_FAILED                       -0x004E  /**< Sending information through the socket failed. */
#define LWIP_ERR_NET_CONN_RESET                        -0x0050  /**< Connection was reset by peer. */
#define LWIP_ERR_NET_UNKNOWN_HOST                      -0x0052  /**< Failed to get an IP address for the given hostname. */
#define LWIP_ERR_NET_BUFFER_TOO_SMALL                  -0x0043  /**< Buffer is too small to hold the data. */
#define LWIP_ERR_NET_INVALID_CONTEXT                   -0x0045  /**< The context is invalid, eg because it was free()ed. */

#define LWIP_ERR_SSL_WANT_READ                         -0x6900  /**< Connection requires a read call. */
#define LWIP_ERR_SSL_WANT_WRITE                        -0x6880  /**< Connection requires a write call. */
#define LWIP_ERR_SSL_TIMEOUT                           -0x6800  /**< The operation timed out. */
#define LWIP_ERR_SSL_CLIENT_RECONNECT                  -0x6780  /**< The client initiated a reconnect from the same port. */
#define LWIP_ERR_SSL_UNEXPECTED_RECORD                 -0x6700  /**< Record header looks valid but is not expected. */
#define LWIP_ERR_SSL_NON_FATAL                         -0x6680  /**< The alert message received indicates a non-fatal error. */



#define LWIP_NET_LISTEN_BACKLOG         10 /**< The backlog that listen() should use. */

#define LWIP_NET_PROTO_TCP 0 /**< The TCP transport protocol */
#define LWIP_NET_PROTO_UDP 1 /**< The UDP transport protocol */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Wrapper type for sockets.
 *
 * Currently backed by just a file descriptor, but might be more in the future
 * (eg two file descriptors for combined IPv4 + IPv6 support, or additional
 * structures for hand-made UDP demultiplexing).
 */
typedef struct
{
    int fd;             /**< The underlying file descriptor                 */
}
lwip_net_context;

/**
 * \brief          Initialize a context
 *                 Just makes the context ready to be used or freed safely.
 *
 * \param ctx      Context to initialize
 */
void lwip_net_init( int socket );

/**
 * \brief          Initiate a connection with host:port in the given protocol
 *
 * \param ctx      Socket to use
 * \param host     Host to connect to
 * \param port     Port to connect to
 * \param proto    Protocol: LWIP_NET_PROTO_TCP or LWIP_NET_PROTO_UDP
 *
 * \return         0 if successful, or one of:
 *                      LWIP_ERR_NET_SOCKET_API_FAILED,
 *                      LWIP_ERR_NET_UNKNOWN_HOST,
 *                      LWIP_ERR_NET_CONNECT_FAILED
 *
 * \note           Sets the socket in connected mode even with UDP.
 */
int lwip_net_connect(int *socket, const char *host, const char *port, int proto);

/**
 * \brief          Create a receiving socket on bind_ip:port in the chosen
 *                 protocol. If bind_ip == NULL, all interfaces are bound.
 *
 * \param ctx      Socket to use
 * \param bind_ip  IP to bind to, can be NULL
 * \param port     Port number to use
 * \param proto    Protocol: LWIP_NET_PROTO_TCP or LWIP_NET_PROTO_UDP
 *
 * \return         0 if successful, or one of:
 *                      LWIP_ERR_NET_SOCKET_FAILED,
 *                      LWIP_ERR_NET_BIND_FAILED,
 *                      LWIP_ERR_NET_LISTEN_FAILED
 *
 * \note           Regardless of the protocol, opens the sockets and binds it.
 *                 In addition, make the socket listening if protocol is TCP.
 */
int lwip_net_bind( int socket, const char *bind_ip, const char *port, int proto );

/**
 * \brief           Accept a connection from a remote client
 *
 * \param bind_ctx  Relevant socket
 * \param client_ctx Will contain the connected client socket
 * \param client_ip Will contain the client IP address
 * \param buf_size  Size of the client_ip buffer
 * \param ip_len    Will receive the size of the client IP written
 *
 * \return          0 if successful, or
 *                  LWIP_ERR_NET_ACCEPT_FAILED, or
 *                  LWIP_ERR_NET_BUFFER_TOO_SMALL if buf_size is too small,
 *                  LWIP_ERRSSL_WANT_READ if bind_fd was set to
 *                  non-blocking and accept() would block.
 */
int lwip_net_accept( lwip_net_context *bind_ctx,
                        lwip_net_context *client_ctx,
                        void *client_ip, size_t buf_size, size_t *ip_len );

/**
 * \brief          Set the socket blocking
 *
 * \param ctx      Socket to set
 *
 * \return         0 if successful, or a non-zero error code
 */
int lwip_net_set_block( int socket );

/**
 * \brief          Set the socket non-blocking
 *
 * \param ctx      Socket to set
 *
 * \return         0 if successful, or a non-zero error code
 */
int lwip_net_set_nonblock( int socket );

/**
 * \brief          Portable usleep helper
 *
 * \param usec     Amount of microseconds to sleep
 *
 * \note           Real amount of time slept will not be less than
 *                 select()'s timeout granularity (typically, 10ms).
 */
void lwip_net_usleep( unsigned long usec );

/**
 * \brief          Read at most 'len' characters. If no error occurs,
 *                 the actual amount read is returned.
 *
 * \param ctx      Socket
 * \param buf      The buffer to write to
 * \param len      Maximum length of the buffer
 *
 * \return         the number of bytes received,
 *                 or a non-zero error code; with a non-blocking socket,
 *                 LWIP_ERRSSL_WANT_READ indicates read() would block.
 */
int lwip_net_recv( int socket, unsigned char *buf, size_t len );

/**
 * \brief          Write at most 'len' characters. If no error occurs,
 *                 the actual amount read is returned.
 *
 * \param ctx      Socket
 * \param buf      The buffer to read from
 * \param len      The length of the buffer
 *
 * \return         the number of bytes sent,
 *                 or a non-zero error code; with a non-blocking socket,
 *                 LWIP_ERRSSL_WANT_WRITE indicates write() would block.
 */
int lwip_net_send( int socket, const unsigned char *buf, size_t len );

/**
 * \brief          Read at most 'len' characters, blocking for at most
 *                 'timeout' seconds. If no error occurs, the actual amount
 *                 read is returned.
 *
 * \param ctx      Socket
 * \param buf      The buffer to write to
 * \param len      Maximum length of the buffer
 * \param timeout  Maximum number of milliseconds to wait for data
 *                 0 means no timeout (wait forever)
 *
 * \return         the number of bytes received,
 *                 or a non-zero error code:
 *                 LWIP_ERRSSL_TIMEOUT if the operation timed out,
 *                 LWIP_ERRSSL_WANT_READ if interrupted by a signal.
 *
 * \note           This function will block (until data becomes available or
 *                 timeout is reached) even if the socket is set to
 *                 non-blocking. Handling timeouts with non-blocking reads
 *                 requires a different strategy.
 */
int lwip_net_recv_timeout( int socket, unsigned char *buf, size_t len,
                      uint32_t timeout );

/**
 * \brief          Gracefully shutdown the connection and free associated data
 *
 * \param ctx      The context to free
 */
void lwip_net_free( int *socket );

#ifdef __cplusplus
}
#endif

#endif /* net_sockets.h */
