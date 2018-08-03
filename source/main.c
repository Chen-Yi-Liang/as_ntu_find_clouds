#include <stdio.h>
#include <math.h>
#include "find_clouds_def.h"
#include "exif_gps.h"
#include "image_process.h"
#include "solar_location.h"
#include "draw.h"
#include "params.h"

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
	
	params param;
	
	if (params_parser(argc, argv, &param) == 0) return 0;
	params_dump(&param);
	
	// run_test("./sample/20180529235500_12.jpg", "./output/temp.png");
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


