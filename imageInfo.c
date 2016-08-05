#include "imageInfo.h"

int 
get_width(const VipsImage *image) {
	return vips_image_get_width(image);
}

int
get_height(const VipsImage *image) {
	return vips_image_get_height(image);
}

/*horizontal image resolution in pixels per millimeter*/
double
get_horiz_resol(const VipsImage *image) {
	return vips_image_get_xres(image);
}

/*vertical image resolution in pixels per millimeter*/
double 
get_vert_resol(const VipsImage *image) {
	return vips_image_get_yres(image);
}

int
min_resol(struct collageMakerImage *list, int num_elem){
	int i, i_app = 0;
	double min = get_horiz_resol(list[0].image)*get_vert_resol(list[0].image);
	for (i=1; i<num_elem; i++) {
		if (get_horiz_resol(list[i].image)*get_vert_resol(list[i].image) < min){
			min = get_horiz_resol(list[i].image)*get_vert_resol(list[i].image);
			i_app = i;
		}
	}
	return i_app;
}

int 
min_height(struct collageMakerImage *list, int num_elem){
	int i, i_app = 0;
	int min = get_height(list[0].image);
	for (i=0; i<num_elem; i++){
		if (get_height(list[i].image) < min){
			min = get_height(list[i].image);
			i_app = i;
		}
	}
	return i_app;
}

int 
min_width(struct collageMakerImage *list, int num_elem){
	int i, i_app = 0;
	int min = get_width(list[0].image);
	for (i=0; i<num_elem; i++){
		if (get_width(list[i].image) < min){
			min = get_width(list[i].image);
			i_app = i;
		}
	}
	return i_app;
}



