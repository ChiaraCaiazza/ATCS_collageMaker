/*include our header files*/
#include <math.h>
#include "collageMaker.h"
#include "ioUtils.h"
#include "layout.h"
#include "imageUtils.h"

void create_collage(struct collage_t* myCollage)
{
	int min_res, i;
	int *photo2frame;
	int canvasWidth, canvasHeight;
	double  *images_WHratio, *frames_WHratio;
	VipsImage *canvas, *canvas_col;
	double ink[3];
	
	min_res = min_resol(myCollage->images, myCollage->num_images);
	images_WHratio = image_width_over_height(myCollage->images, myCollage->num_images);
	frames_WHratio = frame_width_over_height(&myCollage->layout);
	photo2frame = find_best_match(images_WHratio, frames_WHratio, myCollage->num_images);
	
	//larghezza foto / (larghezza % frame)
	canvasWidth = (int) (get_width(myCollage->images[min_res]) / 
							get_frame_width(&myCollage->layout, photo2frame[min_res]));
	//altezza foto / (altezza % frame)
	canvasHeight = (int) (get_height(myCollage->images[min_res]) / 
							get_frame_height(&myCollage->layout, photo2frame[min_res]));
	
	printf("canvasWidth: %d, canvasHeight: %d\n", canvasWidth, canvasHeight);
	printf("fotoWidth: %d, fotoHeight: %d\n", get_width(myCollage->images[min_res]), get_height(myCollage->images[min_res]));
	printf("frameWidth: %f, frameHeight: %f\n", get_frame_width(&myCollage->layout, photo2frame[min_res]), get_frame_height(&myCollage->layout, photo2frame[min_res]));
	
	//crea il canvas nero
	vips_black (&canvas, canvasWidth, canvasHeight, NULL);
	//vips_xyz (&canvas, canvasWidth, canvasHeight, NULL);
	
	
	//lo colora del colore scelto dall'utente
	VipsInterpretation try_interp;
	try_interp = vips_image_guess_interpretation (canvas);
	vips_colourspace(canvas, &canvas_col, VIPS_INTERPRETATION_sRGB, NULL);
	/*
	ink[0] = myCollage->backgroundColour.x;
	ink[1] = myCollage->backgroundColour.y;
	ink[2] = myCollage->backgroundColour.z;*/
	ink[0] = 255;
	ink[1] = 255;
	ink[2] = 255;
	vips_draw_flood(canvas_col, ink, 3, 0, 0, NULL);	
	
	
	
	for(i = 0; i < myCollage->num_images; i++)
	{
		VipsImage *temp_image;
		int frame_i = photo2frame[i];
		int frame_width = get_frame_width(&myCollage->layout, frame_i) * canvasWidth;
		int frame_height = get_frame_height(&myCollage->layout, frame_i) * canvasHeight;
		if(i != min_res)
		{
			//scalare foto
			double scale_x = (double)frame_width / (double)get_width(myCollage->images[i]);
			double scale_y = (double)frame_height / (double)get_height(myCollage->images[i]);
			
			double scale = (scale_x < scale_y)? scale_x : scale_y;
			vips_resize(myCollage->images[i], &temp_image, scale,  NULL);
			myCollage->images[i] = temp_image;
		}
		
		//la posizione orizzontale del frame è data dalla posizione orizz. percentuale 
		//moltiplicata per il coefficiente di conversione
		int frame_posX = get_frame_posX(&myCollage->layout, frame_i) * canvasWidth;
		int frame_posY = get_frame_posY(&myCollage->layout, frame_i) * canvasHeight;
		
		//la posizione dell'immagine è quella del frame spostata di metà della differenza tra 
		//la larghezza/lunghezza del frame e quella della foto (la differenza può essere nulla)
		int image_posX = frame_posX + ( frame_width - get_width(myCollage->images[i]) )/2;
		int image_posY = frame_posY + ( frame_height - get_height(myCollage->images[i]) )/2;
		
		double frame_rot = get_frame_rot(&myCollage->layout, frame_i);
		if( frame_rot != 0)
		{
			double frame_rot_rad = frame_rot / (180.0 / M_PI);
			double aa, bb, cc, dd;
			aa = cos(frame_rot_rad);
			bb = -sin(frame_rot_rad);
			cc = sin(frame_rot_rad);
			dd = cos(frame_rot_rad);
			
			vips_affine (myCollage->images[i], &temp_image,aa, bb, cc, dd, NULL);
			myCollage->images[i] = temp_image;
		}
		
		vips_draw_image(canvas_col, myCollage->images[i], image_posX, image_posY, NULL);
		
		//vips_copy (prova, &out_image_temp, NULL);
		//vips_insert (out_image_temp ,myCollage->images[i], &prova, image_posX, image_posY, "expand", TRUE,  NULL);
		
				
	}
	
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
    		vips_error_exit ("unable to start VIPS");
		return -1;
	}
	else 
		printf ("\nvips started...\t\e[34myes\e[0m\ncurrent version installed:\t\e[34m%s\n\n\e[0m", vips_version_string());

	printf("\e[0;40m|  |\e[0;41m|  |\e[0;42m|  |\e[0;43m|  |\e[0;44m|  |\e[0;45m|  |\e[0;46m|  |\e[0;47m|  |\e[0m\n");
	
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
 //inputImages/immaginePNG.png


