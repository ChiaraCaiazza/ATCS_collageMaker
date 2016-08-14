#ifndef LAYOUT_H_
#define LAYOUT_H_

#include <libconfig.h>

#define MAX_NUM_FRAME 6
#define MIN_NUM_FRAME 2

struct frame_t
{
	double pos_x;				//position of the left side of the frame
	double pos_y;				//position of the upper side of the frame
	double width;		
	double height;
	double rot;					//rotation angle
};

struct layout_t {
	int num_frames;
	int perc_to_pixel;
	struct frame_t* frames;
};

double get_frame_posX(const struct layout_t* layout, int index);
double get_frame_posY(const struct layout_t* layout, int index);
double get_frame_width(const struct layout_t* layout, int index);
double get_frame_height(const struct layout_t* layout, int index);
double get_frame_rot(const struct layout_t* layout, int index);
int get_layout(struct layout_t * out, int num_frame, int id_layout);
void destroy_layout(struct layout_t * layout);
int get_num_layouts(int num_frame);
void print_layouts(int num_frame);
int* printColor();
double* frame_width_over_height(struct layout_t* layout);

#endif
