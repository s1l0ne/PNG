#pragma once
#include <iostream>
#include "CRC.h"


class PNG {
	FILE* png;

	struct RGB {
		uint8_t r;
		uint8_t g;
		uint8_t b;
	};

	struct RGBA : public RGB {
		uint8_t a;
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

	struct chunck {
		uint32_t len = 0;
		uint32_t* type = nullptr;
		uint8_t* data = nullptr;
	} curr_chunck;

	IHDR ihdr;
	RGB* plte;

	uint8_t** picture;

	uint32_t as_uint32(uint32_t uint) {

	}
	void read_bytes(uint32_t cnt, uint8_t* to);
	void get_chunck();

	void ihdr_chunck();
	void plte_chunck();
	void idat_chunck();
	void iend_chunck();

public:
	PNG(FILE* png);
	~PNG();
};