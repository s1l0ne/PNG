#include "Png.h"


inline void PNG::read_bytes(uint32_t cnt, uint8_t* to) {
	for (uint32_t i = 0; i < cnt; i++) {
		*(to + i) = fgetc(png);
	}
}

void PNG::get_chunck_info(uint8_t* len, uint8_t* type) {
	read_bytes(4, len);
	read_bytes(4, type);
}


void PNG::plte_chunck(const uint32_t len) {
	plte = new RGB[len / 3];
	for (uint32_t i = 0; i < len / 3; i++) {
		plte[i].r = fgetc(png);
		plte[i].g = fgetc(png);
		plte[i].b = fgetc(png);
	}


}


PNG::PNG(FILE* png) : png(png), plte(nullptr) {
	// Parse header
	uint8_t header[8];
	for (uint8_t i = 0; i < 8; i++) {
		if ((header[i] = fgetc(png)) == EOF) {
			throw "Wrong header";
		}
	}

	if (*reinterpret_cast<uint64_t*>(header) != 0x89'504e47'0d0a'1a'0a) {
		throw "Wrong header";
	}


	// Parse IHDR chunck
	uint32_t len = 0;
	uint32_t type;
	read_bytes(4, reinterpret_cast<uint8_t*>(len));

	for (uint8_t i = 0; i < 4; i++) {
		if ("IHDR"[i] != fgetc(png)) {
			throw "Wrong IHDR";
		}
	}

	read_bytes(4, reinterpret_cast<uint8_t*>(&ihdr.width));
	read_bytes(4, reinterpret_cast<uint8_t*>(&ihdr.height));
	ihdr.bit_depth = fgetc(png);
	ihdr.color_type = fgetc(png);
	if ((ihdr.compression_method = fgetc(png)) != 0) {
		throw "Wrong compression method";
	}
	if ((ihdr.filter_method = fgetc(png)) != 0) {
		throw "Wrong filter method";
	}
	ihdr.interlace = fgetc(png);


	// Parse other chuncks
	while (true) {
		get_chunck_info(reinterpret_cast<uint8_t*>(&len), reinterpret_cast<uint8_t*>(&type));

		if (type == *reinterpret_cast<uint32_t*>("PLTE")) {
			plte_chunck(len);
		}
		else if (type == *reinterpret_cast<uint32_t*>("IDAT")) {
			idat_chunck(len);
		}
		else if (type == *reinterpret_cast<uint32_t*>("IEND")) {
			iend_chunck;
		}
	}
}