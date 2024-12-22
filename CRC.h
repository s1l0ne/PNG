#pragma once
#include <cstdint>

class CRC {
	static bool table_computed;
	static uint8_t table[256];
	static uint32_t update_crc(uint32_t crc, uint8_t* buff, uint32_t len);

public:
	static void make_table();
	static uint32_t get_crc(uint8_t* buff, uint32_t len);
};