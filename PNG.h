#pragma once
#include <iostream>


class PNG {
	FILE* png;

	struct RGB {
		uint8_t r;
		uint8_t g;
		uint8_t b;
	};

	struct IHDR {
		uint32_t width;
		uint32_t height;
		uint8_t bit_depth;
		uint8_t color_type;
		uint8_t compression_method;
		uint8_t filter_method;
		uint8_t interlace;
	};

	IHDR ihdr;
	RGB* plte;

	uint8_t** picture;

	void get_chunck_info(uint8_t* len, uint8_t* type);
	void read_bytes(uint32_t cnt, uint8_t* to);

	void plte_chunck(const uint32_t len);
	void idat_chunck(const uint32_t len);
	void iend_chunck(const uint32_t len);

public:
	PNG(FILE* png);
};