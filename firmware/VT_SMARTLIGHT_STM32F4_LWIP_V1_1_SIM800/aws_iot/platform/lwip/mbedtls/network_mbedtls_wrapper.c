/*
 * Copyright 2010-2015 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *  http://aws.amazon.com/apache2.0
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <string.h>
#include <timer_platform.h>
#include <network_interface.h>

#include "aws_iot_error.h"
#include "aws_iot_log.h"
#include "network_interface.h"
#include "network_platform.h"

#include "aws_iot_config.h"
#if USE_AWS_IOT
const char AWS_IOT_ROOT_CA [] = \
"-----BEGIN CERTIFICATE-----\n"\
"MIIE0zCCA7ugAwIBAgIQGNrRniZ96LtKIVjNzGs7SjANBgkqhkiG9w0BAQUFADCB\n"\
"yjELMAkGA1UEBhMCVVMxFzAVBgNVBAoTDlZlcmlTaWduLCBJbmMuMR8wHQYDVQQL\n"\
"ExZWZXJpU2lnbiBUcnVzdCBOZXR3b3JrMTowOAYDVQQLEzEoYykgMjAwNiBWZXJp\n"\
"U2lnbiwgSW5jLiAtIEZvciBhdXRob3JpemVkIHVzZSBvbmx5MUUwQwYDVQQDEzxW\n"\
"ZXJpU2lnbiBDbGFzcyAzIFB1YmxpYyBQcmltYXJ5IENlcnRpZmljYXRpb24gQXV0\n"\
"aG9yaXR5IC0gRzUwHhcNMDYxMTA4MDAwMDAwWhcNMzYwNzE2MjM1OTU5WjCByjEL\n"\
"MAkGA1UEBhMCVVMxFzAVBgNVBAoTDlZlcmlTaWduLCBJbmMuMR8wHQYDVQQLExZW\n"\
"ZXJpU2lnbiBUcnVzdCBOZXR3b3JrMTowOAYDVQQLEzEoYykgMjAwNiBWZXJpU2ln\n"\
"biwgSW5jLiAtIEZvciBhdXRob3JpemVkIHVzZSBvbmx5MUUwQwYDVQQDEzxWZXJp\n"\
"U2lnbiBDbGFzcyAzIFB1YmxpYyBQcmltYXJ5IENlcnRpZmljYXRpb24gQXV0aG9y\n"\
"aXR5IC0gRzUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQCvJAgIKXo1\n"\
"nmAMqudLO07cfLw8RRy7K+D+KQL5VwijZIUVJ/XxrcgxiV0i6CqqpkKzj/i5Vbex\n"\
"t0uz/o9+B1fs70PbZmIVYc9gDaTY3vjgw2IIPVQT60nKWVSFJuUrjxuf6/WhkcIz\n"\
"SdhDY2pSS9KP6HBRTdGJaXvHcPaz3BJ023tdS1bTlr8Vd6Gw9KIl8q8ckmcY5fQG\n"\
"BO+QueQA5N06tRn/Arr0PO7gi+s3i+z016zy9vA9r911kTMZHRxAy3QkGSGT2RT+\n"\
"rCpSx4/VBEnkjWNHiDxpg8v+R70rfk/Fla4OndTRQ8Bnc+MUCH7lP59zuDMKz10/\n"\
"NIeWiu5T6CUVAgMBAAGjgbIwga8wDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8E\n"\
"BAMCAQYwbQYIKwYBBQUHAQwEYTBfoV2gWzBZMFcwVRYJaW1hZ2UvZ2lmMCEwHzAH\n"\
"BgUrDgMCGgQUj+XTGoasjY5rw8+AatRIGCx7GS4wJRYjaHR0cDovL2xvZ28udmVy\n"\
"aXNpZ24uY29tL3ZzbG9nby5naWYwHQYDVR0OBBYEFH/TZafC3ey78DAJ80M5+gKv\n"\
"MzEzMA0GCSqGSIb3DQEBBQUAA4IBAQCTJEowX2LP2BqYLz3q3JktvXf2pXkiOOzE\n"\
"p6B4Eq1iDkVwZMXnl2YtmAl+X6/WzChl8gGqCBpH3vn5fJJaCGkgDdk+bW48DW7Y\n"\
"5gaRQBi5+MHt39tBquCWIMnNZBU4gcmU7qKEKQsTb47bDN0lAtukixlE0kF6BWlK\n"\
"WE9gyn6CagsCqiUXObXbf+eEZSqVir2G3l6BFoMtEMze/aiCKm0oHw0LxOXnGiYZ\n"\
"4fQRbxC1lfznQgUy286dUV4otp6F01vvpX1FQHKOtw5rDgb7MzVIcbidJ4vEZV8N\n"\
"hnacRHr2lVz2XTIIM6RUthg/aFzyQkqFOFSDX9HoLPKsEdao7WNq\n"\
"-----END CERTIFICATE-----\0";

const char AWS_IOT_CERTIFICATE [] = \
"-----BEGIN CERTIFICATE-----\n"\
"MIIDWjCCAkKgAwIBAgIVAOVxVIBiCr0971qltlrDyaTZP/cxMA0GCSqGSIb3DQEB\n"\
"CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t\n"\
"IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0xNzExMDIwNDU0\n"\
"MzlaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh\n"\
"dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQCNpzn3fDPV2Cw0v1xl\n"\
"m41TRQMC+w0HnL+fsM5kiBCWN1KOfOQ79mhXYOpD84aHFsknUrmPfZO5WB8jB3dz\n"\
"/3qbJOhV44gRxn3Czw/7anmUZmmf9Kp7AUkOONQPJuUrmP3+zq0/NJxPNUIzpFEi\n"\
"ehpgwoQmKMtW2Q6F3bq5mi1pjhFJ7zLe1Z0JF9jHvTbwlPi2nBRkkjbl/ZJ18+65\n"\
"h/t8OVKqyfgPZ17xPnlZ0TnJ2WKTJRR4OIWpsT5Lb8KGZUCMITmOe2iDJdEiCHxR\n"\
"ctzccLoT1IIjQsNd0vhp6zNV+cnqfWKJLJ3rIWeIaqx9q2108vv0vJZEa5Vijdh5\n"\
"z/dHAgMBAAGjYDBeMB8GA1UdIwQYMBaAFDgC6SHUEbNqN+YU7t9VnFmAoe0LMB0G\n"\
"A1UdDgQWBBRzpMxCIs8u7a2sg7F22nunvI15KTAMBgNVHRMBAf8EAjAAMA4GA1Ud\n"\
"DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAkWyPspT4ICdzcUq7d0m+HTOH\n"\
"2mCdEheaU5S0hTfQ4kjCXbJZjUceeombM5lZXvm12r/o/4GhypgbgROJrcXgEECc\n"\
"3/IBz5uc4DcdSuQkxveFWeiiVR7Um+6UxP8Q/11JfPPWc+nJkmnz0tL3g8uckDUt\n"\
"jk1fIIzkQeswZICvLnipBve19druRU/IYEctSeTabrKn9mydAWP75WiFNWR60Cs0\n"\
"grOlOts5jEMEcX2+uDsEdheSM/xWgGlEGcIQ4lAvZNkRyfkr7Q7lmMMRodaH4Jj3\n"\
"Y485jT9BhAIsl6Ae+xxUEXLqlXs9g0AQB40nErMfs4qv0uLeoo3OwBFpn9SglQ==\n"\
"-----END CERTIFICATE-----\0";

const char AWS_IOT_PRIVATE_KEY [] = \
"-----BEGIN RSA PRIVATE KEY-----\n"\
"MIIEogIBAAKCAQEAjac593wz1dgsNL9cZZuNU0UDAvsNB5y/n7DOZIgQljdSjnzk\n"\
"O/ZoV2DqQ/OGhxbJJ1K5j32TuVgfIwd3c/96myToVeOIEcZ9ws8P+2p5lGZpn/Sq\n"\
"ewFJDjjUDyblK5j9/s6tPzScTzVCM6RRInoaYMKEJijLVtkOhd26uZotaY4RSe8y\n"\
"3tWdCRfYx7028JT4tpwUZJI25f2SdfPuuYf7fDlSqsn4D2de8T55WdE5ydlikyUU\n"\
"eDiFqbE+S2/ChmVAjCE5jntogyXRIgh8UXLc3HC6E9SCI0LDXdL4aeszVfnJ6n1i\n"\
"iSyd6yFniGqsfattdPL79LyWRGuVYo3Yec/3RwIDAQABAoIBAHXWJNyBz8UWvT9r\n"\
"6PsFeX1JrYWvPlmUxbucIzVyEwpDH0Q1FM/k3UvC7V51mPWLuO4jXQXG2zqV2eET\n"\
"9wTiBCZHf5bjXnAbBMcn5Q2qt1UCFN/hcmg8238fOxkXrBCmhrSPTYOEynsBvP6k\n"\
"tHHPzV2ssq3jxbOLgQ9PMmPvo9NoAcZiXiBJvxm1pmRx0+Mg0swdB0ndzPNOSy1E\n"\
"vd7Kw75rlLwHblfH6Ox71mGtueSPmhUKitbw4TBy/86xdJK7jrajnNiJYT3T+JRJ\n"\
"ZeGExAJ0XrAbt+Y7prpnagPbw4ym6nMSa4bTT5s45xOYFEXFe0t3L7CH55TKI85n\n"\
"sWSYWfECgYEA2Z7eUajqiD8/tXfXQVr4UgX0E7v3rJcLiIWWyvjdp7UmkqX24gxR\n"\
"g/3hIA7MtNFfl25bwIBc3ko11NDSQMe35wSYMXix7JMNsGHvaix5tYXrjpp7XO4r\n"\
"wlq6EHvUajCit4z2rG1GMgvgoF9fsO8QgN7bTbWSW1Xpr4mgrWkmFwUCgYEApqKV\n"\
"t/Xl0yxgEYij2gtbnbeFrgnPEmc8M0z5BaVe99vGaVG/qa+fdGUQC08mPI1eiWQ7\n"\
"VKf8TDRYtanUSk56QPXrkYluffEj8L2IwKan7gHWEkGBTYssaWzkAv10UUDdNpaH\n"\
"I/uBVvutq3Z4Mh8rBUzMwC0d2hDJjScJeY6YDtsCgYAeGQLWeG43ecft6S00NMiz\n"\
"T296azzN+fX2Y8N/Pvwo/bzf+2Qxz1JW1v4T2Pgm6e42m2TFMvulIs5fKKJrozOk\n"\
"F9aUwQSam5wBEQpzvML4gBUcG/GD4MzcjH+/V+EA7skSacSUBGj48vuWMpqhde7q\n"\
"Y+ggz0P2reMsC/TVBv3loQKBgCUoYCZzaNpcLNf8Fj2LBtT2lZKnkUtTRfNSmL0P\n"\
"kUvH8EiP+CNIjFASMS+0aJxcEdalSS2pf/MmQRnRUwt20IAn1m3PrZnb6d3HBfWf\n"\
"tBa4xhmwDbtS64Zex8F67zMnOazwiBYYAVF6MdHoSwkyNg+qfoXkCaIU2DznlY9L\n"\
"otC9AoGAJO27t9IFX/goRLkzU2AC9jLrXEUdfFuxLxsUR01MoylVhdf1YwKRVuzC\n"\
"9+8+K5AE4Hc97lARlFCGkvqnW9YPasXykqu14bUDZ2d/LGWlXz1sQPFJDLWRfcbp\n"\
"JziHmWu+S2yXxHckTkr5wO2j1tvQdbwEcRxGZS/4DzaD8+VUppA=\n"\
"-----END RSA PRIVATE KEY-----\0";

#else
const char AWS_IOT_ROOT_CA [] = \
"-----BEGIN CERTIFICATE-----\n"\
"MIIDrzCCApegAwIBAgIQCDvgVpBCRrGhdWrJWZHHSjANBgkqhkiG9w0BAQUFADBh\n"\
"MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n"\
"d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD\n"\
"QTAeFw0wNjExMTAwMDAwMDBaFw0zMTExMTAwMDAwMDBaMGExCzAJBgNVBAYTAlVT\n"\
"MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\n"\
"b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IENBMIIBIjANBgkqhkiG\n"\
"9w0BAQEFAAOCAQ8AMIIBCgKCAQEA4jvhEXLeqKTTo1eqUKKPC3eQyaKl7hLOllsB\n"\
"CSDMAZOnTjC3U/dDxGkAV53ijSLdhwZAAIEJzs4bg7/fzTtxRuLWZscFs3YnFo97\n"\
"nh6Vfe63SKMI2tavegw5BmV/Sl0fvBf4q77uKNd0f3p4mVmFaG5cIzJLv07A6Fpt\n"\
"43C/dxC//AH2hdmoRBBYMql1GNXRor5H4idq9Joz+EkIYIvUX7Q6hL+hqkpMfT7P\n"\
"T19sdl6gSzeRntwi5m3OFBqOasv+zbMUZBfHWymeMr/y7vrTC0LUq7dBMtoM1O/4\n"\
"gdW7jVg/tRvoSSiicNoxBN33shbyTApOB6jtSj1etX+jkMOvJwIDAQABo2MwYTAO\n"\
"BgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUA95QNVbR\n"\
"TLtm8KPiGxvDl7I90VUwHwYDVR0jBBgwFoAUA95QNVbRTLtm8KPiGxvDl7I90VUw\n"\
"DQYJKoZIhvcNAQEFBQADggEBAMucN6pIExIK+t1EnE9SsPTfrgT1eXkIoyQY/Esr\n"\
"hMAtudXH/vTBH1jLuG2cenTnmCmrEbXjcKChzUyImZOMkXDiqw8cvpOp/2PV5Adg\n"\
"06O/nVsJ8dWO41P0jmP6P6fbtGbfYmbW0W5BjfIttep3Sp+dWOIrWcBAI+0tKIJF\n"\
"PnlUkiaY4IBIqDfv8NZ5YBberOgOzW6sRBc4L0na4UU+Krk2U886UAb3LujEV0ls\n"\
"YSEY1QSteDwsOoBrp+uvFRTp2InBuThs4pFsiv9kuXclVzDAGySj4dzp30d8tbQk\n"\
"CAUw7C29C79Fv1C5qfPrmAESrciIxpg0X40KPMbp1ZWVbd4=\n"\
"-----END CERTIFICATE-----\0";
#endif


/* This is the value used for ssl read timeout */
#define IOT_SSL_READ_TIMEOUT 1000

