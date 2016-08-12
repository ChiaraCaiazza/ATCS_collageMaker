#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <vips/vips.h>

//the function converts the RGB components into the XYZ ones
int RGB2XYZ(int r, int g, int b, float* x, float* y, float* z);

int get_width(const VipsImage *image);

int get_height(const VipsImage *image);

//horizontal image resolution in pixels per millimeter
double get_horiz_resol(const VipsImage *image);

//vertical image resolution in pixels per millimeter
double get_vert_resol(const VipsImage *image);

//the function returns the index into the array corresponding to the element with the minimum resolution
int min_resol(VipsImage **img_array, int num_elem);

//the function returns the index into the array corresponding to the element with the minimum height
int min_height(VipsImage **img_array, int num_elem);

//the function returns the index into the array corresponding to the element with the minimum width
int min_width(VipsImage **img_array, int num_elem);

//the function returns an array of width/height values
double* image_width_over_height(VipsImage **img_array, int num_elem);

/*The function takes 2 vectors as input: width/height values of images and width/height values of frames.
They function returns the best match between images and frames in the form of an array in which the 
index is the image index and the value is the frame index associated to the image. */
int* find_best_match(double* images_ratio, double* frames_ratio, int num_elem);
