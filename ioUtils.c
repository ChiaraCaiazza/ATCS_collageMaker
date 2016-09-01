#include <getopt.h>

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "ioUtils.h"
#include "layout.h"
#include "imageUtils.h"


int choose_layout(struct collage_t* collage, int size){
	int num_layouts = get_num_layouts(collage->num_images);

	print_layouts(collage->num_images);

	while(1)
	{
		printf("Choose one of the following layouts: \n");
		char layout_id = tolower(getchar());
		if(layout_id != '\n')
		{
			int layout_index = layout_id - 'a';
			if( layout_index < 0 || layout_index > num_layouts)
			{
				printf("\n\e[91mType a letter between 'a' and '%c'.\e[0m \n\n", 'a' + num_layouts - 1);
			}
			else
			{			
				get_layout(&(collage->layout), collage->num_images, layout_index);
				break;
			}
		}
	}
	return 0;
}

void take_single_value(int* value){
	while (1)
	{
		scanf("%i", value);
		
		if ((*value<0)||(*value>255)){
			printf("\e[91m Choose a value between 0 and 255 \e[0m \t ");
		}
		else 
			break;
	}
}

void takeRGB(struct collage_t * collage)
{
	int R,G,B;
	
	printf ("R:\t");
	take_single_value(&R);
	printf ("G:\t");
	take_single_value(&G);
	printf ("B:\t");
	take_single_value(&B);

	collage->background_colour.r=R;
	collage->background_colour.g=G;
	collage->background_colour.b=B;
	
}

int choose_colour(struct collage_t* collage){
	char colorID[3];
	int colID;
	int *RGB_array=NULL;

	while(1)
	{
		printf("\nChoose one of the following colors or press \'p\' to choose the RGB values [q to exit]\n");
		RGB_array = print_colour();
		if (RGB_array == NULL)
			return -1;
		
		scanf ("%s", colorID);
		if (strcmp(colorID,"p") == 0){
			printf("Choose the RGB values\n");
			takeRGB(collage);
			break;
		}

		if (strcmp(colorID,"q") == 0){
			printf("\e[91m\nExiting...\n\n\e[0m");
			return -1;
		}
		
		colID = atoi(colorID);
		if ((colID!=0) & (colID<9))
		{
			colID--;
			collage->background_colour.r=RGB_array[colID*3];
			collage->background_colour.g=RGB_array[colID*3+1];
			collage->background_colour.b=RGB_array[colID*3+2];
			break;
		}
	}

	printf("Chosen RGB values: \e[36m %i-%i-%i \e[0m \n", collage->background_colour.r, collage->background_colour.g, collage->background_colour.b);

	return 0;
}

int extract_extension (char* file){
	char* extension;
  	extension = strrchr(file, '.');

	if (extension == NULL){
		printf("\e[91m\nThis file doesn't have any extension.\n\n\e[0m");
		return -1;
	}
	if ((strcmp(extension, ".png")!=0) && (strcmp(extension, ".jpeg")!=0) &&
	    (strcmp(extension, ".jpg")!=0) && (strcmp(extension, ".gif")!=0)) {
		printf("\e[91m\nThe file extension is not a valid one. Only PNG, GIF and JPEG formats are admitted.\n\n\e[0m");
		return -1;
	}

	return 0;
}

int choose_files(struct collage_t* collage){
	int i, ret;
	char file_name[50];
	struct stat file_stat;
	
	i=0;
	printf("\n");
	while(i != collage->num_images){
		printf("Choose the name of the photos to be used [q to exit]\n");
		scanf ("%s", file_name);
		printf("\n");
		if (strcmp(file_name,"q") == 0){
			printf("\e[91m\nExiting...\n\n\e[0m");
			return -1;
		}
		
		ret = extract_extension(file_name);
		if (ret < 0)
			continue;
		
		if (stat(file_name, &file_stat) < 0) {
   			printf("\e[91mThe file doesn't exist. Choose another one.\n\n\e[0m");
        		continue;
		}
 		
		collage->images[i] = vips_image_new_from_file(file_name, NULL);
		i++;
	}

	return 1;
}

void print_summary(struct collage_t* collage){
	printf("\n\e[36mSUMMARY\e[0m\n");
	printf("\e[36m--------------------------------------------------------\e[0m\n");
	printf("Number of photos to use:\t%i\n", collage->num_images);
	printf("Name of the output file:\t%s.%s\n", collage->output_file_name, collage->extension);
	printf("RGB of the background:\t\t%i-%i-%i\n",collage->background_colour.r, collage->background_colour.g, collage->background_colour.b);
	printf("\e[36m--------------------------------------------------------\e[0m\n");
	printf("\n");
}

int scan_input(int argc, char** argv, struct collage_t* collage, int collage_size){	
	int  n_val = 0, c;
	char *t_val = NULL, *o_val = NULL;
	
	printf("Usage:\e[36m ./collageMaker  [-n num_photos] [-t extension] [-o output]\e[0m\n\n");

	while ((c = getopt(argc, argv, "n:t:o:")) != -1)
		switch (c){
			case 'n':
				n_val = atoi(optarg);
				
				if (n_val > MAX_NUM_FRAME){
					printf("\e[91mYou can't add more than 6 photos to the collage.\e[0m\n\n");
					return -1;
				}
				else
					if ((n_val < MIN_NUM_FRAME) & (n_val != 0)){
						printf("\e[91mYou can't add less than 2 photos to the collage.\e[0m\n\n");
						return -1;
					}

				break;
			case 't':
				t_val = optarg;
				if ((strcmp(t_val, "png")!=0) && (strcmp(t_val, "jpg")!=0)){
					printf("%s is not a valid photo extension.\nThe default one (.png) will be used.\n", t_val);
					t_val = "png";
				}
				break;
			case 'o':
				o_val = optarg;
				break;
			case '?':
				printf("\nUsage:\e[36m ./collageMaker  [-n num_photos] [-t extension] [-o output]\e[0m\n");
				return -1;
			default:
				return -1;
  		}

	//check if all parameters have been set
	if (n_val == 0){
		printf("You haven't chosen a valid number of photos.\nThe default value n=\e[36m2\e[0m will be used.\n");
		n_val = 2;
	}
	if (t_val == NULL){
		printf("You haven't chosen a valid photo extension for the output.\nThe default extension \e[36m.png\e[0m will be used.\n");
		t_val = "png";
	}
	if (o_val == NULL){
		printf("You haven't chosen a valid name for the output.\nThe default name \e[36m\"collageMakerOutput\"\e[0m will be used.\n");
		o_val = "collage_output";
	}

	collage->num_images = n_val;
	collage->extension = t_val;
	collage->output_file_name = o_val;
	
	return 0;
}


int retrieve_input(struct collage_t* collage, int size){
	int ret;

	printf ("\nPress a key to see the available layouts for %i photos\n", collage->num_images);
	getchar();
	
	ret = choose_layout(collage, size);	
	if (ret < 0)
		return -1;

	ret = choose_colour(collage);	
	if (ret < 0)
		return -1;

	collage->images = (VipsImage**)malloc(sizeof(VipsImage*) * collage->num_images);

	ret = choose_files(collage);	
	if (ret < 0)
		return -1;
	
	return 0;
}



