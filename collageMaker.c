/* Bring in standard I/O so we can output the PNG to a file */
#include <stdio.h>

/* Bring in gd library functions */
#include "lib/libgd-2.2.3/src/gd.h"
/*include our header files*/
#include "collageMaker.h"
#include "ioUtils.h"



int main() {
	struct layout myLayout;
	int ret;
	
	/* Declare the image */
	gdImagePtr im, im1, im2;
	/* Declare output files */

	
	
	ret=retrieveInput(&myLayout, sizeof myLayout);
	if (ret<0)
	{
		printf("Error!");
	}
	
	im = gdImageCreate(myLayout.layoutDim.x, myLayout.layoutDim.x);
	
	im1=gdImageCreate(myLayout.photo[0].dim.x, myLayout.photo[0].dim.x);
	im2=gdImageCreate(myLayout.photo[1].dim.x, myLayout.photo[1].dim.x);
	
	

	/* green layout*/
	ret=gdImageColorAllocate(im, myLayout.col.r, myLayout.col.g, myLayout.col.b);
	ret=gdImageColorAllocate(im1, 100, 255, 255);
	ret=gdImageColorAllocate(im2, 200, 255, 200);//
	
	gdImageCopy (im,im1,20,20,200,20,120,100);
	createOutputImage(im);
}