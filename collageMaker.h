#ifndef COLLAGEMAKER_H_
#define COLLAGEMAKER_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "layout.h"
/*include vips*/
#include <vips/vips.h>


struct colour{
	int x;
	int y;
	int z;
};

struct collage_t {
	int num_images;
	char* outputFileName;
	char* extension;
	VipsImage** images;
	struct colour backgroundColor;
	struct layout_t layout;
};

#endif
