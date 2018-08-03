#include "image_process.h"
#include <malloc.h>
#include <string.h>
#include <jpeglib.h>
#include <png.h>

// private functions define //
image_data * create_image_data_with_jpg(char * file_name);
int write_image_data_with_png(image_data * data, char * file_name);


image_data * create_image_data(char * file_name) {
	return create_image_data_with_jpg(file_name);
}

int write_image_data(image_data * data, char * file_name) {
	return write_image_data_with_png(data, file_name);
}

void delete_image_data(image_data * data) {
	if (data != NULL) {
		if (data->data != NULL) {
			free(data->data);
		}
		free(data);
	}
}

// private functions //

image_data * create_image_data_with_jpg(char * file_name) {
	
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  FILE * ptr;

  if ((ptr = fopen(file_name, "rb")) == NULL) {
	  printf("open file %s false\n", file_name);
	  return NULL;
  }
  
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo);
  jpeg_stdio_src(&cinfo, ptr);
  jpeg_read_header(&cinfo, TRUE);
  
  printf("%d*%d\n", cinfo.image_width, cinfo.image_height);
  
	// set decompress param //
	cinfo.scale_num = 1;
	cinfo.scale_denom = 1;
	cinfo.out_color_space = JCS_RGB;

	// start decompress //
	jpeg_start_decompress(&cinfo);
	
	image_data * data = (image_data *)malloc(sizeof(image_data));
	data->width = cinfo.output_width;
	data->height = cinfo.output_height;
	int total_size = cinfo.output_width * cinfo.output_height * cinfo.output_components;
	data->data = (unsigned char *)malloc(total_size);
	unsigned char * data_buffer = data->data;
	
	int row_stride = cinfo.output_width * cinfo.output_components;
  while (cinfo.output_scanline < cinfo.output_height)
  {
		(void) jpeg_read_scanlines(&cinfo, &data_buffer, 1);
		data_buffer += row_stride;
	}
	
  // all data with R,G,B 3bytes array in output_components //
	printf("%d*%d (%d)\n", cinfo.output_width, cinfo.output_height, cinfo.output_components);
	
  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
  
  fclose(ptr);
  
  return data;
}

int write_image_data_with_png(image_data * data, char * file_name) {
	
	int returnCode = 1;
	FILE * fp = NULL;
	png_structp png_ptr = NULL;
	png_infop info_ptr = NULL;
	
  fp = fopen(file_name, "wb");
  if (fp == NULL) {
		returnCode = 0;
    goto finalise;
  }
	
  png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (png_ptr == NULL) {
		returnCode = 0;
    goto finalise;
  }
	
  info_ptr = png_create_info_struct(png_ptr);
  if (info_ptr == NULL) {
		returnCode = 0;
    goto finalise;
  }
	
	png_init_io(png_ptr, fp);
  png_set_IHDR(png_ptr, info_ptr, data->width, data->height,
	  8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
    PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	png_write_info(png_ptr, info_ptr);
	
	/*
	for (int y = 0; y < data->height; y ++) {
		for (int x = 0; x <data->width; x ++) {
			data->data[(data->width * y + x) * 3] = 0xFF;
		}
	}
	*/
	
	for (int y = 0; y < data->height; y ++) {
	  png_write_row(png_ptr, data->data + (data->width * 3 * y));
	}
	png_write_end(png_ptr, NULL);
	
  finalise:
  if (fp != NULL) fclose(fp);
  if (png_ptr != NULL) png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
	return returnCode;
}
