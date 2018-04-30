#include "lwip/lwip_net_socket.h"
#include "network_interface.h"
#include "lwip/sockets.h"
#include <string.h>
#include "lwip/netdb.h"
#include "FreeRTOS.h"
#include "task.h"
#include "errno.h"
#define accept(a,b,c)         lwip_accept(a,b,c)
#define bind(a,b,c)           lwip_bind(a,b,c)
#define shutdown(a,b)         lwip_shutdown(a,b)
#define closesocket(s)        lwip_close(s)
#define connect(a,b,c)        lwip_connect(a,b,c)
#define getsockname(a,b,c)    lwip_getsockname(a,b,c)
#define getpeername(a,b,c)    lwip_getpeername(a,b,c)
#define setsockopt(a,b,c,d,e) lwip_setsockopt(a,b,c,d,e)
#define getsockopt(a,b,c,d,e) lwip_getsockopt(a,b,c,d,e)
#define listen(a,b)           lwip_listen(a,b)
#define recv(a,b,c,d)         lwip_recv(a,b,c,d)
#define recvfrom(a,b,c,d,e,f) lwip_recvfrom(a,b,c,d,e,f)
#define send(a,b,c,d)         lwip_send(a,b,c,d)
#define sendto(a,b,c,d,e,f)   lwip_sendto(a,b,c,d,e,f)
#define socket(a,b,c)         lwip_socket(a,b,c)
#define select(a,b,c,d,e)     lwip_select(a,b,c,d,e)
#define ioctlsocket(a,b,c)    lwip_ioctl(a,b,c)
#define read(a,b,c)           lwip_read(a,b,c)
#define write(a,b,c)          lwip_write(a,b,c)
#define close(s)              lwip_close(s)

#define gethostbyname(name) lwip_gethostbyname(name)
#define gethostbyname_r(name, ret, buf, buflen, result, h_errnop) \
       lwip_gethostbyname_r(name, ret, buf, buflen, result, h_errnop)
#define freeaddrinfo(addrinfo) lwip_freeaddrinfo(addrinfo)
#define getaddrinfo(nodname, servname, hints, res) \
       lwip_getaddrinfo(nodname, servname, hints, res)
/*
 * Prepare for using the sockets interface
 */
static int net_prepare( void )
{

   return( 0 );
}

void lwip_net_init(int socket) {

	return;
}

int lwip_net_connect(int *socket, const char *host, const char *port, int proto)
{
	int ret;
	struct addrinfo hints, *addr_list, *cur;
	if( ( ret = net_prepare() ) != 0 )
        return( ret );
	/* Do name resolution with both IPv6 and IPv4 */
	memset( &hints, 0, sizeof( hints ) );
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = proto == LWIP_NET_PROTO_UDP ? SOCK_DGRAM : SOCK_STREAM;
	hints.ai_protocol = proto == LWIP_NET_PROTO_UDP ? IPPROTO_UDP : IPPROTO_TCP;
	if( getaddrinfo( host, port, &hints, &addr_list ) != 0 )
		return( LWIP_ERR_NET_UNKNOWN_HOST );

	/* Try the sockaddrs until a connection succeeds */
	ret = LWIP_ERR_NET_UNKNOWN_HOST;
	for( cur = addr_list; cur != NULL; cur = cur->ai_next )
	{
		*socket = (int) socket( cur->ai_family, cur->ai_socktype,
							cur->ai_protocol );
		if( *socket < 0 )
		{
			ret = LWIP_ERR_NET_SOCKET_FAILED;
			continue;
		}
		if( connect( *socket, cur->ai_addr, cur->ai_addrlen ) == 0 )
		{
			ret = 0;
			break;
		}
		close( *socket );
		ret = LWIP_ERR_NET_CONNECT_FAILED;
	}

	freeaddrinfo( addr_list );

	return( ret );
}

/*
 * Set the socket blocking or non-blocking
 */
int lwip_net_set_block( int socket )
{
#if ( defined(_WIN32) || defined(_WIN32_WCE) ) && !defined(EFIX64) && \
    !defined(EFI32)
    u_long n = 0;
    return( ioctlsocket( socket, FIONBIO, &n ) );
#else
    return( lwip_fcntl( socket, F_SETFL, lwip_fcntl( socket, F_GETFL ,0) & ~O_NONBLOCK ) );
#endif
}


int lwip_net_set_nonblock( int socket )
{
#if ( defined(_WIN32) || defined(_WIN32_WCE) ) && !defined(EFIX64) && \
    !defined(EFI32)
    u_long n = 1;
    return( ioctlsocket( socket, FIONBIO, &n ) );
#else
    return( lwip_fcntl( socket, F_SETFL, lwip_fcntl( socket, F_GETFL ,0) | O_NONBLOCK ) );
#endif
}

/*
 * Check if the requested operation would be blocking on a non-blocking socket
 * and thus 'failed' with a negative return value.
 *
 * Note: on a blocking socket this function always returns 0!
 */
static int net_would_block( const int socket )
{
    /*
     * Never return 'WOULD BLOCK' on a non-blocking socket
     */
    if( ( lwip_fcntl( socket, F_GETFL ,0) & O_NONBLOCK ) != O_NONBLOCK )
        return( 0 );

    switch( errno )
    {
#if defined EAGAIN
        case EAGAIN:
#endif
#if defined EWOULDBLOCK && EWOULDBLOCK != EAGAIN
        case EWOULDBLOCK:
#endif
            return( 1 );
    }
    return( 0 );
}

