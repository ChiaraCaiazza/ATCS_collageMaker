#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <vips/vips.h>
#include "collageMaker.h"


int get_width(const VipsImage *image);

int get_height(const VipsImage *image);

/*horizontal image resolution in pixels per millimeter*/
double get_horiz_resol(const VipsImage *image);

/*vertical image resolution in pixels per millimeter*/
double get_vert_resol(const VipsImage *image);

//the function returns the index into the array corresponding to the element with the minimum resolution
int min_resol(struct collageMakerImage* list, int num_elem);

//the function returns the index into the array corresponding to the element with the minimum height
int min_height(struct collageMakerImage* list, int num_elem);

//the function returns the index into the array corresponding to the element with the minimum width
int min_width(struct collageMakerImage* list, int num_elem);
