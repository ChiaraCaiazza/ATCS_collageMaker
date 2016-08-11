/*include our header files*/
#include "collageMaker.h"
#include "ioUtils.h"
#include "layout.h"
#include "imageInfo.h"


/*
comando per debian :
		gcc -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0 -lconfig -Wall collageMaker.c ioUtils.c layout.c imageInfo.c `pkg-config --cflags glib-2.0` `pkg-config --libs glib-2.0` `pkg-config vips --cflags --libs` -o collageMaker

		

comando per mac:
		?

exec:
		./collageMaker [-n num (default=2)] [-t string (default=null)] [-o string (default=null)]

*/



int main(int argc, char **argv) {
	int ret;

	struct layout myLayout;


	if (vips_init (argv[0])){
    		vips_error_exit ("unable to start VIPS");
		return -1;
	}
	else 
		printf ("\nvips started...\t\e[34myes\e[0m\ncurrent version installed:\t\e[34m%s\n\n\e[0m", vips_version_string());

	ret=scanInputValue (argc, argv, &myLayout, sizeof myLayout );
	if (ret<0)
		return -1;

	
	ret = retrieveInput(&myLayout, sizeof myLayout);
	if (ret<0)
		return -1;

	printf("After retrieve input");
	printSummary(&myLayout);

	
	struct frame_t *frames = get_frames(2, 1);
	destroy_frames(frames);

	vips_shutdown();

	return 0;
}
 //inputImages/immaginePNG.png

	
	
