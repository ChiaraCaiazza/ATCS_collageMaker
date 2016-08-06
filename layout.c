#include "layout.h"
#include <stdlib.h>
#include <stdio.h>
#include <libconfig.h>


void append_frame(struct frame_t *head, config_setting_t *frame_config)
{
	
	struct frame_t *q = head;
	
	struct frame_t *frame = (struct frame_t*) malloc (sizeof(struct frame_t));
	config_setting_lookup_int(frame_config, "pos_x", &(frame->pos_x));
	config_setting_lookup_int(frame_config, "pos_y", &(frame->pos_y));
	config_setting_lookup_int(frame_config, "width", &(frame->width));
	config_setting_lookup_int(frame_config, "height", &(frame->height));
	config_setting_lookup_int(frame_config, "rot", &(frame->rot));
	frame->next = NULL;
	
	if(q == NULL)
	{
		head = frame;
		return;
	}
	while(q->next != NULL)
		q = q->next;
	q->next = frame;
	
}


struct frame_t* get_frames(int num_frame, int id_layout)
{
	config_setting_t *category_list, *category, *layout_list, *layout, *frame_list, *frame;
	config_t layout_config;
	int frame_length, i;
	struct frame_t *result = NULL;

	config_init(&layout_config);
	config_read_file(&layout_config, "./layout.cfg");
	
	category_list = config_lookup(&layout_config, "application.layout_group");
	category = config_setting_get_elem(category_list, num_frame - MIN_NUM_FRAME);
	
	layout_list = config_setting_get_member(category, "layout");
	layout = config_setting_get_elem(layout_list, id_layout);

	frame_list = config_setting_get_member(layout, "frame");
	frame_length = config_setting_length(frame_list);
	for(i = 0; i < frame_length; i++)
	{
		frame = config_setting_get_elem(frame_list, i);
		append_frame(result, frame);
	}

	
	config_destroy(&layout_config);
	return result;
}

void destroy_frames(struct frame_t *head)
{
	struct frame_t *temp = head;
	while(head != NULL)
	{
		head = head->next;
		free(temp);
		temp = head;
	}
}

void print_layout(int num_frame)
{
	
	config_setting_t *category_list, *category, *layout_list, *layout;
	config_t layout_config;
	int layout_length, i;
	const char* ascii_image;

	config_init(&layout_config);
	config_read_file(&layout_config, "./layout.cfg");
	
	category_list = config_lookup(&layout_config, "application.layout_group");
	category = config_setting_get_elem(category_list, num_frame - MIN_NUM_FRAME);
	
	layout_list = config_setting_get_member(category, "layout");
	layout_length = config_setting_length(layout_list);
	for(i = 0; i < layout_length; i++)
	{
		layout = config_setting_get_elem(layout_list, i);
		config_setting_lookup_string(layout, "image", &ascii_image);
		printf("%s\n", ascii_image);
	}
	
	config_destroy(&layout_config);
}