/*
 * This is a function to do further verification if needed on the cert received
 */

static int _iot_tls_verify_cert(void *data, mbedtls_x509_crt *crt, int depth, uint32_t *flags) {
	char buf[1024];
	((void) data);

	IOT_DEBUG("\nVerify requested for (Depth %d):\n", depth);
	mbedtls_x509_crt_info(buf, sizeof(buf) - 1, "", crt);
	IOT_DEBUG("%s", buf);

	if((*flags) == 0) {
		IOT_DEBUG("  This certificate has no flags\n");
	} else {
		IOT_DEBUG(buf, sizeof(buf), "  ! ", *flags);
		IOT_DEBUG("%s\n", buf);
	}

	return 0;
}
// static void my_debug( void *ctx, int level,
//                       const char *file, int line, const char *str )
// {
//     ((void) level);
//     fprintf( (FILE *) ctx, "%s:%04d: %s", file, line, str );
//     fflush(  (FILE *) ctx  );
// }

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

	pNetwork->connect = iot_tls_connect;
	pNetwork->read = iot_tls_read;
	pNetwork->write = iot_tls_write;
	pNetwork->disconnect = iot_tls_disconnect;
	pNetwork->isConnected = iot_tls_is_connected;
	pNetwork->destroy = iot_tls_destroy;

	pNetwork->tlsDataParams.flags = 0;

	return AWS_SUCCESS;
}

