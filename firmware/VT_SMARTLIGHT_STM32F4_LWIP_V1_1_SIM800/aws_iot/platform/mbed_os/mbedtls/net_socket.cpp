#include "net_sockets.h"
#include "network_interface.h"
#include "OnboardCellularInterface.h"

TCPSocket * tcp_socket;
static OnboardCellularInterface driver(true);



/**
 * connect to the network
 */
nsapi_error_t mbedtls_do_connect(void)
{
    int num_retries = 0;
    nsapi_error_t err = NSAPI_ERROR_OK;
		driver.disconnect();
    while (!driver.is_connected()) {
        err = driver.connect(MBED_CONF_APP_DEFAULT_PIN, MBED_CONF_APP_APN,
                              MBED_CONF_APP_USERNAME, MBED_CONF_APP_PASSWORD);
        if (err == NSAPI_ERROR_OK || num_retries > MBED_CONF_APP_MAX_RETRIES) {
            break;
        }
        num_retries++;
    }

    return err;
}

void mbedtls_net_init(mbedtls_net_context *ctx) {
	tcp_socket = new TCPSocket(&driver);
	return;
}

int mbedtls_net_connect(mbedtls_net_context *ctx, const char *host,
		const char *port, int proto) {
	    return tcp_socket->connect(host, atoi(port));
}

int mbedtls_net_set_block(mbedtls_net_context *ctx) {
	tcp_socket->set_blocking(false);
	return 0;
}

int mbedtls_net_recv(void *ctx, unsigned char *buf, size_t len) {
	return tcp_socket->recv(buf, len);
}

int mbedtls_net_send(void *ctx, const unsigned char *buf, size_t len) {
	int size = -1;

	    size = tcp_socket->send(buf, len);

	    if(NSAPI_ERROR_WOULD_BLOCK == size){
	        return len;
	    }else if(size < 0){
	        return -1;
	    }else{
	        return size;
	    }
}

int mbedtls_net_recv_timeout(void *ctx, unsigned char *buf, size_t len,
		uint32_t timeout) {

	    int recv = -1;
	    tcp_socket->set_timeout(timeout);
	    recv = tcp_socket->recv(buf, len);

	    if(NSAPI_ERROR_WOULD_BLOCK == recv ||
	       recv == 0){
	        return 0;
	    }else if(recv < 0){
	        return -1;
	    }else{
	        return recv;
	    }
}

void mbedtls_net_free(mbedtls_net_context *ctx) {
	return;
}
