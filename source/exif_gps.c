#include "exif_gps.h"
#include <stdio.h>
#include <stdlib.h>
#include <libexif/exif-data.h>
#include <libexif/exif-tag.h>
#include <libexif/exif-entry.h>

/**
 * @return 1, -1. 0 => unknown value. 
 */
int longitude_ref_from_char(char c) {
	if (c == 'E' || c == 'e') return 1;
	if (c == 'W' || c == 'w') return -1;
	return 0;
}

/**
 * @return 1, -1. 0 => unknown value. 
 */
int latitude_ref_from_char(char c) {
	if (c == 'N' || c == 'n') return 1;
	if (c == 'S' || c == 's') return -1;
	return 0;
}

/**
 * 計算gps a,b,c 格式的 gps 經緯度。
 */
double gps_value_from_buffer(char * buf) {
	double h,m,s;
	sscanf(buf, "%lf,%lf,%lf", &h, &m, &s);
	return h + ((m + s / 60.) / 60.);
}

exif_gps parser_exif_gps(ExifData * exifData) {
	
	exif_gps rgps;
	rgps.latitude = EXIF_GPS_UNDEFINED;
	rgps.longitude = EXIF_GPS_UNDEFINED;
	
  char buffer[1024];
	int longitude_ref;
	int latitude_ref;
	double longitude;
	double latitude;
	
  ExifEntry * entry;
  entry	= exif_content_get_entry(exifData->ifd[EXIF_IFD_GPS], EXIF_TAG_GPS_LONGITUDE_REF);
  if (entry == NULL) {
		return rgps;
  }
	exif_entry_get_value(entry, buffer, sizeof(buffer));
	longitude_ref = longitude_ref_from_char(buffer[0]);
	
  entry	= exif_content_get_entry(exifData->ifd[EXIF_IFD_GPS], EXIF_TAG_GPS_LONGITUDE);
  if (entry == NULL) {
		return rgps;
  }
	exif_entry_get_value(entry, buffer, sizeof(buffer));
	longitude = gps_value_from_buffer(buffer);
	if (longitude < 0) {
		return rgps;
	}
	
  entry	= exif_content_get_entry(exifData->ifd[EXIF_IFD_GPS], EXIF_TAG_GPS_LATITUDE_REF);
  if (entry == NULL) {
		return rgps;
  }
	exif_entry_get_value(entry, buffer, sizeof(buffer));
	latitude_ref = latitude_ref_from_char(buffer[0]);
	
  entry	= exif_content_get_entry(exifData->ifd[EXIF_IFD_GPS], EXIF_TAG_GPS_LATITUDE);
  if (entry == NULL) {
		return rgps;
  }
	exif_entry_get_value(entry, buffer, sizeof(buffer));
	latitude = gps_value_from_buffer(buffer);
	if (latitude < 0) {
		return rgps;
	}
	
	rgps.latitude = latitude * latitude_ref;
	rgps.longitude = longitude * longitude_ref;
	
	return rgps;
}

exif_gps get_exif_gps(char * file_name) {

  ExifData * exifData = exif_data_new_from_file(file_name);
  if (!exifData) {
		exif_gps rgps;
		rgps.latitude = EXIF_GPS_UNDEFINED;
		rgps.longitude = EXIF_GPS_UNDEFINED;
		return rgps;
  }
  
	exif_gps rgps = parser_exif_gps(exifData);
	exif_data_free(exifData);
	return rgps;
}
