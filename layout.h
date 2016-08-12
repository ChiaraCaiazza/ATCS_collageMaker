#ifndef LAYOUT_H_
#define LAYOUT_H_

#include <libconfig.h>

#define MAX_NUM_FRAME 6
#define MIN_NUM_FRAME 2

struct frame_t
{
	int pos_x;				//position of the left side of the frame
	int pos_y;				//position of the upper side of the frame
	int width;		
	int height;
	int rot;				//rotation angle
};

struct layout_t {
	int num_frames;
	int perc_to_pixel;
	struct frame_t* frames;
};

int get_layout(struct layout_t * out, int num_frame, int id_layout);
void destroy_layout(struct layout_t * layout);
int get_num_layouts(int num_frame);
void print_layouts(int num_frame);
double* frame_width_over_height(struct layout_t* layout);

#endif
