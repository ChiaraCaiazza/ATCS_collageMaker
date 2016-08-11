#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*include vips*/
#include <vips/vips.h>



#define MAX_NUM_FRAME 6
#define MIN_NUM_FRAME 2

#ifndef COLLAGEMAKER_H_
#define COLLAGEMAKER_H_
struct coord{
	int x;
	int y;
};

struct colour{
	int r;
	int g;
	int b;
};

struct collageMakerImage{
	struct coord pos;
	struct coord dim;
	int rot;
	VipsImage* image;
};

struct layout {
	int number;
	char* extension;
	char* outputFileName;
	char layoutID;
	struct coord layoutDim;
	struct colour backgroundColor;
	struct collageMakerImage *arrayOfImages;
};

#endif
