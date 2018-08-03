#ifndef solar_location_h
#define solar_location_h

#include <time.h>

// use utc time to yday //
double yday_with_time(time_t time);
double yday(int year, int month, int day, int hour, int min, int sec);
void solar_angle(double latitude, double longitude, double yday, double * e_angle, double * azimuth); 

// double solar_dec(double yday);
// double solar_local_ra(double longitude,double yday);
// double solar_elevation_angle(double latitude, double solar_dec,double solar_local_ra);
// double solar_azimuth(double latitude, double solar_dec, double solar_local_ra, double solar_e_angle);

#endif
