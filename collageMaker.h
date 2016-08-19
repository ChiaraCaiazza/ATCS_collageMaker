#ifndef COLLAGEMAKER_H_
#define COLLAGEMAKER_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "layout.h"
/*include vips*/
#include <vips/vips.h>


struct colour{
	int r;
	int g;
	int b;
};

struct collage_t {
	char* outputFileName;
	char* extension;
	int num_images;
	VipsImage** images;
	int canvas_width;
	int canvas_height;
	struct colour backgroundColour;
	struct layout_t layout;
};

#endif
