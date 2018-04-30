/**
 *
 *
 */

/* Includes --------------------------------------------------------------------------------------*/
#include <stdbool.h>
#include <string.h>
#include <timer_platform.h>
#include <network_interface.h>

#include "aws_iot_error.h"
#include "aws_iot_log.h"
#include "network_interface.h"
#include "network_platform.h"

#include "aws_iot_config.h"
/* Private define --------------------------------------------------------------------------------*/
/* Private functions -----------------------------------------------------------------------------*/
IoT_Error_t iot_notls_connect(Network *pNetwork, TLSConnectParams *params);
IoT_Error_t iot_notls_write(Network *pNetwork, unsigned char *pMsg, size_t len, TimerAWS *timer, size_t *written_len);
IoT_Error_t iot_notls_read(Network *pNetwork, unsigned char *pMsg, size_t len, TimerAWS *timer, size_t *read_len);
IoT_Error_t iot_notls_disconnect(Network *pNetwork);
IoT_Error_t iot_notls_destroy(Network *pNetwork);
IoT_Error_t iot_notls_is_connected(Network *pNetwork) ;

void _iot_tls_set_connect_params(Network *pNetwork, char *pRootCALocation, char *pDeviceCertLocation,
								 char *pDevicePrivateKeyLocation, char *pDestinationURL,
								 uint16_t destinationPort, uint32_t timeout_ms, bool ServerVerificationFlag) {
	pNetwork->tlsConnectParams.DestinationPort = destinationPort;
	pNetwork->tlsConnectParams.pDestinationURL = pDestinationURL;
	pNetwork->tlsConnectParams.pDeviceCertLocation = pDeviceCertLocation;
	pNetwork->tlsConnectParams.pDevicePrivateKeyLocation = pDevicePrivateKeyLocation;
	pNetwork->tlsConnectParams.pRootCALocation = pRootCALocation;
	pNetwork->tlsConnectParams.timeout_ms = timeout_ms;
	pNetwork->tlsConnectParams.ServerVerificationFlag = ServerVerificationFlag;
}
IoT_Error_t iot_tls_init(Network *pNetwork, char *pRootCALocation, char *pDeviceCertLocation,
						 char *pDevicePrivateKeyLocation, char *pDestinationURL,
						 uint16_t destinationPort, uint32_t timeout_ms, bool ServerVerificationFlag) {
	_iot_tls_set_connect_params(pNetwork, pRootCALocation, pDeviceCertLocation, pDevicePrivateKeyLocation,
								pDestinationURL, destinationPort, timeout_ms, ServerVerificationFlag);

	pNetwork->connect = iot_notls_connect;
	pNetwork->read = iot_notls_read;
	pNetwork->write = iot_notls_write;
	pNetwork->disconnect = iot_notls_disconnect;
	pNetwork->isConnected = iot_notls_is_connected;
	pNetwork->destroy = iot_notls_destroy;

	pNetwork->tlsDataParams.flags = 0;

	return AWS_SUCCESS;
}
IoT_Error_t iot_notls_is_connected(Network *pNetwork)
{
	/* Use this to add implementation which can check for physical layer disconnect */
	return NETWORK_PHYSICAL_LAYER_CONNECTED;
}
IoT_Error_t iot_notls_connect(Network *pNetwork, TLSConnectParams *params)
{
	int ret;
	char portBuffer[6];
	TLSDataParams *tlsDataParams = &(pNetwork->tlsDataParams);
	snprintf(portBuffer, 6, "%d", pNetwork->tlsConnectParams.DestinationPort);
	if((ret = mbedtls_net_connect(&(tlsDataParams->server_fd), pNetwork->tlsConnectParams.pDestinationURL,
							  portBuffer, MBEDTLS_NET_PROTO_TCP)) != 0)
	{
		IOT_ERROR(" failed\n  ! mbedtls_net_connect returned -0x%x\n\r", -ret);
		switch(ret)
		{
			case MBEDTLS_ERR_NET_SOCKET_FAILED:
				return NETWORK_ERR_NET_SOCKET_FAILED;
			case MBEDTLS_ERR_NET_UNKNOWN_HOST:
				return NETWORK_ERR_NET_UNKNOWN_HOST;
			case MBEDTLS_ERR_NET_CONNECT_FAILED:
			default:
				return NETWORK_ERR_NET_CONNECT_FAILED;
		}

	}
	return AWS_SUCCESS;

}
IoT_Error_t iot_notls_write(Network *pNetwork, unsigned char *pMsg, size_t len, TimerAWS *timer, size_t *written_len)
{
	size_t written_so_far;
	bool isErrorFlag = false;
	int ret;
	TLSDataParams *tlsDataParams = &(pNetwork->tlsDataParams);
	for(written_so_far = 0;
		written_so_far < len && !has_timer_expired(timer); written_so_far += ret)
	{

		ret = mbedtls_net_send(&(tlsDataParams->server_fd), pMsg + written_so_far, len - written_so_far);
		if(ret < 0)
		{
			IOT_ERROR(" failed\n  ! mbedtls_ssl_write returned -0x%x\n\r", -ret);
			/* All other negative return values indicate connection needs to be reset.
			* Will be caught in ping request so ignored here */
			isErrorFlag = true;
			break;
		}
	}

	*written_len = written_so_far;

	if(isErrorFlag) {
		return NETWORK_SSL_WRITE_ERROR;
	} else if(has_timer_expired(timer) && written_so_far != len) {
		return NETWORK_SSL_WRITE_TIMEOUT_ERROR;
	}

	return AWS_SUCCESS;
}
IoT_Error_t iot_notls_read(Network *pNetwork, unsigned char *pMsg, size_t len, TimerAWS *timer, size_t *read_len)
{
	size_t rxLen = 0;
	int ret;
	TLSDataParams *tlsDataParams = &(pNetwork->tlsDataParams);
	while (len > 0)
	{
		ret = mbedtls_net_recv(&(tlsDataParams->server_fd), pMsg, len);
		if (ret > 0) {
			rxLen += ret;
			pMsg += ret;
			len -= ret;
		} else if (!ret) {
			return NETWORK_SSL_READ_ERROR;
		}

		if (has_timer_expired(timer)) {
			break;
		}
	}

	if (len == 0)
	{
		*read_len = rxLen;
		return AWS_SUCCESS;
	}

	if (rxLen == 0)
		return NETWORK_SSL_NOTHING_TO_READ;
	else
		return NETWORK_SSL_READ_TIMEOUT_ERROR;
}
IoT_Error_t iot_notls_disconnect(Network *pNetwork)
{
	TLSDataParams *tlsDataParams = &(pNetwork->tlsDataParams);
	mbedtls_net_free(&(tlsDataParams->server_fd));
	return AWS_SUCCESS;
}
IoT_Error_t iot_notls_destroy(Network *pNetwork)
{
	TLSDataParams *tlsDataParams = &(pNetwork->tlsDataParams);
	mbedtls_net_free(&(tlsDataParams->server_fd));
	return AWS_SUCCESS;
}
