#include "imageInfo.h"

int 
get_width(const VipsImage *image) {
	return vips_image_get_width(image);
}

int
get_height(const VipsImage *image) {
	return vips_image_get_height(image);
}

double
get_horiz_resol(const VipsImage *image) {
	return vips_image_get_xres(image);
}

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
	for (i=1; i<num_elem; i++){
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
	for (i=1; i<num_elem; i++){
		if (get_width(list[i].image) < min){
			min = get_width(list[i].image);
			i_app = i;
		}
	}
	return i_app;
}

double*
im_width_over_height(struct collageMakerImage *list, int num_elem){
	int i;
	double* ret = (double*)malloc(sizeof(double)*num_elem);
	for (i=0; i<num_elem; i++)
		ret[i] = (double)get_width(list[i].image) / (double)get_height(list[i].image);
	return ret;
}
	
int*
find_best_match(double* images_ratio, double* frames_ratio, int num_elem){
	int i, j;
	int remaining = num_elem;
	int image_index, frame_index;
	double min;
	int* ret = (int*)malloc(sizeof(int)*num_elem);
	double** matrix = (double**)malloc(sizeof(double*)*num_elem);
	for (i=0; i<num_elem; i++){		
		matrix[i] = (double*)malloc(sizeof(double)*num_elem);
	}
	for (i=0; i<num_elem; i++){	//for every image
		for (j=0; j<num_elem; j++){	//for every frame
			if (frames_ratio[j] > images_ratio[i])
				matrix[i][j] = frames_ratio[j]-images_ratio[i];
			else
				matrix[i][j] = images_ratio[i]-frames_ratio[j];
		}
	}
	while (remaining!=0){
		image_index = 0;
		frame_index = 0;
		min = matrix[0][0];
		for (i=0; i<num_elem; i++){
			for (j=0; j<num_elem; j++){
				if (matrix[i][j] < min){					
					min = matrix[i][j];
					image_index = i;
					frame_index = j;
				}
			}
		}
		ret[image_index] = frame_index;
		remaining--;
		for (i=0; i<num_elem; i++){
			matrix[image_index][i] = 10.0;
			matrix[i][frame_index] = 10.0;
		}	
	}
	return ret;
}



