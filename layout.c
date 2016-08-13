#include "layout.h"
#include <stdlib.h>
#include <stdio.h>
#include <libconfig.h>

double get_frame_posX(const struct layout_t* layout, int index)
{
	if(layout != NULL && layout->frames != NULL)
	{
		return layout->frames[index].pos_x;
	}
	return -1;
}

double get_frame_posY(const struct layout_t* layout, int index)
{
	if(layout != NULL && layout->frames != NULL)
	{
		return layout->frames[index].pos_y;
	}
	return -1;
}

double get_frame_width(const struct layout_t* layout, int index)
{
	if(layout != NULL && layout->frames != NULL)
	{
		return layout->frames[index].width;
	}
	return -1;
}

double get_frame_height(const struct layout_t* layout, int index)
{
	if(layout != NULL && layout->frames != NULL)
	{
		return layout->frames[index].height;
	}
	return -1;
}

double get_frame_rot(const struct layout_t* layout, int index)
{
	if(layout != NULL && layout->frames != NULL)
	{
		return layout->frames[index].rot;
	}
	return -1;
}

int get_layout(struct layout_t* out, int num_frame, int id_layout)
{
	config_setting_t *category_list, *category, *layout_list, *layout, *frame_list, *frame;
	config_t layout_config;
	int frame_length, i;
	out->num_frames = num_frame;
	out->perc_to_pixel = 0; // 
	out->frames = (struct frame_t*) malloc (num_frame * sizeof(struct frame_t));

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
		pos= config_setting_get_member(frame, "pos");
		size = config_setting_get_member(frame, "size");
		if (config_setting_lookup_float(pos, "x", &(out->frames[i].pos_x))==CONFIG_FALSE)
			printf("pos.x:\tONFIG_FALSE\n");
		else printf("pos.x:\tok!\n");

		if ( config_setting_lookup_float(pos, "y", &(out->frames[i].pos_y))==CONFIG_FALSE)
			printf("pos.y:\tONFIG_FALSE\n");
		else printf("pos.y:\tok!\n");
		
		if ( config_setting_lookup_float(size, "w", &(out->frames[i].width))==CONFIG_FALSE)
			printf("size.w:\tONFIG_FALSE\n");
		else printf("size.w:\tok!\n");

		if ( config_setting_lookup_float(size, "h", &(out->frames[i].height))==CONFIG_FALSE)
			printf("size.h:\tONFIG_FALSE\n");
		else printf("size.h:\tok!\n");

		
		//config_setting_lookup_float(frame, "rot", &(out->frames[i].rot));
	}

	
	config_destroy(&layout_config);
	return 0;
}

void destroy_layout(struct layout_t* layout)
{
	free(layout->frames);
	free(layout);
}

int get_num_layouts(int num_frame)
{
	config_setting_t *category_list, *category, *layout_list;
	config_t layout_config;
	int num_layouts;

	config_init(&layout_config);
	config_read_file(&layout_config, "./layout.cfg");
	
	category_list = config_lookup(&layout_config, "application.layout_group");
	category = config_setting_get_elem(category_list, num_frame - MIN_NUM_FRAME);
	
	layout_list = config_setting_get_member(category, "layout");
	num_layouts = config_setting_length(layout_list);
	
	config_destroy(&layout_config);
	return num_layouts;
}



void print_layouts(int num_frame)
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
		printf(" %c)\n", 'A' + i);
		printf("%s\n", ascii_image);
	}
	
	config_destroy(&layout_config);
}


int printColor()
{
	config_setting_t *colorList, *layout;
	config_t layoutConfig;
	int colorLength, i;
	const char* colorValue;

	config_init(&layoutConfig);
	if (!config_read_file(&layoutConfig, "./layout.cfg")) {
        	fprintf(stderr, "%s:%d - %s\n",
           	config_error_file(&layoutConfig),
            	config_error_line(&layoutConfig),
            	config_error_text(&layoutConfig));
        	config_destroy(&layoutConfig);
        	return -1;
    	}
	
	colorList = config_lookup(&layoutConfig, "application.colors");
	colorLength = config_setting_length(colorList);
	for(i = 0; i < colorLength; i++)
	{
		layout = config_setting_get_elem(colorList, i);
		config_setting_lookup_string(layout, "name", &colorValue);
		printf(" %i)\t", i);
		printf("%s\n", colorValue);
	}
	
	config_destroy(&layoutConfig);
	return 0;
}

double* frame_width_over_height(struct layout_t* layout){
	int i;
	double* ret = (double*) malloc (sizeof(double) * layout->num_frames);
	for (i=0; i< layout->num_frames; i++)
	{
		ret[i] = (double)(layout->frames[i].width) / (double)(layout->frames[i].height);
	}
	return ret;
}
	
