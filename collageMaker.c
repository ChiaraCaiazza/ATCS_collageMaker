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

void create_collage(struct collage_t* myCollage)
{
	int min_res, frame_min_res = 0, i;
	int *frame2photo;
	double  *images_WHratio, *frames_WHratio, ink[3];
	double frame_rot, frame_width_perc, frame_height_perc;
	VipsImage *canvas;
	
	min_res = min_resol(myCollage->images, myCollage->num_images);
	images_WHratio = image_width_over_height(myCollage->images, myCollage->num_images);
	frames_WHratio = frame_width_over_height(&myCollage->layout);
	frame2photo = find_best_match(images_WHratio, frames_WHratio, myCollage->num_images);
	
	for(i = 0; i < myCollage->num_images; i++)
	{
		if(frame2photo[i] == min_res)
		{
			frame_min_res = i;
			break;
		}
	}
	
	frame_width_perc = get_frame_width(&myCollage->layout, frame_min_res);
	frame_height_perc = get_frame_height(&myCollage->layout, frame_min_res);
	frame_rot = get_frame_rot(&myCollage->layout, frame_min_res);
	if(frame_rot != 0.0)
	{
		protect_image_from_flood(myCollage->images[min_res]);
		rotate_image(&(myCollage->images[min_res]), frame_rot);
		frame_rot = frame_rot / (180.0 / M_PI);
		
		myCollage->canvas_width = (int) (get_width(myCollage->images[min_res]) / 
										(frame_height_perc * fabs(sin(frame_rot)) 
											+ frame_width_perc * fabs(cos(frame_rot))));
		myCollage->canvas_height = (int) (get_height(myCollage->images[min_res]) / 
										(frame_height_perc * fabs(cos(frame_rot)) 
											+ frame_width_perc * fabs(sin(frame_rot))));

	}
	else
	{
		//photo width / (% width frame)
		myCollage->canvas_width = (int) (get_width(myCollage->images[min_res]) / 
										frame_width_perc);
		//photo height / (% height frame)
		myCollage->canvas_height = (int) (get_height(myCollage->images[min_res]) / 
										frame_height_perc);
	}

	//create black canvas
	canvas = create_blank_canvas(myCollage->canvas_width, myCollage->canvas_height);
	
	for(i = 0; i < myCollage->num_images; i++)
	{
		int image_i = frame2photo[i];
		
		//scale photo to fit into the frame
		if(image_i != min_res)
		{
			fit_image_into_frame(myCollage, image_i, i);	
			protect_image_from_flood(myCollage->images[image_i]);
			
			frame_rot = get_frame_rot(&myCollage->layout, i);
			if(frame_rot != 0.0)
			{
				//rotate photo
				rotate_image(&(myCollage->images[image_i]), frame_rot);
			}
			
		}
		
		//insert photo
		insert_collage_image(myCollage, i, image_i, canvas);
				
	}
	
	//paint with background colour chosen by user
	ink[0] = myCollage->backgroundColour.r;
	ink[1] = myCollage->backgroundColour.g;
	ink[2] = myCollage->backgroundColour.b;
	vips_draw_flood(canvas, ink, 3, 0, 0, "equal", TRUE, NULL);
	
	//store collage in a file
	char filename[256];
	strcpy(filename, myCollage->outputFileName);
	strcat(filename, ".");
	strcat(filename, myCollage->extension);
	vips_image_write_to_file (canvas, filename, NULL);
}




int main(int argc, char **argv) {
	int ret;

	struct collage_t myCollage;


	if (vips_init (argv[0])){
    		vips_error_exit ("Unable to start VIPS");
		return -1;
	}
	
	printf ("\e[36;1m\n\n\t\t--- Welcome in collage maker ---\e[0m\n\n");

	ret=scanInputValue (argc, argv, &myCollage, sizeof myCollage );
	if (ret<0)
		return -1;

	
	ret = retrieveInput(&myCollage, sizeof myCollage);
	if (ret<0)
		return -1;

	printSummary(&myCollage);
	create_collage(&myCollage);

	vips_shutdown();

	return 0;
}


