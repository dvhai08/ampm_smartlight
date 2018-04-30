#include "net_sockets.h"
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

void mbedtls_net_init(mbedtls_net_context *ctx) {

	return;
}

int mbedtls_net_connect(mbedtls_net_context *ctx, const char *host, const char *port, int proto)
{
	int ret;
	struct addrinfo hints, *addr_list, *cur;
	if( ( ret = net_prepare() ) != 0 )
        return( ret );
	/* Do name resolution with both IPv6 and IPv4 */
	memset( &hints, 0, sizeof( hints ) );
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = proto == MBEDTLS_NET_PROTO_UDP ? SOCK_DGRAM : SOCK_STREAM;
	hints.ai_protocol = proto == MBEDTLS_NET_PROTO_UDP ? IPPROTO_UDP : IPPROTO_TCP;
	if( getaddrinfo( host, port, &hints, &addr_list ) != 0 )
		return( MBEDTLS_ERR_NET_UNKNOWN_HOST );

	/* Try the sockaddrs until a connection succeeds */
	ret = MBEDTLS_ERR_NET_UNKNOWN_HOST;
	for( cur = addr_list; cur != NULL; cur = cur->ai_next )
	{
		ctx->fd = (int) socket( cur->ai_family, cur->ai_socktype,
							cur->ai_protocol );
		if( ctx->fd < 0 )
		{
			ret = MBEDTLS_ERR_NET_SOCKET_FAILED;
			continue;
		}
		if( connect( ctx->fd, cur->ai_addr, cur->ai_addrlen ) == 0 )
		{
			ret = 0;
			break;
		}
		close( ctx->fd );
		ret = MBEDTLS_ERR_NET_CONNECT_FAILED;
	}

	freeaddrinfo( addr_list );

	return( ret );
}

/*
 * Set the socket blocking or non-blocking
 */
int mbedtls_net_set_block( mbedtls_net_context *ctx )
{
#if ( defined(_WIN32) || defined(_WIN32_WCE) ) && !defined(EFIX64) && \
    !defined(EFI32)
    u_long n = 0;
    return( ioctlsocket( ctx->fd, FIONBIO, &n ) );
#else
    return( lwip_fcntl( ctx->fd, F_SETFL, lwip_fcntl( ctx->fd, F_GETFL ,0) & ~O_NONBLOCK ) );
#endif
}


int mbedtls_net_set_nonblock( mbedtls_net_context *ctx )
{
#if ( defined(_WIN32) || defined(_WIN32_WCE) ) && !defined(EFIX64) && \
    !defined(EFI32)
    u_long n = 1;
    return( ioctlsocket( ctx->fd, FIONBIO, &n ) );
#else
    return( lwip_fcntl( ctx->fd, F_SETFL, lwip_fcntl( ctx->fd, F_GETFL ,0) | O_NONBLOCK ) );
#endif
}

/*
 * Check if the requested operation would be blocking on a non-blocking socket
 * and thus 'failed' with a negative return value.
 *
 * Note: on a blocking socket this function always returns 0!
 */