IoT_Error_t iot_tls_is_connected(Network *pNetwork) {
	/* Use this to add implementation which can check for physical layer disconnect */
	return NETWORK_PHYSICAL_LAYER_CONNECTED;
}

void iot_tls_dbg(void  *p_dbg ,int level,const char *file, int line,const char *str ) {
	//IOT_DEBUG("SSL_TLS:file=%s:line=%d:info=%s",file,line,str);
	//IOT_DEBUG("SSL_TLS:file=%s:line=%d",&file[strlen(file)-10],line);
}


IoT_Error_t iot_tls_connect(Network *pNetwork, TLSConnectParams *params) {
	int ret = 0;
	const char *pers = "aws_iot_tls_wrapper";
	TLSDataParams *tlsDataParams = NULL;
	char portBuffer[6];
	char vrfy_buf[512];
#ifdef IOT_DEBUG
	unsigned char buf[MBEDTLS_SSL_MAX_CONTENT_LEN + 1];
#endif

	if(NULL == pNetwork) {
		return NULL_VALUE_ERROR;
	}

	if(NULL != params) {
		_iot_tls_set_connect_params(pNetwork, params->pRootCALocation, params->pDeviceCertLocation,
									params->pDevicePrivateKeyLocation, params->pDestinationURL,
									params->DestinationPort, params->timeout_ms, params->ServerVerificationFlag);
	}

	tlsDataParams = &(pNetwork->tlsDataParams);
	
	mbedtls_net_init(&(tlsDataParams->server_fd));
	mbedtls_ssl_init(&(tlsDataParams->ssl));
	mbedtls_ssl_config_init(&(tlsDataParams->conf));
	mbedtls_ctr_drbg_init(&(tlsDataParams->ctr_drbg));
	mbedtls_x509_crt_init(&(tlsDataParams->cacert));
	#if USE_AWS_IOT
	mbedtls_x509_crt_init(&(tlsDataParams->clicert));
	mbedtls_pk_init(&(tlsDataParams->pkey));
	#endif
	mbedtls_ssl_conf_dbg(&(tlsDataParams->conf),iot_tls_dbg,NULL);
	IOT_DEBUG("\n  . Seeding the random number generator...");
	mbedtls_entropy_init(&(tlsDataParams->entropy));
	if((ret = mbedtls_ctr_drbg_seed(&(tlsDataParams->ctr_drbg), mbedtls_entropy_func, &(tlsDataParams->entropy),
									(const unsigned char *) pers, strlen(pers))) != 0) {
		IOT_ERROR(" failed\n  ! mbedtls_ctr_drbg_seed returned -0x%x\n", -ret);
		return NETWORK_MBEDTLS_ERR_CTR_DRBG_ENTROPY_SOURCE_FAILED;
	}

	IOT_DEBUG("  . Loading the CA root certificate ...");
	ret = mbedtls_x509_crt_parse(&(tlsDataParams->cacert), (const unsigned char *)AWS_IOT_ROOT_CA, strlen(AWS_IOT_ROOT_CA));
	if(ret < 0) {
		IOT_ERROR(" failed\n  !  mbedtls_x509_crt_parse returned -0x%x while parsing root cert\n\r", -ret);
		return NETWORK_X509_ROOT_CRT_PARSE_ERROR;
	}
	#if USE_AWS_IOT
	IOT_DEBUG(" ok (%d skipped)\n", ret);

	IOT_DEBUG("  . Loading the client cert. and key...");
	ret = mbedtls_x509_crt_parse(&(tlsDataParams->clicert), (const unsigned char *)AWS_IOT_CERTIFICATE, strlen(AWS_IOT_CERTIFICATE));
	if(ret != 0) {
		IOT_ERROR(" failed\n  !  mbedtls_x509_crt_parse returned -0x%x while parsing device cert\n\r", -ret);
		return NETWORK_X509_DEVICE_CRT_PARSE_ERROR;
	}
	IOT_DEBUG(" ok (%d skipped)\n", ret);
	ret = mbedtls_pk_parse_key(&(tlsDataParams->pkey), (const unsigned char *)AWS_IOT_PRIVATE_KEY, strlen(AWS_IOT_PRIVATE_KEY), (const unsigned char *)(PASSWORD),strlen(PASSWORD));
	if(ret != 0) {
		IOT_ERROR(" failed\n  !  mbedtls_pk_parse_key returned -0x%x while parsing private key\n\r", -ret);
		IOT_DEBUG(" path : %s ", pNetwork->tlsConnectParams.pDevicePrivateKeyLocation);
		return NETWORK_PK_PRIVATE_KEY_PARSE_ERROR;
	}
	#endif
	IOT_DEBUG(" ok\n");
	snprintf(portBuffer, 6, "%d", pNetwork->tlsConnectParams.DestinationPort);
	IOT_DEBUG("  . Connecting to %s/%s...", pNetwork->tlsConnectParams.pDestinationURL, portBuffer);
	if((ret = mbedtls_net_connect(&(tlsDataParams->server_fd), pNetwork->tlsConnectParams.pDestinationURL,
								  portBuffer, MBEDTLS_NET_PROTO_TCP)) != 0) {
		IOT_ERROR(" failed\n  ! mbedtls_net_connect returned -0x%x\n\r", -ret);
		switch(ret) {
			case MBEDTLS_ERR_NET_SOCKET_FAILED:
				return NETWORK_ERR_NET_SOCKET_FAILED;
			case MBEDTLS_ERR_NET_UNKNOWN_HOST:
				return NETWORK_ERR_NET_UNKNOWN_HOST;
			case MBEDTLS_ERR_NET_CONNECT_FAILED:
			default:
				return NETWORK_ERR_NET_CONNECT_FAILED;
		};
	}
	#if USE_AWS_IOT
	ret = mbedtls_net_set_block(&(tlsDataParams->server_fd));
	if(ret != 0) {
		IOT_ERROR(" failed\n  ! net_set_(non)block() returned -0x%x\n\r", -ret);
		return SSL_CONNECTION_ERROR;
	} IOT_DEBUG(" ok\n");
	#endif
	IOT_DEBUG("  . Setting up the SSL/TLS structure...");
	if((ret = mbedtls_ssl_config_defaults(&(tlsDataParams->conf), MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM,
										  MBEDTLS_SSL_PRESET_DEFAULT)) != 0) {
		IOT_ERROR(" failed\n  ! mbedtls_ssl_config_defaults returned -0x%x\n\r", -ret);
		return SSL_CONNECTION_ERROR;
	}

	mbedtls_ssl_conf_verify(&(tlsDataParams->conf), _iot_tls_verify_cert, NULL);
	if(pNetwork->tlsConnectParams.ServerVerificationFlag == true) {
		mbedtls_ssl_conf_authmode(&(tlsDataParams->conf), MBEDTLS_SSL_VERIFY_REQUIRED);
	} else {
		mbedtls_ssl_conf_authmode(&(tlsDataParams->conf), MBEDTLS_SSL_VERIFY_OPTIONAL);
	}
	mbedtls_ssl_conf_rng(&(tlsDataParams->conf), mbedtls_ctr_drbg_random, &(tlsDataParams->ctr_drbg));

	mbedtls_ssl_conf_ca_chain(&(tlsDataParams->conf), &(tlsDataParams->cacert), NULL);
	#if USE_AWS_IOT
	if((ret = mbedtls_ssl_conf_own_cert(&(tlsDataParams->conf), &(tlsDataParams->clicert), &(tlsDataParams->pkey))) !=
	   0) {
		IOT_ERROR(" failed\n  ! mbedtls_ssl_conf_own_cert returned %d\n\r", ret);
		return SSL_CONNECTION_ERROR;
	}
	#endif
	mbedtls_ssl_conf_read_timeout(&(tlsDataParams->conf), pNetwork->tlsConnectParams.timeout_ms);

	if((ret = mbedtls_ssl_setup(&(tlsDataParams->ssl), &(tlsDataParams->conf))) != 0) {
		IOT_ERROR(" failed\n  ! mbedtls_ssl_setup returned -0x%x\n\r", -ret);
		return SSL_CONNECTION_ERROR;
	}
	if((ret = mbedtls_ssl_set_hostname(&(tlsDataParams->ssl), pNetwork->tlsConnectParams.pDestinationURL)) != 0) {
		IOT_ERROR(" failed\n  ! mbedtls_ssl_set_hostname returned %d\n\r", ret);
		return SSL_CONNECTION_ERROR;
	}
	IOT_DEBUG("\n\rSSL state connect : %d ", tlsDataParams->ssl.state);
	mbedtls_ssl_set_bio(&(tlsDataParams->ssl), &(tlsDataParams->server_fd), mbedtls_net_send, NULL,
						mbedtls_net_recv_timeout);
	IOT_DEBUG(" ok\n");

	IOT_DEBUG("\n\rSSL state connect : %d ", tlsDataParams->ssl.state);
	IOT_DEBUG("  . Performing the SSL/TLS handshake...");
	while((ret = mbedtls_ssl_handshake(&(tlsDataParams->ssl))) != 0) {
		if(ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
			IOT_ERROR(" failed\n  ! mbedtls_ssl_handshake returned -0x%x\n", -ret);
			if(ret == MBEDTLS_ERR_X509_CERT_VERIFY_FAILED) {
				IOT_ERROR("    Unable to verify the server's certificate. "
							  "Either it is invalid,\n"
							  "    or you didn't set ca_file or ca_path "
							  "to an appropriate value.\n"
							  "    Alternatively, you may want to use "
							  "auth_mode=optional for testing purposes.\n");
			}
			return SSL_CONNECTION_ERROR;
		}
	}

	IOT_DEBUG(" ok\n    [ Protocol is %s ]\n    [ Ciphersuite is %s ]\n", mbedtls_ssl_get_version(&(tlsDataParams->ssl)),
		  mbedtls_ssl_get_ciphersuite(&(tlsDataParams->ssl)));
	if((ret = mbedtls_ssl_get_record_expansion(&(tlsDataParams->ssl))) >= 0) {
		IOT_DEBUG("    [ Record expansion is %d ]\n", ret);
	} else {
		IOT_DEBUG("    [ Record expansion is unknown (compression) ]\n");
	}

	IOT_DEBUG("  . Verifying peer X.509 certificate...");

	if(pNetwork->tlsConnectParams.ServerVerificationFlag == true) {
		if((tlsDataParams->flags = mbedtls_ssl_get_verify_result(&(tlsDataParams->ssl))) != 0) {
			IOT_ERROR(" failed\n");
			mbedtls_x509_crt_verify_info(vrfy_buf, sizeof(vrfy_buf), "  ! ", tlsDataParams->flags);
			IOT_ERROR("%s\n", vrfy_buf);
			ret = SSL_CONNECTION_ERROR;
		} else {
			IOT_DEBUG(" ok\n");
			ret = AWS_SUCCESS;
		}
	} else {
		IOT_DEBUG(" Server Verification skipped\n");
		ret = AWS_SUCCESS;
	}

#ifdef IOT_DEBUG
	if (mbedtls_ssl_get_peer_cert(&(tlsDataParams->ssl)) != NULL) {
		IOT_DEBUG("  . Peer certificate information    ...\n");
		mbedtls_x509_crt_info((char *) buf, sizeof(buf) - 1, "      ", mbedtls_ssl_get_peer_cert(&(tlsDataParams->ssl)));
		IOT_DEBUG("%s\n", buf);
	}
#endif

	mbedtls_ssl_conf_read_timeout(&(tlsDataParams->conf), IOT_SSL_READ_TIMEOUT);

	return (IoT_Error_t) ret;
}

