#include <getopt.h>

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "ioUtils.h"

int setLayout ( struct layout*, int);
int printASCIIArt(int);
int extractExtension (char* photoName);


int retrieveInput(struct layout* myLayout, int size)
{
	int ret, inserted;
	char photoName[50];
	
	printf ("\e[36m\nBenvenuto in collage maker\e[0m\n\n");
	
	printf("Questi sono i layout disponibili per %i foto", (*myLayout).number);
	
	ret = printASCIIArt((*myLayout).number);
	if (ret<0)
	{
		printf ("\e[31mError: ASCII art not found\e[0m\n\n");
	}
	

	printf ("\e[35mAttenzione!!\n\nBisogna finire di implementare ancora tutta la parte di raccolta proferenze dall'utente!!\e[0m\n\n");
	
	ret= setLayout(myLayout, size);
	if (ret<0)
	{
		printf("Error!");
		return -1;
	}

	printf("\e[35m Chiedere quale layout ed il colore\e[0m\n\n");
	
	
	struct collageMakerImage temp[(*myLayout).number];
	(*myLayout).arrayOfImages= temp;

	inserted=0;
	while(inserted!=(*myLayout).number){
		struct stat fileStat;
		VipsImage *im;

		printf("inserireil nome della foto [q per terminare]\n");
		scanf ("%s", photoName);
		printf("\n\n");
		if (strcmp(photoName,"q") == 0){
			printf("\e[91m\nOperazione annullata\n\n\e[0m");
			return -1;
		}
		ret=extractExtension(photoName);
		if (ret<0)
			continue;
		if(stat(photoName,&fileStat) < 0) {
   			printf("\e[91m\nErrore: file inesistente.\n\n\e[0m");
        		continue;
		}
 		
    
		im = vips_image_new_from_file (photoName, NULL);
		(*myLayout).arrayOfImages[inserted].image=im;
		inserted+=1;

	}
	
	
	//vips_image_write_to_file ((*myLayout).arrayOfImages[0].image, "testarray.jpg", NULL);
	return 0;
}

int scanInputValue (int argc, char** argv, struct layout* myLayout, int layoutSize){
		
	int  nValue, c;
	char *tValue, *oValue;

	opterr = 0;
	nValue=2;
	tValue=NULL;
	oValue=NULL;
	
	while ((c = getopt (argc, argv, "n:t:o:")) != -1)
		switch (c){
			case 'n':
				nValue = atoi(optarg);
				
				if (nValue>MAX_NUM_PHOTO){
					printf("Non e' possibile inserire piu' di 6 foto\n\n");
					return -1;
				}
				else
					if ((nValue<MIN_NUM_PHOTO) & (nValue!=0)){
						printf("Non e' possibile inserire meno di 2 foto\n\n");
						return -1;
					}
					else if (nValue==0) {
						printf("Inserire un carattere valido\n\n");
						return -1;
						}
				break;
			case 't':
				tValue = optarg;
				break;
			case 'o':
				oValue = optarg;
				break;
			case '?':
				printf("\nFormato comando:\e[36m ./collageMaker  [-n num] [-t extension] [-o output]\e[0m\n");
				return -1;
			default:
				return -1;
  		}

	(*myLayout).number=nValue;
	(*myLayout).extension=tValue;
	(*myLayout).outputFileName=oValue;
	
	printf ("# photo(-n) = %i, extensionName(-t) = %s, OutputFileName(-o) = %s\n",(*myLayout).number, (*myLayout).extension, (*myLayout).outputFileName);
	
	return 1;
}


void printSummary(struct layout* myLayout){

	printf("Summary/n");
	printf("\tNumero di foto da stampare:\t%i\n", (*myLayout).number);
	printf("\tNome del file finale:\t%s.%s\n", (*myLayout).outputFileName, (*myLayout).extension);
}






int setLayout ( struct layout* myLayout, int size)
{
	
/*
	(*myLayout).arrayOfImages[0].dim.x=700;
	(*myLayout).arrayOfImages[0].dim.y=500;
	(*myLayout).arrayOfImages[1].dim.x=600;
	(*myLayout).arrayOfImages[1].dim.y=514;
 
	(*myLayout).arrayOfImages[0].pos.x=10;
	(*myLayout).arrayOfImages[0].pos.y=10;
	(*myLayout).arrayOfImages[1].pos.x=600;
	(*myLayout).arrayOfImages[1].pos.y=514;

	
	(*myLayout).layoutDim.x = 2500;
	(*myLayout).layoutDim.y = 2500;
	
	(*myLayout).backgroundColor.r=0;
	(*myLayout).backgroundColor.g=55;
	(*myLayout).backgroundColor.b=25;
	
*/
	return 1;
}


int printASCIIArt(int numPhoto){
	switch (numPhoto)
	{
		case 2:
			printf("\nA):\e[34m\n\t\t _____________\n\t\t|      |      |\n\t\t|      |      |\n\t\t|      |      |\n\t\t|      |      |\n\t\t|      |      |\n\t\t|______|______|\n\n\e[0m");
			printf("\nB):\e[34m\n\t\t _____________\n\t\t|             |\n\t\t|             |\n\t\t|             |\n\t\t|_____________|\n\t\t|             |\n\t\t|             |\n\t\t|             |\n\t\t|_____________|\n\n\e[0m");
			printf("\nC):\e[34m\n\t\t _________________\n\t\t| ______   ______ |\n\t\t||      | |      ||\n\t\t||      | |      ||\n\t\t||      | |      ||\n\t\t||      | |      ||\n\t\t||      | |      ||\n\t\t||______| |______||\n\t\t|_________________|\n\n\e[0m");
			printf("\nD):\e[34m\n\t\t ______________\n\t\t| ____________ |\n\t\t||            ||\n\t\t||            ||\n\t\t||            ||\n\t\t||____________||\n\t\t| ____________ |\n\t\t||            ||\n\t\t||            ||\n\t\t||            ||\n\t\t||____________||\n\t\t|______________|\n\n\e[0m");
			
			break;
		case 3:
			printf ("\e[35mError: ASCII art for %i not found\e[0m\n\n", numPhoto);
			
			
			break;
		case 4:
			printf ("\e[35mError: ASCII art for %i not found\e[0m\n\n", numPhoto);
			
			
			break;
		case 5:
			
			printf ("\e[35mError: ASCII art for %i not found\e[0m\n\n", numPhoto);
			
			break;
		case 6:
			
			
			printf ("\e[35mError: ASCII art for %i not found\e[0m\n\n", numPhoto);
			
			break;
		default:
			printf ("\e[31mError in retrieving ASCII art");
			return -1;
	
	}
	return 1;
}



int extractExtension (char* photoName){
	char* extension;

  	extension = strrchr (photoName, '.');

	if (extension == NULL){
		printf("\e[91m\nErrore: questo file non ha alcuna estensione.\n\n\e[0m");
		return -1;
	}
	if ((strcmp(extension,".png")!=0) && (strcmp(extension,".jpeg")!=0) &&
	    (strcmp(extension,".jpg")!=0) && (strcmp(extension,".gif")!=0)){
		printf("\e[91m\nEstensione file non valida. sono ammesse solo foto in formato PNG, GIF e JPEG.\n\n\e[0m");
		return -1;
	}

	return 1;
}

