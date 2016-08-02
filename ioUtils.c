#include <stdio.h>
#include <getopt.h>

#include "ioUtils.h"


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


int setLayout ( struct layout* myLayout, int size)
{
	(*myLayout).photo[0].dim.x=700;
	(*myLayout).photo[0].dim.y=500;
	(*myLayout).photo[1].dim.x=600;
	(*myLayout).photo[1].dim.y=514;
 
	(*myLayout).photo[0].pos.x=10;
	(*myLayout).photo[0].pos.y=10;
	(*myLayout).photo[1].pos.x=600;
	(*myLayout).photo[1].pos.y=514;
	
	(*myLayout).layoutDim.x = 2500;
	(*myLayout).layoutDim.y = 2500;
	
	(*myLayout).backgroundColor.r=0;
	(*myLayout).backgroundColor.g=55;
	(*myLayout).backgroundColor.b=25;
	

	return 1;
}

int evaluateNumPhoto(int numPhoto)
{
	printf ("\e[35mevluateNumPhoto ancora da fare!!\e[0m\n\n");
	return 0;
}

int printASCIIArt(int numPhoto)
{
	switch (numPhoto)
	{
		case 2:
			printf("\nA):\e[34m\n\t\t _____________\n\t\t|      |      |\n\t\t|      |      |\n\t\t|      |      |\n\t\t|      |      |\n\t\t|      |      |\n\t\t|______|______|\n\n\e[0m");
			printf("\nB):\e[34m\n\t\t _____________\n\t\t|             |\n\t\t|             |\n\t\t|             |\n\t\t|_____________|\n\t\t|             |\n\t\t|             |\n\t\t|             |\n\t\t|_____________|\n\n\e[0m");
			printf("\nC):\e[34m\n\t\t _________________\n\t\t| ______   ______ |\n\t\t||      | |      ||\n\t\t||      | |      ||\n\t\t||      | |      ||\n\t\t||      | |      ||\n\t\t||      | |      ||\n\t\t||______| |______||\n\t\t|_________________|\n\n\e[0m");
			printf("\nC):\e[34m\n\t\t ______________\n\t\t| ____________ |\n\t\t||            ||\n\t\t||            ||\n\t\t||            ||\n\t\t||____________||\n\t\t| ____________ |\n\t\t||            ||\n\t\t||            ||\n\t\t||            ||\n\t\t||____________||\n\t\t|______________|\n\n\e[0m");
			
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

int retrieveInput(struct layout* myLayout, int size)
{
	int ret;
	
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
	return 0;
}

