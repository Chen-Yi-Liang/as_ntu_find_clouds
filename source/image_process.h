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

#endif
