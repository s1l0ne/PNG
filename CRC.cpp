#include "CRC.h"

bool CRC::table_computed = 0;
uint8_t CRC::table[256] = { 0 };

void CRC::make_table() {
    if (table_computed) return;

    uint32_t c;
    for (uint32_t n = 0; n < 256; n++) {
        c = n;
        for (uint32_t k = 0; k < 8; k++) {
            if (c & 1)
                c = 0xedb88320 ^ (c >> 1);
            else
                c = c >> 1;
        }
        table[n] = c;
    }
    
    table_computed = 1;
}

uint32_t CRC::update_crc(uint32_t crc, uint8_t* buff, uint32_t len) {
    for (uint32_t n = 0; n < len; n++) {
        crc = table[(crc ^ buff[n]) & 0xff] ^ (crc >> 8);
    }
    return crc;
}

uint32_t CRC::get_crc(uint8_t* buff, uint32_t len) {
    return update_crc(0xffffffffL, buff, len) ^ 0xffffffffL;
}