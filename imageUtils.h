#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <vips/vips.h>

int get_width(const VipsImage *image);

int get_height(const VipsImage *image);

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

//The function creates a blank canvas ready to be used by other draw functions
VipsImage* create_blank_canvas(int width, int height);

/*The function inserts a border (1pixel) around the image to avoid that the flood operation 
continues inside the image (expecially for black-based images)*/
void protect_image_from_flood(VipsImage* image);

//The function rotates the image (rotation is in degrees and clockwise)
void rotate_image(VipsImage** image, double rotation);
