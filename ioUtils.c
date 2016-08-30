#include <getopt.h>

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "ioUtils.h"
#include "layout.h"
#include "imageUtils.h"


int scanInputValue (int argc, char** argv, struct collage_t* myCollage, int collage_size){	
	int  nValue, c;
	char *tValue, *oValue;
	
	opterr = 0;
	nValue=0;
	tValue=NULL;
	oValue=NULL;
	
	printf("Usage:\e[36m ./collageMaker  [-n num_photos] [-t extension] [-o output]\e[0m\n\n");

	while ((c = getopt (argc, argv, "n:t:o:")) != -1)
		switch (c){
			case 'n':
				nValue = atoi(optarg);
				
				if (nValue>MAX_NUM_FRAME){
					printf("You can't add more than 6 photos\n\n");
					return -1;
				}
				else
					if ((nValue<MIN_NUM_FRAME) & (nValue!=0)){
						printf("You can't add less than 2 photos\n\n");
						return -1;
					}

				break;
			case 't':
				tValue = optarg;
				if ((strcmp(tValue,"png")!=0) && (strcmp(tValue,"jpg")!=0)){
					printf("%s is not a valid photo extension.\nThe default one (.png) will be used.\n", tValue);
					tValue="png";
				}
				break;
			case 'o':
				oValue = optarg;
				break;
			case '?':
				printf("\nUsage:\e[36m ./collageMaker  [-n num_photos] [-t extension] [-o output]\e[0m\n");
				return -1;
			default:
				return -1;
  		}

	//check if all parameters have been set
	if (nValue==0){
		printf("You haven't chosen a valid number of photos.\nThe default value n=\e[36m2\e[0m will be used.\n");
		nValue=2;
	}
	if (tValue==NULL){
		printf("You haven't chosen a valid photo extension for the output.\nThe default extension \e[36m.png\e[0m will be used.\n");
		tValue="png";
	}
	if (oValue==NULL){
		printf("You haven't chosen a valid name for the output.\nThe default name \e[36m\"collageMakerOutput\"\e[0m will be used.\n");
		oValue="collageMakerOutput";
	}

	myCollage->num_images = nValue;
	myCollage->extension = tValue;
	myCollage->outputFileName = oValue;
	
	return 0;
}


int retrieveInput(struct collage_t* myCollage, int size){
	int ret;

	printf ("\nPress a key to see the available layouts for %i photos\n", myCollage->num_images);
	getchar();
	
	ret=chooseLayout(myCollage, size);	
	if (ret<0)
		return -1;

	ret=chooseColor(myCollage);	
	if (ret<0)
		return -1;

	myCollage->images = (VipsImage**)malloc(sizeof(VipsImage*) * myCollage->num_images);

	ret=chooseFiles(myCollage);	
	if (ret<0)
		return -1;
	
	return 0;
}

int extractExtension (char* photoName){
	char* extension;
  	extension = strrchr (photoName, '.');

	if (extension == NULL){
		printf("\e[91m\nThis file doesn't have any extension.\n\n\e[0m");
		return -1;
	}
	if ((strcmp(extension,".png")!=0) && (strcmp(extension,".jpeg")!=0) &&
	    (strcmp(extension,".jpg")!=0) && (strcmp(extension,".gif")!=0)){
		printf("\e[91m\nThe file extension is not a valid one. Only PNG, GIF and JPEG formats are admitted.\n\n\e[0m");
		return -1;
	}

	return 0;
}


int chooseLayout(struct collage_t * myCollage, int size){
	int num_layouts = get_num_layouts(myCollage->num_images);

	print_layouts(myCollage->num_images);

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
				printf("Chosen layout: \e[36m %c \e[0m \n", layout_id);				
				get_layout(&(myCollage->layout), myCollage->num_images, layout_index);
				break;
			}
		}
	}
	return 0;
}


int chooseColor(struct collage_t * myCollage){
	char colorID[3];
	int colID;
	int *RGBArray=NULL;

	while(1)
	{
		printf("\nChoose one of the following colors or press \'p\' to choose the RGB values [q to exit]\n");
		RGBArray = printColor();
		if (RGBArray==NULL)
			return -1;
		
		scanf ("%s", colorID);
		if (strcmp(colorID,"p") == 0){
			printf("Choose the RGB values\n");
			takeRGB(myCollage);
			break;
		}

		if (strcmp(colorID,"q") == 0){
			printf("\e[91m\nExiting...\n\n\e[0m");
			return -1;
		}
		
		colID=atoi(colorID);
		if ((colID!=0) & (colID<9))
		{
			colID--;
			myCollage->backgroundColour.r=RGBArray[colID*3];
			myCollage->backgroundColour.g=RGBArray[colID*3+1];
			myCollage->backgroundColour.b=RGBArray[colID*3+2];
			break;
		}
	}

	printf("Chosen RGB values: \e[36m %i-%i-%i \e[0m \n", myCollage->backgroundColour.r, myCollage->backgroundColour.g, myCollage->backgroundColour.b);

	return 0;
}

void takeRGB(struct collage_t * myCollage)
{
	int R,G,B;
	
	printf ("R:\t");
	takeSingleValue(&R);
	printf ("G:\t");
	takeSingleValue(&G);
	printf ("B:\t");
	takeSingleValue(&B);

	myCollage->backgroundColour.r=R;
	myCollage->backgroundColour.g=G;
	myCollage->backgroundColour.b=B;
	
}

void takeSingleValue(int* value){
	while (1)
	{
		scanf("%i",value );
		
		if ((*value<0)||(*value>255)){
			printf("\e[91m Choose a value between 0 and 255 \e[0m \t ");
		}
		else 
			break;
	}
}

int chooseFiles(struct collage_t* myCollage){
	int i, ret;
	char photoName[50];
	struct stat fileStat;
	
	i=0;
	printf("\n");
	while(i != myCollage->num_images){
		printf("Choose the name of the input photos to be used [q to exit]\n");
		scanf ("%s", photoName);
		printf("\n");
		if (strcmp(photoName,"q") == 0){
			printf("\e[91m\nExiting...\n\n\e[0m");
			return -1;
		}
		
		ret=extractExtension(photoName);
		if (ret<0)
			continue;
		
		if(stat(photoName,&fileStat) < 0) {
   			printf("\e[91mThe file doesn't exist. Choose another one.\n\n\e[0m");
        		continue;
		}
 		
		myCollage->images[i] = vips_image_new_from_file (photoName, NULL);
		i++;
	}

	return 1;
}

void printSummary(struct collage_t* myCollage){
	printf("\n\e[36mSUMMARY\e[0m\n");
	printf("\e[36m--------------------------------------------------------\e[0m\n");
	printf("Number of photos to use:\t%i\n", myCollage->num_images);
	printf("Name of the output file:\t%s.%s\n", myCollage->outputFileName, myCollage->extension);
	printf("RGB of the background:\t\t%i-%i-%i\n",myCollage->backgroundColour.r, myCollage->backgroundColour.g, myCollage->backgroundColour.b);
	printf("\e[36m--------------------------------------------------------\e[0m\n");
	printf("\n");
}

