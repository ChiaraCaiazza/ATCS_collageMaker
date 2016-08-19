/*include our header files*/
#include "collageMaker.h"
#include "ioUtils.h"
#include "layout.h"
#include "imageUtils.h"

void create_collage(struct collage_t* myCollage)
{
	int min_res, frame_min_res = 0, i;
	int *frame2photo;
	int canvasWidth, canvasHeight;
	double  *images_WHratio, *frames_WHratio;
	VipsImage *canvas, *canvas_col;
	double ink[3];
	
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
	
	//photo width / (% width frame)
	canvasWidth = (int) (get_width(myCollage->images[min_res]) / 
							get_frame_width(&myCollage->layout, frame_min_res));
	//photo height / (% height frame)
	canvasHeight = (int) (get_height(myCollage->images[min_res]) / 
							get_frame_height(&myCollage->layout, frame_min_res));
	
	
	//create black canvas
	vips_black (&canvas, canvasWidth, canvasHeight, NULL);
	
	//paint the canvas with the colour chosen by the user
	VipsInterpretation try_interp;
	try_interp = vips_image_guess_interpretation (canvas);
	vips_colourspace(canvas, &canvas_col, VIPS_INTERPRETATION_sRGB, NULL);
	ink[0] = myCollage->backgroundColour.r;
	ink[1] = myCollage->backgroundColour.g;
	ink[2] = myCollage->backgroundColour.b;
	
	
	for(i = 0; i < myCollage->num_images; i++)
	{
		VipsImage *temp_image;
		int image_i = frame2photo[i];
		int frame_width = get_frame_width(&myCollage->layout, i) * canvasWidth;
		int frame_height = get_frame_height(&myCollage->layout, i) * canvasHeight;
		if(image_i != min_res)
		{
			//scale the photo
			double scale_x = (double)frame_width / (double)get_width(myCollage->images[image_i]);
			double scale_y = (double)frame_height / (double)get_height(myCollage->images[image_i]);
			
			double scale = (scale_x < scale_y)? scale_x : scale_y;
			vips_resize(myCollage->images[image_i], &temp_image, scale,  NULL);
			myCollage->images[image_i] = temp_image;
		}
		
		protect_image_from_flood(myCollage->images[image_i]);
		
		/* the horizontal/vertical position of the frame is given by the horizontal/vertical position 
		(%) times the conversion coefficient*/
		int frame_posX = get_frame_posX(&myCollage->layout, i) * canvasWidth;
		int frame_posY = get_frame_posY(&myCollage->layout, i) * canvasHeight;
		
		/* the image position is the frame position shifted by half of the difference between the 
		width/height of the frame and width/height of the photo (difference may be 0)*/
		int image_posX = frame_posX + ( frame_width - get_width(myCollage->images[image_i]) )/2;
		int image_posY = frame_posY + ( frame_height - get_height(myCollage->images[image_i]) )/2;
		
		double frame_rot = get_frame_rot(&myCollage->layout, i);
		if( frame_rot != 0)
		{
			rotate_image(&(myCollage->images[i]), frame_rot);
		}
		
		vips_draw_image(canvas_col, myCollage->images[image_i], image_posX, image_posY, NULL);
				
	}
	
	vips_draw_flood(canvas_col, ink, 3, 0, 0, "equal", TRUE, NULL);
	
	char filename[256];
	strcpy(filename, myCollage->outputFileName);
	strcat(filename, ".");
	strcat(filename, myCollage->extension);
	vips_image_write_to_file (canvas_col, filename, NULL);
}

int main(int argc, char **argv) {
	int ret;

	struct collage_t myCollage;


	if (vips_init (argv[0])){
    		vips_error_exit ("Unable to start VIPS");
		return -1;
	}
	else 
		printf ("\nVips started...\t\e[34myes\e[0m\ncurrent version installed:\t\e[34m%s\n\n\e[0m", vips_version_string());

	
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


