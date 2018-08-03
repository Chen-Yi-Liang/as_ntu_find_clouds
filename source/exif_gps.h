#ifndef exif_gps_h
#define exif_gps_h

#define EXIF_GPS_UNDEFINED	-999.0

/**
 * return data type of exif_gps
 */
typedef struct {
	double latitude;
	double longitude;
} exif_gps;

extern exif_gps get_exif_gps(char * file_name);

#endif
