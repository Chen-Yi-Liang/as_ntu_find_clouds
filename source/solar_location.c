#include "solar_location.h"
#include <math.h>

#ifndef PI
#define M_PI 3.14159265358979323846
#endif

double yday(int year, int month, int day, int hour, int min, int sec) {
	struct tm tm1;
	tm1.tm_year = year - 1900;
	tm1.tm_mon = month - 1;
	tm1.tm_mday = day;
	tm1.tm_hour = hour;
	tm1.tm_min = min;
	tm1.tm_sec = sec;
	return yday_with_time(mktime(&tm1));
}

double yday_with_time(time_t time) {
	struct tm * tm2 = localtime(&time);
	return tm2->tm_yday + 
	  ((tm2->tm_sec / 60. + tm2->tm_min) / 60. + tm2->tm_hour) / 24.;
}

double solar_dec(double yday) {
double b = 2. * M_PI * yday / 365.;
return 0.006918 - 
  0.399912 * cos(b) +
	0.070257 * sin(b) -
	0.006758 * cos(b * 2.) +
	0.000907 * sin(b * 2.) -
	0.002697 * cos(b * 3.) +
	0.00148 * sin(b * 3.);
}

double solar_local_ra(double longitude,double yday) {
	double time_in_day = yday - floor(yday);
	double solar_time = (0.5 - time_in_day);
	return longitude - (solar_time * 2. * M_PI);
}

double solar_elevation_angle(double latitude, double solar_dec, double solar_local_ra) {
	return asin(cos(solar_local_ra) * cos(solar_dec)* cos(latitude) +
	  sin(solar_dec) * sin(latitude));
}

double solar_azimuth(double latitude, double solar_dec, double solar_local_ra, double solar_e_angle) {
	double sin_azimuth = -sin(solar_local_ra) * cos(solar_dec) / cos(solar_e_angle);
	double cos_azimuth =(sin(solar_dec) - sin(solar_e_angle) * sin(latitude)) / 
	  cos(solar_e_angle)/ cos(latitude);
	return atan2(sin_azimuth, cos_azimuth);
}

void solar_angle(double latitude, double longitude, double yday, double * e_angle, double * azimuth) {
	
	double dec = solar_dec(yday);
	double ar = solar_local_ra(longitude, yday);
	*e_angle = solar_elevation_angle(latitude, dec, ar);
	*azimuth = solar_azimuth(latitude, dec, ar, *e_angle);
}
