#include "PNG.h"


inline void PNG::read_bytes(uint32_t cnt, uint8_t* to) {
	for (uint32_t i = 0; i < cnt; i++) {
		*(to + i) = fgetc(png);
	}
}

inline void PNG::get_chunck() {
	if (curr_chunck.type != nullptr) {
		delete[] curr_chunck.type;
	}

	read_bytes(4, reinterpret_cast<uint8_t*>(&curr_chunck.len));

	curr_chunck.data = new uint8_t[curr_chunck.len];
	read_bytes(4 + curr_chunck.len, curr_chunck.data);
	curr_chunck.type = reinterpret_cast<uint32_t*>(curr_chunck.data);
	curr_chunck.data += 4;

	// example : IHDR chunck
	//	memory : I H D R _ _ _ _ _ _ _ _ _ _ 0 0 _
	//			 ^		 ^
	//			type    data
}


void PNG::ihdr_chunck() {
	if (*curr_chunck.type != *reinterpret_cast<uint32_t*>(const_cast<char*>("IHDR"))) {
		throw "Error: expected IHDR";
	}

	memcpy(reinterpret_cast<uint8_t*>(&ihdr.width), curr_chunck.data, 4);
	memcpy(reinterpret_cast<uint8_t*>(&ihdr.height), curr_chunck.data + 4, 4);

	ihdr.bit_depth = *(curr_chunck.data + 8);
	ihdr.color_type = *(curr_chunck.data + 9);
	if ((ihdr.compression_method = *(curr_chunck.data + 10)) != 0) {
		throw "Wrong compression method. Expected: 0";
	}
	if ((ihdr.filter_method = *(curr_chunck.data + 11)) != 0) {
		throw "Wrong filter method. Expected: 0";
	}
	ihdr.interlace = *(curr_chunck.data + 12);
}


void PNG::plte_chunck() {
	plte = new RGB[curr_chunck.len / 3];
	for (uint32_t i = 0; i < curr_chunck.len / 3; i++) {
		plte[i].r = curr_chunck.data[i * 3];
		plte[i].g = curr_chunck.data[i * 3 + 1];
		plte[i].b = curr_chunck.data[i * 3 + 2];
	}
}


void PNG::idat_chunck() {
	
}


void PNG::iend_chunck() {

}



PNG::PNG(FILE* png) : png(png), plte(nullptr) {
	CRC::make_table();
	// Parse header
	uint8_t header[8];
	for (uint8_t i = 0; i < 8; i++) {
		if ((header[i] = fgetc(png)) == EOF) {
			throw "Wrong header";
		}
	}
	uint64_t check = *reinterpret_cast<uint64_t*>(header);
	if (*reinterpret_cast<uint64_t*>(header) != 0x0a'1a'0a0d'474e50'89) {
		throw "Wrong header";
	}


	// Parse IHDR chunck
	get_chunck();
	ihdr_chunck();


	// Parse other chuncks
	while (true) {
		get_chunck();
		
		if (*curr_chunck.type == *reinterpret_cast<uint32_t*>(const_cast<char*>("PLTE"))) {
			plte_chunck();
		}
		else if (*curr_chunck.type == *reinterpret_cast<uint32_t*>(const_cast<char*>("IDAT"))) {
			idat_chunck();
		}
		else if (*curr_chunck.type == *reinterpret_cast<uint32_t*>(const_cast<char*>("IEND"))) {
			break;
		}
	}
}


PNG::~PNG() {
	if (plte != nullptr)
		delete[] plte;

	if (curr_chunck.type != nullptr)
		delete[] curr_chunck.type;
}