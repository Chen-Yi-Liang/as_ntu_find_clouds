#include <stdio.h>
#include "exif_gps.h"
#include "image_process.h"
#include "solar_location.h"
#include <math.h>

#ifndef PI
#define M_PI 3.14159265358979323846
#endif

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

void cloud_check(unsigned char * pixel) {
	double rb = ((double)pixel[0]) / ((double)pixel[2]);
	double gb = ((double)pixel[1]) / ((double)pixel[2]);
	unsigned char Gray = gray_value(pixel);
	if (rb > 0.8 && gb > 0.8) {
		// have cloud , draw r //
		pixel[0] = Gray;
		pixel[1] = 0;
		pixel[2] = 0;
	}
	else {
		pixel[0] = Gray;
		pixel[1] = Gray;
		pixel[2] = Gray;
	}
}

int process_image_data(image_data * data) {
	double r_rate = 0.98;
	double cx = (data->width - 1) / 2.;
	double cy = (data->height - 1) / 2.;
	double r_max = (cx > cy) ? cy : cx;
	r_max *= r_rate;	// count r < r_max //
	for (int y = 0; y < data->height; y ++) {
		for (int x = 0; x < data->width; x ++) {
			double dx = x - cx;
			double dy = y - cy;
			if (dx * dx + dy * dy < r_max * r_max) {
				cloud_check(data->data + ((x + y * data->width) * 3));
			}
		}
	}
}

void draw_circle(image_data * data, double cx, double cy, 
  double r1, double r2, unsigned char r, unsigned char g, unsigned char b) {
		
	r1 *= r1;
	r2 *= r2;
	for (int y = 0; y < data->height; y ++) {
		for (int x = 0; x < data->width; x ++) {
			double dx = x - cx;
			double dy = y - cy;
			double dr = dx * dx + dy * dy;
			if (dr > r1 && dr < r2) {
				unsigned char * p = data->data + (x + y * data->width) * 3;
				*p = r;
				*(p+1) = g;
				*(p+2) = b;
			}
		}
	}
}

double get_x(image_data * data, double e_angle, double azimuth) {

	double cx = (data->width - 1) / 2.;
	double cy = (data->height - 1) / 2.;
	double r_max = (cx > cy) ? cy : cx;
	double r = (1 - (e_angle / M_PI * 2.)) * r_max;
	return sin(- M_PI / 2. - azimuth) * r + r_max;
}

double get_y(image_data * data, double e_angle, double azimuth) {

	double cx = (data->width - 1) / 2.;
	double cy = (data->height - 1) / 2.;
	double r_max = (cx > cy) ? cy : cx;
	double r = (1 - (e_angle / M_PI * 2.)) * r_max;
	return cos(- M_PI / 2. - azimuth) * r + r_max;
}

void mark_solar(image_data * data, double solar_e_angle, double solar_azimuth,
  double r1, double r2, unsigned char r, unsigned char g, unsigned char b) {
	printf("mark_solar at %f, %f\n", solar_e_angle * 180. / M_PI, solar_azimuth * 180. / M_PI);
  double solar_x = get_x(data, solar_e_angle, solar_azimuth);
  double solar_y = get_y(data, solar_e_angle, solar_azimuth);
	draw_circle(data, solar_x, solar_y, r1, r2, r, g, b);
}

int JPEG2PNG(char * read_file, char * write_file) {

  image_data * data = create_image_data(read_file);
	if (data == NULL) return 0;
	process_image_data(data);
	if (!write_image_data(data, write_file)) {
		delete_image_data(data);
		printf("write png false\n");
		return 0;
	}
	delete_image_data(data);
  return 1;
}

int readExif(char * file_name) {
	
  exif_gps gps = get_exif_gps(file_name);
	printf("%lf,%lf\n", gps.latitude, gps.longitude);
  return 1;
}

int run_test(char * read_file, char * write_file) {
	exif_gps gps = get_exif_gps(read_file);
	double y_day = yday(2018, 5, 29, 23, 55, 0);
	double e_a, azimuth;
	solar_angle(gps.latitude * M_PI / 180., gps.longitude * M_PI / 180., y_day, &e_a, &azimuth);
	
	printf("%f,%f\n", gps.latitude, gps.longitude);
	printf("%f\n", y_day);
	printf("%f\n", e_a, azimuth);
	
  image_data * data = create_image_data(read_file);
	if (data == NULL) return 0;
	process_image_data(data);

	double r = (5. / 90. * data->width / 2.);
	mark_solar(data, e_a, azimuth, r - 1, r + 1, 0, 0xFF, 0);
	mark_solar(data, 22.32 * M_PI / 180., 70.52 * M_PI / 180., r - 1, r + 1, 0xFF, 0xFF, 0);
	mark_solar(data, (90 - 64.2423) * M_PI / 180., 70.52 * M_PI / 180., r - 1, r + 1, 0, 0, 0xFF);
	
	if (!write_image_data(data, write_file)) {
		delete_image_data(data);
		printf("write png false\n");
		return 0;
	}
	delete_image_data(data);
  return 1;
}

int main(int argc, char * argv[]) {
	
	run_test("./sample/20180529235500_12.jpg", "./output/temp.png");
	/*
	if (!readExif("./sample/20180529235500_12.jpg")) {
		return 0;
	}
	
	if (!JPEG2PNG("./sample/20180529235500_12.jpg", "./output/temp.png")) {
		return 0;
	}
	*/
	return 0;
}

