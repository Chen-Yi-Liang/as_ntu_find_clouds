#ifndef params_h
#define params_h

typedef struct {
	double rb;
	double gb;
	double latitude;
	double longitude;
	double rotate;
	double region;
  double r_scale;
	double sun_region;
	double y_day;	// 該年的第幾天 //
	int ignore_sun;
	char * out_image;
	char * input_image_file;
} params;

extern int params_parser(int argc, char * argv[], params * param);
extern void params_dump(params * param);

#endif
