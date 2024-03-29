
#include "crc16.h"
#include <stdlib.h>
uint16_t crc16_compute(uint8_t const * p_data, uint32_t size, uint16_t const * p_crc)
{		
		uint32_t i;
    uint16_t crc = (p_crc == NULL) ? 0xFFFF : *p_crc;

    for (i = 0; i < size; i++)
    {
        crc  = (uint8_t)(crc >> 8) | (crc << 8);
        crc ^= p_data[i];
        crc ^= (uint8_t)(crc & 0xFF) >> 4;
        crc ^= (crc << 8) << 4;
        crc ^= ((crc & 0xFF) << 4) << 1;
    }

    return crc;
}

