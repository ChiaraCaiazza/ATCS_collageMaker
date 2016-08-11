#include <libconfig.h>
#include "collageMaker.h"

struct frame_t
{
	int pos_x;				//position of the left side of the frame
	int pos_y;				//position of the upper side of the frame
	int width;		
	int height;
	int rot;				//rotation angle
};

struct frame_t* get_frames(int num_frame, int id_layout);
int get_num_layouts(int num_frame);
void destroy_frames(struct frame_t * list);
void print_layout(int num_frame);
double* frame_width_over_height(struct frame_t *list, int num_elem);
