#ifndef params_h
#define params_h

typedef struct {
	double rb;
	double gb;
	double latitude;
	double longitude;
	double rotate;
	double region;
	double sun_region;
	int ignore_sun;
	char * out_image;
	char * input_image_file;
} params;

extern int params_parser(int argc, char * argv[], params * param);
extern void params_dump(params * param);

#endif
