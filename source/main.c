#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "find_clouds_def.h"
#include "exif_gps.h"
#include "image_process.h"
#include "solar_location.h"
#include "draw.h"
#include "params.h"

void cloud_check(unsigned char * pixel, double rb, double gb) {
	double rbv = ((double)pixel[0]) / ((double)pixel[2]);
	double gbv = ((double)pixel[1]) / ((double)pixel[2]);
	unsigned char Gray = gray_value(pixel);
	if (rbv > rb && gbv > gb) {
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

int process_image_data(image_data * data, params * param) {
	double r_rate = param->region / 90.0;
	double cx = (data->width - 1) / 2.;
	double cy = (data->height - 1) / 2.;
	double r_max = (cx > cy) ? cy : cx;
	r_max *= r_rate;	// count r < r_max //
	for (int y = 0; y < data->height; y ++) {
		for (int x = 0; x < data->width; x ++) {
			double dx = x - cx;
			double dy = y - cy;
			if (dx * dx + dy * dy < r_max * r_max) {
				cloud_check(data->data + ((x + y * data->width) * 3), param->rb, param->gb);
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

/*
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
*/

int data_time_string_from_file_name(char * file_name, double * y_day) {
	int len = strlen(file_name);
	
	if (len < 21) return 0;
	/** @todo : 要能辨識大寫JPG */
	if (strcmp(file_name + len - 4, ".jpg") != 0) return 0;
	char buffer[10];
	
	strncpy(buffer, file_name + len - 21, 4); 
	buffer[4] = 0;
	int year = atoi(buffer);

	strncpy(buffer, file_name + len - 21 + 4, 2); 
	buffer[2] = 0;
	int month = atoi(buffer);
	
	strncpy(buffer, file_name + len - 21 + 6, 2); 
	buffer[2] = 0;
	int day = atoi(buffer);

	strncpy(buffer, file_name + len - 21 + 8, 2); 
	buffer[2] = 0;
	int hour = atoi(buffer);

	strncpy(buffer, file_name + len - 21 + 10, 2); 
	buffer[2] = 0;
	int minute = atoi(buffer);

	strncpy(buffer, file_name + len - 21 + 12, 2); 
	buffer[2] = 0;
	int second = atoi(buffer);

	// printf("%d %d %d %d %d %d\n", year, month, day, hour, minute, second);
	
	*y_day = yday(year, month, day, hour, minute, second);
	return 1;
}

int main(int argc, char * argv[]) {
	
	// 從command line 取出參數 //
	params param;
	if (params_parser(argc, argv, &param) == 0) return 0;
	
	// 讀取 gps //
	exif_gps gps = get_exif_gps(param.input_image_file);
	
	// 從檔名讀取日期 //
	double y_day = -1;
	data_time_string_from_file_name(param.input_image_file, &y_day);
	
	// 取代要計算的參數 //
	if (param.latitude == EXIF_GPS_UNDEFINED)
	  param.latitude = gps.latitude;
	if (param.longitude == EXIF_GPS_UNDEFINED)
		param.longitude = gps.longitude;
	if (param.y_day == -1.0)
		param.y_day = y_day;
	
	params_dump(&param);
	
  image_data * data = create_image_data(param.input_image_file);
	if (data == NULL) return 0;
	
	if (param.latitude != EXIF_GPS_UNDEFINED &&
	    param.longitude != EXIF_GPS_UNDEFINED &&
			(!param.ignore_sun) &&
			param.sun_region > 0) {
		// 要遮蔽太陽 //
		double e_a, azimuth;
		solar_angle(
		  param.latitude * M_PI / 180., 
		  param.longitude * M_PI / 180., 
			param.y_day, &e_a, &azimuth);
		azimuth -= (param.rotate * M_PI / 180.);
		
		double r = (param.sun_region / 90. * data->width / 2.);
		mark_solar(data, e_a, azimuth, r - 1, r + 1, 0, 0xFF, 0);
	}

	process_image_data(data, &param);
	
	if (param.out_image != NULL) {
		if (!write_image_data(data, param.out_image)) {
			delete_image_data(data);
			fprintf(stderr, "write png false\n");
			return 0;
		}
	}
	
	delete_image_data(data);
	
	return 0;
}