IoT_Error_t iot_tls_write(Network *pNetwork, unsigned char *pMsg, size_t len, TimerAWS *timer, size_t *written_len) {
	size_t written_so_far;
	bool isErrorFlag = false;
	int frags, ret;
	TLSDataParams *tlsDataParams = &(pNetwork->tlsDataParams);

	for(written_so_far = 0, frags = 0;
		written_so_far < len && !has_timer_expired(timer); written_so_far += ret, frags++) {
		while(!has_timer_expired(timer) &&
			  (ret = mbedtls_ssl_write(&(tlsDataParams->ssl), pMsg + written_so_far, len - written_so_far)) <= 0) {
			if(ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
				IOT_ERROR(" failed\n  ! mbedtls_ssl_write returned -0x%x\n\r", -ret);
				/* All other negative return values indicate connection needs to be reset.
		 		* Will be caught in ping request so ignored here */
				isErrorFlag = true;
				break;
			}
		}
		if(isErrorFlag) {
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

IoT_Error_t iot_tls_read(Network *pNetwork, unsigned char *pMsg, size_t len, TimerAWS *timer, size_t *read_len) {
	mbedtls_ssl_context *ssl = &(pNetwork->tlsDataParams.ssl);
	size_t rxLen = 0;
	int ret;

	while (len > 0) {
		// This read will timeout after IOT_SSL_READ_TIMEOUT if there's no data to be read
		ret = mbedtls_ssl_read(ssl, pMsg, len);
		if (ret > 0) {
			rxLen += ret;
			pMsg += ret;
			len -= ret;
		} else if (ret == 0 || (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE && ret != MBEDTLS_ERR_SSL_TIMEOUT)) {
			return NETWORK_SSL_READ_ERROR;
		}

		// Evaluate timeout after the read to make sure read is done at least once
		if (has_timer_expired(timer)) {
			break;
		}
	}

	if (len == 0) {
		*read_len = rxLen;
		return AWS_SUCCESS;
	}

	if (rxLen == 0) {
		return NETWORK_SSL_NOTHING_TO_READ;
	} else {
		return NETWORK_SSL_READ_TIMEOUT_ERROR;
	}
}

IoT_Error_t iot_tls_disconnect(Network *pNetwork) {
	mbedtls_ssl_context *ssl = &(pNetwork->tlsDataParams.ssl);
	int ret = 0;
	do {
		ret = mbedtls_ssl_close_notify(ssl);
	} while(ret == MBEDTLS_ERR_SSL_WANT_WRITE);

	/* All other negative return values indicate connection needs to be reset.
	 * No further action required since this is disconnect call */

	return AWS_SUCCESS;
}

IoT_Error_t iot_tls_destroy(Network *pNetwork) {
	TLSDataParams *tlsDataParams = &(pNetwork->tlsDataParams);
	printf("iot_tls_destroy\r\n");
	mbedtls_net_free(&(tlsDataParams->server_fd));
	#if USE_AWS_IOT
	mbedtls_x509_crt_free(&(tlsDataParams->clicert));
	#endif
	mbedtls_x509_crt_free(&(tlsDataParams->cacert));
	#if USE_AWS_IOT
	mbedtls_pk_free(&(tlsDataParams->pkey));
	#endif
	mbedtls_ssl_free(&(tlsDataParams->ssl));
	mbedtls_ssl_config_free(&(tlsDataParams->conf));
	mbedtls_ctr_drbg_free(&(tlsDataParams->ctr_drbg));
	mbedtls_entropy_free(&(tlsDataParams->entropy));

	return AWS_SUCCESS;
}

#ifdef __cplusplus
}
#endif
