#include <libconfig.h>
#include "collageMaker.h"

struct frame_t
{
	int pos_x;				//position of the left side of the frame
	int pos_y;				//position of the upper side of the frame
	int width;		
	int height;
	int rot;				//rotation angle
	struct frame_t *next;		//pointer to the next frame
};

void append_frame(struct frame_t * head, config_setting_t * frame_config);
struct frame_t* get_frames(int num_frame, int id_layout);
void destroy_frames(struct frame_t * head);
void print_layout(int num_frame);