static int net_would_block( const mbedtls_net_context *ctx )
{
    /*
     * Never return 'WOULD BLOCK' on a non-blocking socket
     */
    if( ( lwip_fcntl( ctx->fd, F_GETFL ,0) & O_NONBLOCK ) != O_NONBLOCK )
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

//int mbedtls_net_recv(void *ctx, unsigned char *buf, size_t len)
//{
//	int32_t ret;
//	mbedtls_net_context *pctx = (mbedtls_net_context *)ctx;
//	if(pctx->fd < 0)
//		return -1;
//	ret = lwip_recv(pctx->fd , buf, len,MSG_DONTWAIT);
//	if(ret == -1 && (get_socket_error(pctx->fd) != EWOULDBLOCK))
//	{
//		if( net_would_block( ctx ) != 0 )
//			return( MBEDTLS_ERR_SSL_WANT_READ );
//		return MBEDTLS_ERR_SSL_TIMEOUT;
//	}
//	return ret;
//}

/*
 * Read at most 'len' characters
 */
int mbedtls_net_recv( void *ctx, unsigned char *buf, size_t len )
{
    int ret;
    int fd = ((mbedtls_net_context *) ctx)->fd;

    if( fd < 0 )
        return( MBEDTLS_ERR_NET_INVALID_CONTEXT );

    ret = (int) read( fd, buf, len);

    if( ret < 0 )
    {
        if( net_would_block( ctx ) != 0 )
            return( MBEDTLS_ERR_SSL_WANT_READ );

#if ( defined(_WIN32) || defined(_WIN32_WCE) ) && !defined(EFIX64) && \
    !defined(EFI32)
        if( WSAGetLastError() == WSAECONNRESET )
            return( MBEDTLS_ERR_NET_CONN_RESET );
#else
        if( errno == EPIPE || errno == ECONNRESET )
            return( MBEDTLS_ERR_NET_CONN_RESET );

        if( errno == EINTR )
            return( MBEDTLS_ERR_SSL_WANT_READ );
#endif

        return( MBEDTLS_ERR_NET_RECV_FAILED );
    }

    return( ret );
}

//int mbedtls_net_send(void *ctx, const unsigned char *buf, size_t len) {
//	mbedtls_net_context *pctx = (mbedtls_net_context *)ctx;
//	if(pctx->fd < 0)
//		return -1;
//	return lwip_write(pctx->fd, buf, len);
//}

/*
 * Write at most 'len' characters
 */
int mbedtls_net_send( void *ctx, const unsigned char *buf, size_t len )
{
    int ret;
    int fd = ((mbedtls_net_context *) ctx)->fd;

    if( fd < 0 )
        return( MBEDTLS_ERR_NET_INVALID_CONTEXT );

    ret = (int) write( fd, buf, len );

    if( ret < 0 )
    {
        if( net_would_block( ctx ) != 0 )
            return( MBEDTLS_ERR_SSL_WANT_WRITE );

#if ( defined(_WIN32) || defined(_WIN32_WCE) ) && !defined(EFIX64) && \
    !defined(EFI32)
        if( WSAGetLastError() == WSAECONNRESET )
            return( MBEDTLS_ERR_NET_CONN_RESET );
#else
        if( errno == EPIPE || errno == ECONNRESET )
            return( MBEDTLS_ERR_NET_CONN_RESET );

        if( errno == EINTR )
            return( MBEDTLS_ERR_SSL_WANT_WRITE );
#endif

        return( MBEDTLS_ERR_NET_SEND_FAILED );
    }

    return( ret );
}

//int mbedtls_net_recv_timeout(void *ctx, unsigned char *buf, size_t len,
//	uint32_t timeout)
//{
//	
//	uint32_t time = HAL_GetTick();
//	int32_t ret;
//	mbedtls_net_context *pctx = (mbedtls_net_context *)ctx;
//	while (1)
//	{
//		if(pctx->fd < 0)
//			return -1;
//		ret = lwip_recv(pctx->fd , buf, len,MSG_DONTWAIT);
//		osDelay(timeout);
//		if(ret == -1 && (get_socket_error(pctx->fd) != EWOULDBLOCK))
//		{
//			return -1;
//		}
//		if (ret > 0)
//				break;
//		if(xTaskGetTickCount() > time + timeout)
//		{
//			return MBEDTLS_ERR_SSL_TIMEOUT;
//		}
//	}
//	return ret;
//}


/*
 * Read at most 'len' characters, blocking for at most 'timeout' ms
 */
int mbedtls_net_recv_timeout( void *ctx, unsigned char *buf, size_t len,
                      uint32_t timeout )
{
    int ret;
    struct timeval tv;
    fd_set read_fds;
    int fd = ((mbedtls_net_context *) ctx)->fd;

    if( fd < 0 )
        return( MBEDTLS_ERR_NET_INVALID_CONTEXT );

    FD_ZERO( &read_fds );
    FD_SET( fd, &read_fds );

    tv.tv_sec  = timeout / 1000;
    tv.tv_usec = ( timeout % 1000 ) * 1000;

    ret = select( fd + 1, &read_fds, NULL, NULL, timeout == 0 ? NULL : &tv );

    /* Zero fds ready means we timed out */
    if( ret == 0 )
        return( MBEDTLS_ERR_SSL_TIMEOUT );

    if( ret < 0 )
    {
#if ( defined(_WIN32) || defined(_WIN32_WCE) ) && !defined(EFIX64) && \
    !defined(EFI32)
        if( WSAGetLastError() == WSAEINTR )
            return( MBEDTLS_ERR_SSL_WANT_READ );
#else
        if( errno == EINTR )
            return( MBEDTLS_ERR_SSL_WANT_READ );
#endif

        return( MBEDTLS_ERR_NET_RECV_FAILED );
    }

    /* This call will not block */
    return( mbedtls_net_recv( ctx, buf, len ) );
}

void mbedtls_net_free(mbedtls_net_context *ctx)
{
	if( ctx->fd == -1 )
			return;
	mbedtls_net_context *pctx = (mbedtls_net_context *)ctx;
	shutdown( ctx->fd, 2 );
	lwip_close(pctx->fd);
	ctx->fd = -1;
}
