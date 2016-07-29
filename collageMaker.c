/* Bring in standard I/O so we can output the PNG to a file */
#include <stdio.h>

/* Bring in gd library functions */
#include "lib/libgd-2.2.3/src/gd.h"
/*include our header files*/
#include "collageMaker.h"
#include "ioUtils.h"


int main(int argc, char **argv) {
	struct layout myLayout;
	int white, black, ret;
	/* Declare the image */
	gdImagePtr im, im1, im2, im3, canvas;
	
	FILE *in1, *in2, *in3;
	
	ret=scanInputValue (argc, argv, &myLayout, sizeof myLayout );
	if (ret<0)
	{
		printf("Error!");
	}
	
	ret=retrieveInput(&myLayout, sizeof myLayout);
	if (ret<0)
	{
		printf("Error!");
	}
	
	
	in1 = fopen("inputImages/immaginePNG.png", "rb");
	im1 = gdImageCreateFromPng(in1);
	fclose(in1);
	
	in2 = fopen("inputImages/immagineJPEG.jpg", "rb");
	im2 = gdImageCreateFromJpegEx(in2, 0);
	fclose(in2);
	
	in3 = fopen("inputImages/immagineGIF.gif", "rb");
	im3 = gdImageCreateFromGif(in3);
	fclose(in2);
	
	
	canvas = gdImageCreate(myLayout.layoutDim.x, myLayout.layoutDim.y);
	black = gdImageColorAllocate(canvas, 0, 0, 0, 0.5);
	white = gdImageColorAllocate(canvas, 255, 255, 255, 0.5);
	gdImageFilledRectangle (	canvas, 20, 20, 580, 580, white);
	
	//gdImageCopy (canvas, im2, 20, 20,0,0, 500, 375);
	
	createOutputImage(canvas);
}