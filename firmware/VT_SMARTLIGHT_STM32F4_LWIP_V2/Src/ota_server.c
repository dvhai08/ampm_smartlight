#include "main.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "uart1.h"
#include "uart5.h"
#include "lib/sys_time.h"
#include "system_config.h"
#include "ampm_gsm_main_task.h"
#include "hw_config.h"
#include "lib/sys_time.h"
#include "lwip/netdb.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "lib/sys_tick.h"
#include "ota_server.h"
#include "lwip/lwip_net_socket.h"
#include "sha256.h"
#include "base64.h"
#include "sst25.h"
/* Platform specific implementation header files */
#include "network_interface.h"
#include "timer_interface.h"

#define OTA_DBG(...) printf(__VA_ARGS__)

char *http_get = "GET /iot/download/sml_v1.0 HTTP/1.1\r\n\
HOST: platform.viettel.io\r\n\
auth: b430afd0ea1211e79480000c2960b9d4\r\n\r\n";

char *body ="\
{\
\"thingId\":\"8800127\",\
\"checkinLocale\":\"en\",\
\"buildNumber\":\"0\",\
\"deviceModel\":\"deviceModel\",\
\"productID\":\"sml\"\
}";

#define HTTPS	1

int total_bytes_recv = 0;
osThreadId ota_thread_id;
int ota_socket = -1;
//uint8_t hashOut[128];
//uint8_t ota_packet[600];
uint8_t *hashOut = NULL;
uint8_t *http_request = NULL;
uint8_t *http_response = NULL;
uint8_t *update_token = NULL;
uint8_t *update_url = NULL;
uint8_t *update_version = NULL;
uint8_t *update_encryptedfwkey = NULL;
uint8_t *update_checksum = NULL;
uint32_t update_size_in_bytes = 0;
uint8_t tryDownloadCnt = 0;
uint8_t flag_got_header = 0;
uint32_t firmware_file_offset = 0;



int checkAuthorizationString(const unsigned char *body, const unsigned char *deviceSecret,uint8_t *authorizationString){
	
	uint8_t buf[32];
	uint32_t len;
	memset(buf,0,32);
	mbedtls_sha256_context ctx;
	mbedtls_sha256_init( &ctx );
	mbedtls_sha256_starts( &ctx, 0 );//sha256
	
	mbedtls_sha256_context ctx1;
	mbedtls_sha256_init( &ctx1 );
	mbedtls_sha256_starts( &ctx1, 0 );//sha256
	
	mbedtls_sha256_update( &ctx, body, strlen((char *)body) );
	mbedtls_sha256_finish( &ctx, authorizationString );
	//printf("body_hash%s\r\n",authorizationString);
	
	mbedtls_sha256_update( &ctx1, authorizationString, strlen((char *)authorizationString) );
	mbedtls_sha256_update( &ctx1, deviceSecret, strlen((char *)deviceSecret) );
	mbedtls_sha256_finish( &ctx1, buf );
	
	if( mbedtls_base64_encode( authorizationString, 128, &len, buf, 32) != 0)
	{
			return( 0 );
	}
	printf("body_device_key_hash: %s\r\n",authorizationString);
  return 1;
}

uint32_t createHttpCheckin(char *header,char *auth,char *body)
{
	uint32_t len;
	len = sprintf(header,"POST /iot/checkinfw HTTP/1.1\r\n");
	len += sprintf(&header[len],"Host: platform.viettel.io\r\n");
	len += sprintf(&header[len],"Content-Length: %d\r\n",strlen(body));
	len += sprintf(&header[len],"auth: %s\r\n",auth);
	len += sprintf(&header[len],"Content-Type: application/json\r\n\r\n");
	len += sprintf(&header[len],"%s",body);
	return len;
}
//https://platform.viettel.io/iot/download/VT_SML_HCM_V1.bin
uint32_t createHttpFirmwareDownload(char *header,char *link,char *auth,uint32_t offset)
{
	uint32_t len,i;
	if(strstr(link,"https://") || strstr(link,"http://"))
	{
		for(i = 10; i < strlen(link);)
		{
			if(link[i] == '/')
			{
				link[i] = 0;
				break;
			}
			else
				i++;
		}
		len = sprintf(header,"GET /%s HTTP/1.1\r\n",&link[i+1]);
		for(i = 0; i < strlen(link);)
		{
			if((link[i] == '/') && (link[i+1] == '/'))
			{
				break;
			}
			else
				i++;
		}
		len += sprintf(&header[len],"HOST: %s\r\n",&link[i + 2]);
	}
	else
		return 0;
	len += sprintf(&header[len],"auth: %s\r\n",auth);
	len += sprintf(&header[len],"range: bytes=%d\r\n\r\n",offset);
	return len;
}


