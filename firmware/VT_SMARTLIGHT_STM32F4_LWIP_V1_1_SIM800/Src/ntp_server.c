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
#include "net_sockets.h"
#include "lwip/lwip_net_socket.h"
#include "alarm.h"
#define NTP_DBG(...) printf(__VA_ARGS__)
extern void RTC_SetTime(DATE_TIME time);
typedef struct  __attribute__((__packed__)){
	uint8_t flags;
	uint8_t stratum;
	uint8_t poll;
	uint8_t precision;
	uint32_t root_delay;
	uint32_t root_dispersion;
	uint8_t referenceID[4];
	uint32_t ref_ts_sec;
	uint32_t ref_ts_frac;
	uint32_t origin_ts_sec;
	uint32_t origin_ts_frac;
	uint32_t recv_ts_sec;
	uint32_t recv_ts_frac;
	uint32_t trans_ts_sec;
	uint32_t trans_ts_frac;
} ntpPacket;
ntpPacket ntp_packet;

void ntp_appcall(void);

osThreadId thread_id;
int ntp_socket = -1;
uint8_t rtcGetNewTimeFlag = 0;

#define UNIX_OFFSET 		2208988800L
#define NTP_VERSION 		0x1b /*0xe3*/
#define ENDIAN_SWAP32(data)  	((data >> 24) | /* right shift 3 bytes */ \
				((data & 0x00ff0000) >> 8) | /* right shift 1 byte */ \
			        ((data & 0x0000ff00) << 8) | /* left shift 1 byte */ \
				((data & 0x000000ff) << 24)) /* left shift 3 bytes */

void ntp_init(uint32_t priority)
{

}

void ntp_reset(void)
{

}

