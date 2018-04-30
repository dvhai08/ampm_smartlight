#include "net_sockets.h"
#include "network_interface.h"
#include "lwip/sockets.h"
#include <string.h>
#include "lwip/netdb.h"
#include "FreeRTOS.h"
#include "task.h"



void mbedtls_net_init(mbedtls_net_context *ctx) {

	return;
}

int mbedtls_net_connect(mbedtls_net_context *ctx, const char *host, const char *port, int proto)
{
	int ret;
	struct addrinfo hints, *addr_list, *cur;

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

int mbedtls_net_set_block(mbedtls_net_context *ctx) {

	return 0;
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

int mbedtls_net_recv(void *ctx, unsigned char *buf, size_t len)
{
	int32_t ret;
	mbedtls_net_context *pctx = (mbedtls_net_context *)ctx;
	if(pctx->fd < 0)
		return -1;
	ret = lwip_recv(pctx->fd , buf, len,MSG_DONTWAIT);
	if(ret == -1 && (get_socket_error(pctx->fd) != EWOULDBLOCK))
	{
		if( net_would_block( ctx ) != 0 )
			return( MBEDTLS_ERR_SSL_WANT_READ );
		return MBEDTLS_ERR_SSL_TIMEOUT;
	}
	return ret;
}

int mbedtls_net_send(void *ctx, const unsigned char *buf, size_t len) {
	mbedtls_net_context *pctx = (mbedtls_net_context *)ctx;
	if(pctx->fd < 0)
		return -1;
	return lwip_write(pctx->fd, buf, len);
}

int mbedtls_net_recv_timeout(void *ctx, unsigned char *buf, size_t len,
	uint32_t timeout)
{
	
	uint32_t time = HAL_GetTick();
	int32_t ret;
	mbedtls_net_context *pctx = (mbedtls_net_context *)ctx;
	while (1)
	{
		if(pctx->fd < 0)
			return -1;
		ret = lwip_recv(pctx->fd , buf, len,MSG_DONTWAIT);
		osDelay(timeout);
		if(ret == -1 && (get_socket_error(pctx->fd) != EWOULDBLOCK))
		{
			return -1;
		}
		if (ret > 0)
				break;
		if(xTaskGetTickCount() > time + timeout)
		{
			return MBEDTLS_ERR_SSL_TIMEOUT;
		}
	}
	return ret;
}

void mbedtls_net_free(mbedtls_net_context *ctx)
{
	mbedtls_net_context *pctx = (mbedtls_net_context *)ctx;
	lwip_close(pctx->fd);
}