static void vOTA_Task(void const *param)
{
	int recbytes;
	int str_len;
	uint32_t content_len;
	int tryCnt = 0;
	int ret,wantRead = 0;
	char *pt,*pt1;
	#if (HTTPS)
	size_t sentLen;
	Network networkStack;
	IoT_Error_t rc;
	TimerAWS timer;
	rc = iot_tls_init(&networkStack,NULL, NULL,
					  NULL, "platform.viettel.io", 443,
					  10000,true);
	init_timer(&timer);
	#endif
	while(tryCnt < 10)
	{
		tryCnt++;
		http_request = NULL;
		hashOut = NULL;
		http_response = NULL;
		update_token = NULL;
		update_url = NULL;
		update_version = NULL;
		update_encryptedfwkey = NULL;
		update_checksum = NULL;
		while(!Ampm_GsmGPRS_IsWorking())
		{
			osDelay(1000);
		}
		#if (HTTPS)
		if((rc = networkStack.connect(&networkStack,NULL)) !=0)
		{
			
			networkStack.destroy(&networkStack);
			osDelay(1000);
			continue;
		}
		#else
		if((ret = lwip_net_connect(&ota_socket,"platform.viettel.io","80",LWIP_NET_PROTO_TCP)) != 0)
		{
			OTA_DBG("OTA_DBG:C > lwip_net_connect fail\n");
			switch(ret)
			{
				case LWIP_ERR_NET_SOCKET_FAILED:
					break;
				case LWIP_ERR_NET_UNKNOWN_HOST:
					break;
				case LWIP_ERR_NET_CONNECT_FAILED:
				default:
					break;
			}
			lwip_net_free(&ota_socket);
			osDelay(60000);
			continue;
		}
		#endif
		hashOut = pvPortMalloc(128);
		http_request = pvPortMalloc(400);
		memset(hashOut,0,128);
		checkAuthorizationString((const unsigned char *)body,(const unsigned char *)"8ded5f5365657c7ed2c51b776bffb12331ee1bd7c42140d1bd63c0e47753b216",hashOut);
		str_len = createHttpCheckin((char *)http_request,(char *)hashOut,body);
		vPortFree(hashOut);  hashOut = NULL;
		OTA_DBG("OTA_DBG:C > lwip_net_connect ok\n");
		#if (HTTPS)
		countdown_ms(&timer, 5000);
		rc = networkStack.write(&networkStack, (unsigned char *)http_request, strlen((char *)http_request),&timer,&sentLen);
		if(rc != 0)
		{
			networkStack.disconnect(&networkStack);
			networkStack.destroy(&networkStack);
			continue;
		}
		#else
		if (lwip_net_send(ota_socket, (const unsigned char *)http_request, strlen((char *)http_request)) < 0) {
				lwip_net_free(&ota_socket);
				vPortFree(http_request); http_request = NULL;
				OTA_DBG("OTA_DBG:C > send fail\n");
				osDelay(3000);
				continue;
		}
		#endif
		str_len = 0;
		vPortFree(http_request); http_request = NULL;
		http_response = pvPortMalloc(600);
		while(1)
		{
			#if (HTTPS)
			countdown_ms(&timer, 10000);
			if(str_len == 0)
				wantRead = 600;
			else
			{
				if(content_len >= str_len)
					wantRead = content_len - str_len;
				if(wantRead >= 600)
					wantRead = 600;
			}
			rc = networkStack.read(&networkStack, (unsigned char *)&http_response[str_len], wantRead,&timer,(size_t *)&recbytes);
			if(rc != 0)
			{
				//osDelay(3000);
				break;
			}
			#else
			recbytes = lwip_net_recv_timeout(ota_socket , (unsigned char *)&http_response[str_len], 600,5000);
			if(recbytes <= 0 && recbytes != LWIP_ERR_SSL_TIMEOUT)
			{
				//lwip_net_free(&ota_socket);
				OTA_DBG("OTA_DBG:C > recv fail\n");
				osDelay(3000);
				break;
			}
			#endif
			else
			{
				http_response[str_len + recbytes] = 0;
				OTA_DBG("OTA_DBG:C >Got %s\n\r",&http_response[str_len]);
				if(str_len == 0)
				{
					http_response[recbytes] = 0;
					//pt = strstr((const char *)http_response,"platform.viettel.io");
					//if(pt)
					{
						pt = strstr((const char *)http_response,"Content-Length:");
						if(pt)
						{
							sscanf(pt,"Content-Length: %d",&content_len);
							if(content_len < 4096)
							{
								pt = strstr((const char *)pt,"\r\n\r\n{\"");
								if(pt && (((uint8_t *)pt - http_response) < recbytes))
								{
									pt += 4; 
									str_len = recbytes - ((uint8_t *)pt - http_response);
									pt1 = pvPortMalloc(content_len + 1);
									http_response[content_len] = 0;
									memcpy(pt1,pt,str_len);
									vPortFree(http_response); http_response = NULL;
									http_response = (uint8_t *)pt1;
								}
							}
						}
					}
				}
				else
				{
					str_len += recbytes;
					if(str_len >= content_len)
					{
						pt1 = pvPortMalloc(256);
						http_response[content_len] = 0;
						OTA_DBG("\r\nRecv%d#%s",content_len,http_response);
						pt = strstr((const char *)http_response,"\"update_token\":\"");
						if(pt)
						{
							update_token = (uint8_t *)pt1;
							sscanf(pt,"\r\n\"update_token\":\"%[^\"]",update_token);
							update_token = pvPortMalloc(strlen((char *)update_token) + 1);
							strcpy((char *)update_token,pt1);
							OTA_DBG("\"update_token\":%s\"\r\n",update_token);
						}
						pt = strstr((char *)http_response,"\"update_version\":\"");
						if(pt)
						{
							update_version = (uint8_t *)pt1;
							sscanf(pt,"\"update_version\":\"%[^\"]",update_version);
							update_version = pvPortMalloc(strlen((char *)update_version) + 1);
							strcpy((char *)update_version,pt1);
							OTA_DBG("\"update_version\":%s\"\r\n",update_version);
						}
						pt = strstr((char *)http_response,"\"update_url\":\"");
						if(pt)
						{
							update_url = (uint8_t *)pt1;
							sscanf(pt,"\"update_url\":\"%[^\"]",update_url);
							update_url = pvPortMalloc(strlen((char *)update_url) + 1);
							strcpy((char *)update_url,pt1);
							OTA_DBG("\"update_url\":%s\"\r\n",update_url);
						}
						pt = strstr((char *)http_response,"\"update_encryptedfwkey\":\"");
						if(pt)
						{
							update_encryptedfwkey = (uint8_t *)pt1;
							sscanf(pt,"\"update_encryptedfwkey\":\"%[^\"]",update_encryptedfwkey);
							update_encryptedfwkey = pvPortMalloc(strlen((char *)update_encryptedfwkey) + 1);
							strcpy((char *)update_encryptedfwkey,pt1);
							OTA_DBG("\"update_encryptedfwkey\":%s\"\r\n",update_encryptedfwkey);
						}
						pt = strstr((char *)http_response,"\"update_checksum\":\"");
						if(pt)
						{
							update_checksum = (uint8_t *)pt1;
							sscanf(pt,"\"update_checksum\":\"%[^\"]",update_checksum);
							update_checksum = pvPortMalloc(strlen((char *)update_checksum) + 1);
							strcpy((char *)update_checksum,pt1);
							OTA_DBG("\"update_checksum\":%s\"\r\n",update_checksum);
							
						}
						pt = strstr((char *)http_response,"\"update_size_in_bytes\":\"");
						if(pt)
						{
							sscanf(pt,"\"update_size_in_bytes\":\"%d",&update_size_in_bytes);
							OTA_DBG("\"update_size_in_bytes\":%d\"\r\n",update_size_in_bytes);
						}
						uint8_t *u8pt;
						uint32_t crc;
						uint32_t status = 0xA5A5A5A5;
						u8pt = (uint8_t *)&status;
						OS_SST25_Read(FIRMWARE_INFO_ADDR+FIRMWARE_STATUS_OFFSET*4,u8pt,4);
						
						uint32_t fileSize = 0;
						u8pt = (uint8_t *)&fileSize;
						OS_SST25_Read(FIRMWARE_INFO_ADDR+FIRMWARE_FILE_SIZE_OFFSET*4,u8pt,4);
						
						if(status != 0xA5A5A5A5 || fileSize != update_size_in_bytes)
						{
							OS_SST25_Erase(FIRMWARE_INFO_ADDR,block4k);
							u8pt = (uint8_t *)&update_size_in_bytes;
							OS_SST25_Write(FIRMWARE_INFO_ADDR+FIRMWARE_FILE_SIZE_OFFSET*4,u8pt,4);
							uint32_t status = 0xA5A5A5A5;
							OS_SST25_Write(FIRMWARE_INFO_ADDR+FIRMWARE_STATUS_OFFSET*4,u8pt,4);
							//Write CRC
							u8pt = (uint8_t *)&crc;
							OS_SST25_Write(FIRMWARE_INFO_ADDR+FIRMWARE_CRC_OFFSET*4,u8pt,4);
							firmware_file_offset = 0;
						}
						else
						{
							firmware_file_offset = (firmware_file_offset / SST25_SECTOR_SIZE)*SST25_SECTOR_SIZE;
						}
						
						vPortFree(pt1); pt1 = NULL;
						vPortFree(http_response); http_response = NULL;
						#if (HTTPS)
						networkStack.disconnect(&networkStack);
						networkStack.destroy(&networkStack);
						#else
						lwip_net_free(&ota_socket);
						#endif
						tryDownloadCnt = 0;
						flag_got_header = 0;
						while(tryDownloadCnt < 3)
						{
							tryDownloadCnt++;
							//download file
							http_request = pvPortMalloc(400);
							str_len = createHttpFirmwareDownload((char *)http_request,(char *)update_url,(char *)update_token,firmware_file_offset);
							
							if(str_len)
							{
								OTA_DBG("start download %d bytes\r\n",update_size_in_bytes);
								if((ret = lwip_net_connect(&ota_socket,"platform.viettel.io","80",LWIP_NET_PROTO_TCP)) != 0)
								{
									OTA_DBG("OTA_DBG:C > lwip_net_connect fail\n");
									switch(ret)
									{
										case LWIP_ERR_NET_SOCKET_FAILED:
											break;
										case LWIP_ERR_NET_UNKNOWN_HOST:
											break;
										case LWIP_ERR_NET_CONNECT_FAILED:
										default:
											break;
									}
									lwip_net_free(&ota_socket);
									osDelay(5000);
									continue;
								}
								OTA_DBG("OTA_DBG:C > lwip_net_connect ok\n");
								OTA_DBG("OTA_DBG:Send %s\r\n",http_request);
								if (lwip_net_send(ota_socket, (const unsigned char *)http_request, strlen((char *)http_request)) < 0) {
										lwip_net_free(&ota_socket);
										vPortFree(http_request); http_request = NULL;
										OTA_DBG("OTA_DBG:C > send fail\n");
										osDelay(3000);
										continue;
								}
								str_len = 0;
								vPortFree(http_request); http_request = NULL;
								http_response = pvPortMalloc(600);
								while(1)
								{
									recbytes = lwip_net_recv_timeout(ota_socket , (unsigned char *)http_response, 600,5000);
									if(recbytes <= 0 && recbytes != LWIP_ERR_SSL_TIMEOUT)
									{
										//lwip_net_free(&ota_socket);
										OTA_DBG("OTA_DBG:C > recv fail\n");
										osDelay(3000);
										break;
									}
									else
									{
										if(flag_got_header == 0)
										{
											pt = strstr((const char *)http_response,"FOTA-Length:");
											if(pt)
											{
												sscanf(pt,"FOTA-Length: %d",&content_len);
												if(content_len == update_size_in_bytes)
												{
													flag_got_header = 1;
													str_len = 0;
												}
												else
												{
													break;
												}
											}
										}
										else
										{
											OTA_DBG("got %d bytes total %d bytes of %d bytes\r\n",recbytes,firmware_file_offset,update_size_in_bytes);
											firmware_file_offset += recbytes;
											OS_SST25_Write(FIRMWARE_BASE_ADDR + firmware_file_offset,http_response,recbytes);
											if(firmware_file_offset >= update_size_in_bytes)
											{
												
												ResetMcuSet(MCU_RESET_AFTER_10_SEC);
												
												OS_SST25_Erase(FIRMWARE_INFO_ADDR,block4k);
												
												//Write file size
												u8pt = (uint8_t *)&update_size_in_bytes;
												OS_SST25_Write(FIRMWARE_INFO_ADDR+FIRMWARE_FILE_SIZE_OFFSET*4,u8pt,4);
												uint32_t fwCrc = 0;
												//Write CRC
												u8pt = (uint8_t *)&fwCrc; 
												OS_SST25_Write(FIRMWARE_INFO_ADDR+FIRMWARE_CRC_OFFSET*4,u8pt,4);
												//Write Start addr
												uint32_t firmwareBaseAddr = FIRMWARE_BASE_ADDR;
												u8pt = (uint8_t *)&firmwareBaseAddr;
												OS_SST25_Write(FIRMWARE_INFO_ADDR+FIRMWARE_START_ADDR_OFFSET*4,u8pt,4);
												//Write status
												uint32_t firmwareStatus = 0x5A5A5A5A;
												u8pt = (uint8_t *)&firmwareStatus;
												OS_SST25_Write(FIRMWARE_INFO_ADDR+FIRMWARE_STATUS_OFFSET*4,u8pt,4);
												
												tryDownloadCnt = 3;
												tryCnt = 10;
												break;
											}
										}
									}
								}
								lwip_net_free(&ota_socket);
							}
							else
							{
								break;
							}
						}
						
						OTA_DBG("Free mem\r\n");
						if(http_request)
							vPortFree(http_request);
						if(http_response)
							vPortFree(http_response);
						if(update_token)
							vPortFree(update_token);
						if(update_url)
							vPortFree(update_url);
						if(update_version)
							vPortFree(update_version);
						if(update_encryptedfwkey)
							vPortFree(update_encryptedfwkey);
						if(update_checksum)
							vPortFree(update_checksum);
						http_request = NULL;
						hashOut = NULL;
						http_response = NULL;
						update_token = NULL;
						update_url = NULL;
						update_version = NULL;
						update_encryptedfwkey = NULL;
						update_checksum = NULL;
						break;
					}
				}
			}
		}
		#if (HTTPS)
		networkStack.disconnect(&networkStack);
		networkStack.destroy(&networkStack);
		#else
		lwip_net_free(&ota_socket);
		#endif
	}
  ota_thread_id = NULL;
	vTaskDelete( NULL );
	
}


void ota_stop(void)
{
	OTA_DBG("OTA_DBG:C > ota_stop\n");
	if(ota_thread_id)
	{
		if(hashOut)
			vPortFree(hashOut);
		if(http_request)
			vPortFree(http_request);
		if(http_response)
			vPortFree(http_response);
		if(update_token)
			vPortFree(update_token);
		if(update_url)
			vPortFree(update_url);
		if(update_version)
			vPortFree(update_version);
		if(update_encryptedfwkey)
			vPortFree(update_encryptedfwkey);
		if(update_checksum)
			vPortFree(update_checksum);

		OTA_DBG("OTA_DBG:C > ota kill task\n");
		osThreadTerminate(ota_thread_id);
		if(ota_socket > 0)
		{
			OTA_DBG("OTA_DBG:C > W kill socket %d\n",ota_socket);
			lwip_net_free(&ota_socket);
		}
	}
}

void ota_start(void)
{
	ota_stop();
	osThreadDef(ota_task, vOTA_Task, osPriorityNormal, 0, 5*1024);
	ota_thread_id = osThreadCreate(osThread(ota_task), NULL);
}


