//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
#ifndef __MESH_STRUCTURE_H__
#define __MESH_STRUCTURE_H__
#include <stdint.h>
#define LOCAL_NODE_ID           15280009

//#define PACKET_POLLING          1

#if defined(LOCAL_NODE_ID)  &&  LOCAL_NODE_ID == 1
#define MASTER_NODE             1
#endif

#if defined(PACKET_POLLING)
#define MAX_LINK_PACKET_SIZE    128
#define MAX_MESH_PACKET_SIZE    142
#else
#define MAX_LINK_PACKET_SIZE    64
#define MAX_MESH_PACKET_SIZE    88
#endif

#define SERIAL_MAX_LEN          152



#define MESH_PACKET_HEADER  15


#if defined (__ICCARM__) /* IAR   */
#pragma pack(1)
typedef struct
#elif defined (__CC_ARM)
typedef struct __attribute__((packed))
#endif
{
    /*! \brief The step into the history data used for phase correction. */
    uint8_t  packet_length;
    uint8_t  packet_sequence;
    uint32_t from_ID;
    uint32_t to_ID;
    int8_t rxRSSI;
    int8_t txRSSI;
    uint8_t command;
    uint8_t id_length;
    uint8_t body_length;
	#if defined (__ICCARM__) /* IAR   */
    union
	#elif defined (__CC_ARM)
		__packed union
	#endif
    {
        uint8_t  uint8[MAX_LINK_PACKET_SIZE];
        uint16_t uint16[MAX_LINK_PACKET_SIZE/2];
        uint32_t uint32[MAX_LINK_PACKET_SIZE/4];
    } body;  
} packet_fields;
#if defined (__ICCARM__) /* IAR   */
#pragma pack()
#endif

#if defined (__ICCARM__) /* IAR   */
#pragma pack(1)
typedef union
{
    packet_fields packet;
    uint8_t uint8[MAX_MESH_PACKET_SIZE];
    uint16_t uint16[MAX_MESH_PACKET_SIZE/2];
} mesh_packet_t;
#pragma pack()
#elif defined (__CC_ARM)
typedef union
{
    __packed packet_fields packet;
    __packed uint8_t uint8[MAX_MESH_PACKET_SIZE];
    __packed uint16_t uint16[MAX_MESH_PACKET_SIZE/2];
} mesh_packet_t;
#endif



#define ENGAGED                 BIT0
#define FIVE_MINUTE_TIME_OUT    BIT1
#define T30_MINUTES            180000
#define FIVE_MINUTES           30000
#define __infomem__ _Pragma("location=\"INFO\"")
#define RESPOND_TO_DISCOVERY  (1<<0)

extern const uint8_t lcd_chars[];

//#if defined(MESH_NET_SUPPORT)
extern uint32_t OWN_ID;
extern int delay_count;
extern uint32_t long_word_table[10];
extern uint32_t system_status;
extern uint8_t packet_status[];
extern uint16_t rndnum;
extern char message[];
extern mesh_packet_t comm_buffer;
extern volatile uint16_t mesh_host_command_timeout;
extern volatile uint32_t mesh_tick_cnt;
extern volatile uint8_t mesh_host_command_waiting_flag;
extern volatile uint32_t mesh_host_command_tick;
//#endif

extern const uint8_t lcd_chars[];

char *string_bin(char *s, uint32_t *l);

uint8_t *str_decu16(uint16_t value, uint8_t *s);
uint8_t *str_decu32(uint32_t value, uint8_t *s);

void decode(mesh_packet_t *comm_buffer_ptr);
uint8_t prepare_receive_packet(int timeout);
int delay(int delay);
uint8_t *decode_address(uint8_t *s, uint32_t *l);

void meshnet_init(void);
void mesh_rf_timer(void);
void rf_tick_service(void);
void meshnet_hardware_init(void);
void rf_service_service(void);
void RF_IntCallback(void);
void mesh_task_tick(void);
void mesh_task(mesh_packet_t *comm_buffer_ptr,uint8_t *rf_new_data_flg);
void meshnet_service(void);
void prepare_send_packet(mesh_packet_t *comm_buffer);
void update_random(int32_t u,int32_t i);

#endif
