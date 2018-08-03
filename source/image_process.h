#ifndef image_process_h
#define image_process_h
#include <stdio.h>

/**
 * return image struct
 */
typedef struct {
	int width;
	int height;
	unsigned char * data;
} image_data;

/** create an image_data with file name */
extern image_data * create_image_data(char * file_name);

/** write image_data to file */
extern int write_image_data(image_data * data, char * file_name);

/** delete an image_dta with file name */
extern void delete_image_data(image_data * data);

extern double get_x(image_data * data, double e_angle, double azimuth);

extern double get_y(image_data * data, double e_angle, double azimuth);

extern void draw_circle(image_data * data, double cx, double cy, 
  double r1, double r2, unsigned char r, unsigned char g, unsigned char b);

#endif
