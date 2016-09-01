/*include our header files*/
#include "collageMaker.h"
#include "ioUtils.h"
#include "layout.h"
#include "imageUtils.h"
#include <math.h>

void fit_image_into_frame(struct collage_t* collage, int image, int frame)
{
	VipsImage *temp_image;
	int frame_width, frame_height;
	double scale_x, scale_y, scale;

	frame_width = get_frame_width(&collage->layout, frame) * collage->canvas_width;
	frame_height = get_frame_height(&collage->layout, frame) * collage->canvas_height;
	scale_x = (double)frame_width / (double)get_width(collage->images[image]);
	scale_y = (double)frame_height / (double)get_height(collage->images[image]);
	
	scale = (scale_x < scale_y)? scale_x : scale_y;
	vips_resize(collage->images[image], &temp_image, scale,  NULL);
	collage->images[image] = temp_image;
}

void insert_collage_image(struct collage_t* collage, int frame_index, int image_index, VipsImage* canvas)
{
	int box_posX, box_posY, box_width, box_height;
	int image_posX, image_posY;
	double frame_width_perc, frame_height_perc, frame_rot;
	
	/* the horizontal/vertical position of the frame is given by the horizontal/vertical position 
	(%) times the conversion coefficient*/
	box_posX = get_frame_posX(&collage->layout, frame_index) * collage->canvas_width;
	box_posY = get_frame_posY(&collage->layout, frame_index) * collage->canvas_height;
	
	frame_width_perc = get_frame_width(&collage->layout, frame_index);
	frame_height_perc = get_frame_height(&collage->layout, frame_index);
	
	frame_rot = get_frame_rot(&collage->layout, frame_index);	
	if(frame_rot != 0.0)
	{
		frame_rot = frame_rot / (180.0 / M_PI);
		//W_box = W_frame * |sin(alpha)| + H_frame * |cos(alpha)|
		box_width = (int)(frame_width_perc * fabs(cos(frame_rot)) * collage->canvas_width + 
					frame_height_perc * fabs(sin(frame_rot)) * collage->canvas_height);
		
		//H_box = W_frame * |cos(alpha)| + H_frame * |sin(alpha)|
		box_height = (int)(frame_width_perc * fabs(sin(frame_rot)) * collage->canvas_width + 
					frame_height_perc * fabs(cos(frame_rot)) * collage->canvas_height);
	}
	else
	{
		//W = W_frame
		box_width = (int)(frame_width_perc * collage->canvas_width);
		//H = H_frame
		box_height = (int)(frame_height_perc * collage->canvas_height);
	}

	/* the image position is the frame position shifted by half of the difference between the 
	width/height of the frame and width/height of the photo (difference may be 0)*/
	image_posX = box_posX + ( box_width - get_width(collage->images[image_index]) ) / 2;
	image_posY = box_posY + ( box_height - get_height(collage->images[image_index]) ) / 2;
	
	vips_draw_image(canvas, collage->images[image_index], image_posX, image_posY, NULL);
}

void create_collage(struct collage_t* collage)
{
	int min_res, frame_min_res = 0, i;
	int *frame2photo;
	double  *images_WHratio, *frames_WHratio, ink[3];
	double frame_rot, frame_width_perc, frame_height_perc;
	VipsImage *canvas;
	
	images_WHratio = image_width_over_height(collage->images, collage->num_images);
	frames_WHratio = frame_width_over_height(&collage->layout);
	frame2photo = find_best_match(images_WHratio, frames_WHratio, collage->num_images);
	
	//The algorithm chooses the photo with the minimum resolution 
	//to decide the absolute dimensions of the final collage.
	//It's better to reduce the size of the other photos
	//to avoid the presence of pixelated images
	min_res = min_resol(collage->images, collage->num_images);
	for(i = 0; i < collage->num_images; i++)
	{
		if(frame2photo[i] == min_res)
		{
			frame_min_res = i;
			break;
		}
	} 
	frame_width_perc = get_frame_width(&collage->layout, frame_min_res);
	frame_height_perc = get_frame_height(&collage->layout, frame_min_res);
	frame_rot = get_frame_rot(&collage->layout, frame_min_res);
	if(frame_rot != 0.0)
	{
		protect_image_from_flood(collage->images[min_res]);
		rotate_image(&(collage->images[min_res]), frame_rot);
		frame_rot = frame_rot / (180.0 / M_PI);
		
		//W_photo / (W_frame * |sin(alpha)| + (H_frame * |cos(alpha)|))
		collage->canvas_width = (int) (get_width(collage->images[min_res]) / 
										(frame_height_perc * fabs(sin(frame_rot)) 
											+ frame_width_perc * fabs(cos(frame_rot))));
		//H_photo / (H_frame * |sin(alpha)| + (W_frame * |cos(alpha)|))
		collage->canvas_height = (int) (get_height(collage->images[min_res]) / 
										(frame_height_perc * fabs(cos(frame_rot)) 
											+ frame_width_perc * fabs(sin(frame_rot))));

	}
	else
	{
		//W_photo / W_frame
		collage->canvas_width = (int) (get_width(collage->images[min_res]) / 
										frame_width_perc);
		//W_photo / W_frame
		collage->canvas_height = (int) (get_height(collage->images[min_res]) / 
										frame_height_perc);
	}


	//create blank canvas
	canvas = create_blank_canvas(collage->canvas_width, collage->canvas_height);
	
	
	for(i = 0; i < collage->num_images; i++)
	{
		int image_i = frame2photo[i];
		
		if(image_i != min_res)
		{
			//scale photo
			fit_image_into_frame(collage, image_i, i);	
			protect_image_from_flood(collage->images[image_i]);
			
			frame_rot = get_frame_rot(&collage->layout, i);
			if(frame_rot != 0.0)
			{
				//rotate photo
				rotate_image(&(collage->images[image_i]), frame_rot);
			}
			
		}
		
		//insert photo
		insert_collage_image(collage, i, image_i, canvas);
				
	}
	
	//paint with background colour chosen by user
	ink[0] = collage->background_colour.r;
	ink[1] = collage->background_colour.g;
	ink[2] = collage->background_colour.b;
	vips_draw_flood(canvas, ink, 3, 0, 0, "equal", TRUE, NULL);
	
	//store collage in a file
	char filename[256];
	strcpy(filename, collage->output_file_name);
	strcat(filename, ".");
	strcat(filename, collage->extension);
	vips_image_write_to_file (canvas, filename, NULL);
}




int main(int argc, char **argv) {
	int ret;
	struct collage_t collage;

	if (vips_init (argv[0])){
    		vips_error_exit ("Unable to start VIPS");
		return -1;
	}
	
	printf ("\e[36;1m\n\n\t\t--- Welcome in collage maker ---\e[0m\n\n");

	ret = scan_input(argc, argv, &collage, sizeof collage );
	if (ret<0)
		return -1;
	ret = retrieve_input(&collage, sizeof collage);
	if (ret<0)
		return -1;
	print_summary(&collage);

	create_collage(&collage);

	vips_shutdown();

	return 0;
}


