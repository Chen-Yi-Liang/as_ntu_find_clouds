#include "draw.h"

unsigned char gray_value(unsigned char * pixel) {
	int R = pixel[0];
	int G = pixel[1];
	int B = pixel[2];
	return (R*313524 + G*615514 + B*119538) >> 20;
}

void gray(unsigned char * pixel) {
	unsigned char Gray = gray_value(pixel);
	pixel[0] = Gray;
	pixel[1] = Gray;
	pixel[2] = Gray;
}

