#ifndef exif_gps_h
#define exif_gps_h

#define EXIF_GPS_UNDEFINED	-999.0

/**
 * GPS的經緯度值，使用度為單位，latitude -90~90，longitude -180~180。
 * 沒有值時回傳為 EXIF_GPS_UNDEFINED。
 *
 */
typedef struct {
	double latitude;
	double longitude;
} exif_gps;

extern exif_gps get_exif_gps(char * file_name);

#endif
