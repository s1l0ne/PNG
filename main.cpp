#define _CRT_SECURE_NO_WARNINGS

#include "PNG.h"
#include <iostream>


int main() {
	FILE* file = fopen("pawn_w.png", "rb");
	if (file) {
		try {
			PNG img(file);
		}
		catch (const char* res) {
			std::cout << res;
		}
		fclose(file);
	}
	return 0;
}