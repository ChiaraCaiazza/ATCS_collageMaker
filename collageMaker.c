/* Bring in standard I/O */
#include <stdio.h>

/*include vips*/
#include <vips/vips.h>

/*include our header files*/
#include "collageMaker.h"
#include "ioUtils.h"


/*
comando per debian :
		gcc -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0 -Wall collageMaker.c ioUtils.c `pkg-config --cflags glib-2.0` `pkg-config --libs glib-2.0` `pkg-config vips --cflags --libs` -o collageMaker
		

comando per mac:
		?

exec:
		./collageMaker [-n num (default=2)] [-t string (default=null)] [-o string (default=null)]

*/

int main(int argc, char **argv) {
	int ret;
	struct layout myLayout;
	VipsImage *im1, *im2, *im3;    
	double ink;

	ink=0;


	if (vips_init (argv[0])){
    		vips_error_exit ("unable to start VIPS");
		return -1;
	}
	else 
		printf ("\nvips started...\tyes\ncurrent version installed:\t\e[34m%s\n\n\e[0m", vips_version_string());

	ret=scanInputValue (argc, argv, &myLayout, sizeof myLayout );
	if (ret<0)
	{
		printf("Error!");
		return -1;

	}
	ret=retrieveInput(&myLayout, sizeof myLayout);
	if (ret<0)
	{
		printf("Error!");
		return -1;
	}


	im1 = vips_image_new_from_file ("inputImages/immaginePNG.png", NULL);
	im2 = vips_image_new_from_file ("inputImages/immagineJPEG.jpg", NULL);
	im3 = vips_image_new();
	vips_black (&im3,400,400,NULL);
	
	
	vips_draw_image (im2, im1, 0, 0, NULL);
	
	vips_draw_rect1 (im3, ink, 20, 20, 200, 200, NULL);
	vips_draw_flood1 (im3, ink, 30, 30, NULL);
	vips_draw_rect1 (im3, 100, 30, 30, 180, 180, NULL);
	vips_draw_flood1 (im3, 100, 40, 40, NULL);
				
	vips_image_write_to_file (im1, "test.jpg", NULL);
	vips_image_write_to_file (im2, "test2.png", NULL);
	vips_image_write_to_file (im3, "test3.png", NULL);
	
	vips_shutdown();

	return 0;
}