//int lwip_net_recv(int socket, unsigned char *buf, size_t len)
//{
//	int32_t ret;
//	int *pctx = (int *)ctx;
//	if(psocket < 0)
//		return -1;
//	ret = lwip_recv(psocket , buf, len,MSG_DONTWAIT);
//	if(ret == -1 && (get_socket_error(psocket) != EWOULDBLOCK))
//	{
//		if( net_would_block( ctx ) != 0 )
//			return( LWIP_ERR_SSL_WANT_READ );
//		return LWIP_ERR_SSL_TIMEOUT;
//	}
//	return ret;
//}

/*
 * Read at most 'len' characters
 */
int lwip_net_recv( int socket, unsigned char *buf, size_t len )
{
    int ret;

    if( socket < 0 )
        return( LWIP_ERR_NET_INVALID_CONTEXT );

    ret = (int) read( socket, buf, len);

    if( ret < 0 )
    {
        if( net_would_block( socket ) != 0 )
            return( LWIP_ERR_SSL_WANT_READ );

#if ( defined(_WIN32) || defined(_WIN32_WCE) ) && !defined(EFIX64) && \
    !defined(EFI32)
        if( WSAGetLastError() == WSAECONNRESET )
            return( LWIP_ERR_NET_CONN_RESET );
#else
        if( errno == EPIPE || errno == ECONNRESET )
            return( LWIP_ERR_NET_CONN_RESET );

        if( errno == EINTR )
            return( LWIP_ERR_SSL_WANT_READ );
#endif

        return( LWIP_ERR_NET_RECV_FAILED );
    }

    return( ret );
}

//int lwip_net_send(int socket, const unsigned char *buf, size_t len) {
//	int *pctx = (int *)ctx;
//	if(psocket < 0)
//		return -1;
//	return lwip_write(psocket, buf, len);
//}

/*
 * Write at most 'len' characters
 */
int lwip_net_send( int socket, const unsigned char *buf, size_t len )
{
    int ret;

    if( socket < 0 )
        return( LWIP_ERR_NET_INVALID_CONTEXT );

    ret = (int) write( socket, buf, len );

    if( ret < 0 )
    {
        if( net_would_block( socket ) != 0 )
            return( LWIP_ERR_SSL_WANT_WRITE );

#if ( defined(_WIN32) || defined(_WIN32_WCE) ) && !defined(EFIX64) && \
    !defined(EFI32)
        if( WSAGetLastError() == WSAECONNRESET )
            return( LWIP_ERR_NET_CONN_RESET );
#else
        if( errno == EPIPE || errno == ECONNRESET )
            return( LWIP_ERR_NET_CONN_RESET );

        if( errno == EINTR )
            return( LWIP_ERR_SSL_WANT_WRITE );
#endif

        return( LWIP_ERR_NET_SEND_FAILED );
    }

    return( ret );
}

//int lwip_net_recv_timeout(int socket, unsigned char *buf, size_t len,
//	uint32_t timeout)
//{
//	
//	uint32_t time = HAL_GetTick();
//	int32_t ret;
//	int *pctx = (int *)ctx;
//	while (1)
//	{
//		if(psocket < 0)
//			return -1;
//		ret = lwip_recv(psocket , buf, len,MSG_DONTWAIT);
//		osDelay(timeout);
//		if(ret == -1 && (get_socket_error(psocket) != EWOULDBLOCK))
//		{
//			return -1;
//		}
//		if (ret > 0)
//				break;
//		if(xTaskGetTickCount() > time + timeout)
//		{
//			return LWIP_ERR_SSL_TIMEOUT;
//		}
//	}
//	return ret;
//}


/*
 * Read at most 'len' characters, blocking for at most 'timeout' ms
 */
int lwip_net_recv_timeout( int socket, unsigned char *buf, size_t len,
                      uint32_t timeout )
{
    int ret;
    struct timeval tv;
    fd_set read_fds;

    if( socket < 0 )
        return( LWIP_ERR_NET_INVALID_CONTEXT );

    FD_ZERO( &read_fds );
    FD_SET( socket, &read_fds );

    tv.tv_sec  = timeout / 1000;
    tv.tv_usec = ( timeout % 1000 ) * 1000;

    ret = select( socket + 1, &read_fds, NULL, NULL, timeout == 0 ? NULL : &tv );

    /* Zero fds ready means we timed out */
    if( ret == 0 )
        return( LWIP_ERR_SSL_TIMEOUT );

    if( ret < 0 )
    {
#if ( defined(_WIN32) || defined(_WIN32_WCE) ) && !defined(EFIX64) && \
    !defined(EFI32)
        if( WSAGetLastError() == WSAEINTR )
            return( LWIP_ERR_SSL_WANT_READ );
#else
        if( errno == EINTR )
            return( LWIP_ERR_SSL_WANT_READ );
#endif

        return( LWIP_ERR_NET_RECV_FAILED );
    }

    /* This call will not block */
    return( lwip_net_recv( socket, buf, len ) );
}

void lwip_net_free(int *socket)
{
	if( *socket == -1 )
			return;
	shutdown( *socket, 2  );
	lwip_close(*socket);
	*socket = -1;
}