static void vNTP_Task(void const *param)
{
	int recbytes;
	int sin_size;
	int str_len;
	uint32_t cnt = 0;
	int exit = 0,tryCnt = 0;
	uint32_t interval = 20;
	uint16_t timeout = 0;
	int ret;
	struct sockaddr_in remote_ip;
	//thread_id = osThreadGetId();
	//"asia.pool.ntp.org"
    while (1) {
			//	while(tryCnt < 3)
				{
					while(!Ampm_GsmGPRS_IsWorking())
					{
						osDelay(1000);
					}
					if((ret = lwip_net_connect(&ntp_socket,"asia.pool.ntp.org","123",LWIP_NET_PROTO_UDP)) != 0)
					{
						NTP_DBG("NTP:lwip_net_connect fail\n");
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
						lwip_net_free(&ntp_socket);
						osDelay(10000);
						continue;
					}
					NTP_DBG("NTP:lwip_net_connect ok\n");
					memset(&ntp_packet,0,sizeof(ntpPacket));
					ntp_packet.flags = NTP_VERSION;//;
					ntp_packet.precision = 1;
					ntp_packet.trans_ts_sec = 1512707160+UNIX_OFFSET;
					if (lwip_net_send(ntp_socket, (const unsigned char *)&ntp_packet, sizeof(ntpPacket)) < 0) {
							lwip_net_free(&ntp_socket);
							NTP_DBG("NTP:send fail\n");
							osDelay(3000);
							continue;
					}
					exit = 0;
					while(!exit)
					{
						recbytes = lwip_net_recv_timeout(ntp_socket , (unsigned char *)&ntp_packet, sizeof(ntpPacket),5000);
						if(recbytes == sizeof(ntpPacket))
						{
							ntp_packet.ref_ts_sec = ENDIAN_SWAP32(ntp_packet.ref_ts_sec);
							ntp_packet.ref_ts_frac = ENDIAN_SWAP32(ntp_packet.ref_ts_frac);
							ntp_packet.origin_ts_sec = ENDIAN_SWAP32(ntp_packet.origin_ts_sec);
							ntp_packet.origin_ts_frac = ENDIAN_SWAP32(ntp_packet.origin_ts_frac);
							ntp_packet.recv_ts_sec = ENDIAN_SWAP32(ntp_packet.recv_ts_sec);
							ntp_packet.recv_ts_frac = ENDIAN_SWAP32(ntp_packet.recv_ts_frac);
							ntp_packet.trans_ts_sec = ENDIAN_SWAP32(ntp_packet.trans_ts_sec);
							ntp_packet.trans_ts_frac = ENDIAN_SWAP32(ntp_packet.trans_ts_frac);
							NTP_DBG("\n\rNTP: got data\n\r");
//							NTP_DBG("\n\rNTP: \"ntp_data\":{\n\r");
//							NTP_DBG("\"ref_ts_sec\":%u,%X,\n\r",ntp_packet.ref_ts_sec,ntp_packet.ref_ts_sec);
//							NTP_DBG("\"ref_ts_frac\":%u,%X,\n\r",ntp_packet.ref_ts_frac,ntp_packet.ref_ts_frac);
//							NTP_DBG("\"origin_ts_sec\":%u,%X,\n\r",ntp_packet.origin_ts_sec,ntp_packet.origin_ts_sec);
//							NTP_DBG("\"origin_ts_frac\":%u,%X,\n\r",ntp_packet.origin_ts_frac,ntp_packet.origin_ts_frac);
//							NTP_DBG("\"recv_ts_sec\":%u,%X,\n\r",ntp_packet.recv_ts_sec,ntp_packet.recv_ts_sec);
//							NTP_DBG("\"recv_ts_frac\":%u,%X,\n\r",ntp_packet.recv_ts_frac,ntp_packet.recv_ts_frac);
//							NTP_DBG("\"trans_ts_sec\":%u,%X,\n\r",ntp_packet.trans_ts_sec,ntp_packet.trans_ts_sec);
//							NTP_DBG("\"trans_ts_frac\":%u,%X\n\r}\n\r",ntp_packet.trans_ts_frac,ntp_packet.trans_ts_frac);
//							NTP_DBG("\n\rUpdateRtcTime(ntp_packet.recv_ts_sec-UNIX_OFFSET + sysTimeZone);\n\r");
							UpdateRtcTime(ntp_packet.recv_ts_sec-UNIX_OFFSET);
							TIME_FromSec(&sysTime,ntp_packet.recv_ts_sec - UNIX_OFFSET + TIME_ZONE*3600);
							NTP_DBG("\"time in sec\":%lld\n\r",ntp_packet.recv_ts_sec-UNIX_OFFSET);
							NTP_DBG("\"sysTime\":%04u-%02u-%02u %02u:%02u:%02u\n\r",sysTime.year,sysTime.month,sysTime.mday,sysTime.hour,sysTime.min,sysTime.sec);
							RTC_SetTime(sysTime);
							exit = 1;
							tryCnt = 3;
						}
						if(recbytes <= 0  && recbytes != LWIP_ERR_SSL_TIMEOUT)
						{
							exit = 1;
							tryCnt++;
						}
						else
						{
								memset(&ntp_packet,0,sizeof(ntpPacket));
								ntp_packet.flags = NTP_VERSION;//;
								ntp_packet.precision = 1;
								ntp_packet.trans_ts_sec = 1512707160+UNIX_OFFSET;
								if (lwip_net_send(ntp_socket, (const unsigned char *)&ntp_packet, sizeof(ntpPacket)) < 0) {
									exit = 1;
								}
						}
					}
					lwip_net_free(&ntp_socket);
				}
				timeout = 3600;//1hour
				while(timeout--)
				{
					osDelay(1000);
					if(rtcGetNewTimeFlag)
					{
						rtcGetNewTimeFlag = 0;
						break;
					}
				}
				tryCnt = 0;
    }
		
		osThreadTerminate(thread_id);
		thread_id = NULL;
}



void ntp_stop(void)
{
	NTP_DBG("NTP:ntp_stop\n");
	if(thread_id)
	{
		NTP_DBG("NTP:ntp kill task\n");
		osThreadTerminate(thread_id);
		if(ntp_socket > 0)
		{
			NTP_DBG("NTP:ntp kill socket %d\n",ntp_socket);
			lwip_net_free(&ntp_socket);
		}
	}
}

void ntp_get_time(void)
{
	rtcGetNewTimeFlag = 1;
}

void ntp_start(void)
{
	ntp_stop();
	NTP_DBG("NTP:ntp_start\n");
	osThreadDef(ntp_task, vNTP_Task, osPriorityNormal, 0, 256);
	thread_id = osThreadCreate(osThread(ntp_task), NULL);
}

int is_ntp_server_running(void)
{
	if(thread_id)
		return 1;
	return 0;
}
