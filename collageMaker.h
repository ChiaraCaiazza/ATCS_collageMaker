#ifndef COLLAGEMAKER_H_
#define COLLAGEMAKER_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "layout.h"
/*include vips*/
#include <vips/vips.h>


struct colour{
	double x;
	double y;
	double z;
};

struct collage_t {
	char* outputFileName;
	char* extension;
	int num_images;
	VipsImage** images;
	struct colour backgroundColour;
	struct layout_t layout;
};

#endif
