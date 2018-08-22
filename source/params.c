#include <params.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "exif_gps.h"

// static functions //
void params_default(params * param);

int params_parser(int argc, char * argv[], params * param) {
	
	params_default(param);
	
	for (int i = 1; i < argc; i ++) {
		char * command = argv[i];
		if (strcmp("-rb", command) == 0) {
			if (i + 1 >= argc) return 0;	// 參數不足 //
			float f = atof(argv[i + 1]);
			param->rb = f;
			i ++;
			continue;
		}
		if (strcmp("-gb", command) == 0) {
			if (i + 1 >= argc) return 0;	// 參數不足 //
			double f = atof(argv[i + 1]);
			param->gb = f;
			i ++;
			continue;
		}
		if (strcmp("-gb", command) == 0) {
			if (i + 1 >= argc) return 0;	// 參數不足 //
			double f = atof(argv[i + 1]);
			param->gb = f;
			i ++;
			continue;
		}
		if (strcmp("-latitude", command) == 0) {
			if (i + 1 >= argc) return 0;	// 參數不足 //
			double f = atof(argv[i + 1]);
			param->latitude = f;
			i ++;
			continue;
		}
		if (strcmp("-longitude", command) == 0) {
			if (i + 1 >= argc) return 0;	// 參數不足 //
			double f = atof(argv[i + 1]);
			param->longitude = f;
			i ++;
			continue;
		}
		if (strcmp("-rotate", command) == 0) {
			if (i + 1 >= argc) return 0;	// 參數不足 //
			double f = atof(argv[i + 1]);
			param->rotate = f;
			i ++;
			continue;
		}
		if (strcmp("-region", command) == 0) {
			if (i + 1 >= argc) return 0;	// 參數不足 //
			double f = atof(argv[i + 1]);
			param->region = f;
			i ++;
			continue;
		}
		if (strcmp("-r_scale", command) == 0) {
			if (i + 1 >= argc) return 0;	// 參數不足 //
			double f = atof(argv[i + 1]);
			param->r_scale = f;
			i ++;
			continue;
		}
		if (strcmp("-sun_region", command) == 0) {
			if (i + 1 >= argc) return 0;	// 參數不足 //
			double f = atof(argv[i + 1]);
			param->sun_region = f;
			i ++;
			continue;
		}
		if (strcmp("-ignore_sun", command) == 0) {
			param->ignore_sun = 1;
			continue;
		}
		if (strcmp("-out_image", command) == 0) {
			if (i + 1 >= argc) return 0;	// 參數不足 //
			param->out_image = argv[i + 1];
			i ++;
			continue;
		}
		
		if (param->input_image_file != NULL) {
			// unknown param //
			fprintf(stderr, "unknown param %s\n", command);
			return 0;
		}
		
		param->input_image_file = command;
	}
	
	if (param->input_image_file == NULL) {
	  fprintf(stderr, "need input file name\n");
	  return 0;
	}
	
	return 1;
}

void params_default(params * param) {
	param->rotate = 0.0;
	param->region = 60.0;
  param->r_scale = 1.0;
	param->rb = 0.8;
	param->gb = 0.8;
	param->latitude = EXIF_GPS_UNDEFINED;
	param->longitude = EXIF_GPS_UNDEFINED;
	param->sun_region = 5.0;
	param->y_day = -1.0;
	param->ignore_sun = 0;
	param->out_image = NULL;
	param->input_image_file = NULL;
}

void params_dump(params * param) {
	printf("rotate %lf\n", param->rotate);
	printf("region %lf\n", param->region);
  printf("r_scale %lf\n", param->r_scale);
	printf("rb %lf\n", param->rb);
	printf("gb %lf\n", param->gb);
	printf("latitude %lf\n", param->latitude);
	printf("longitude %lf\n", param->longitude);
	printf("sun_region %lf\n", param->sun_region);
	printf("y_day %lf\n", param->y_day);
	printf("ignore_sun %s\n", param->ignore_sun ? "yes" : "no");
	printf("out_image %s\n", (param->out_image != NULL) ? param->out_image : " ");
	printf("input_image_file %s\n", (param->input_image_file != NULL) ? param->input_image_file : " ");
}
