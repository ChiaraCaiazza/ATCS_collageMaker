#include <getopt.h>

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "ioUtils.h"
#include "layout.h"

int setLayout ( struct layout*, int);
int extractExtension (char* photoName);


int retrieveInput(struct layout* myLayout, int size){
	int ret, inserted;
	char photoName[50];
	

	printf ("\e[36;1m\nBenvenuto in collage maker\e[0m\n\n");
	
	printf("Premere un tasto per visualizzare i layout disponibili per %i foto\n", (*myLayout).number);
	getchar();
	
	print_layout((*myLayout).number);
	

	printf ("\e[35mAttenzione!!\n\nBisogna finire di implementare ancora tutta la parte di raccolta proferenze dall'utente!!\e[0m\n\n");
	
	ret= setLayout(myLayout, size);
	if (ret<0){
		printf("Error!");
		return -1;
	}

	printf("\e[35m Chiedere quale layout ed il colore\e[0m\n\n");
	
	
	struct collageMakerImage temp;
	(*myLayout).arrayOfImages= (struct collageMakerImage*)malloc(sizeof temp* (*myLayout).number);

	inserted=0;
	while(inserted!=(*myLayout).number){
		struct stat fileStat;
		VipsImage *im = NULL;;

		printf("Inserire il nome della foto [q per terminare]\n");
		scanf ("%s", photoName);
		printf("\n");
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
		vips_image_write_to_file (im, "inserted.jpg", NULL);
		vips_image_write_to_file (((*myLayout).arrayOfImages[inserted]).image, "inserted2.jpg", NULL);


		
		inserted+=1;
	}
	/*/*/
	return 0;
}


int scanInputValue (int argc, char** argv, struct layout* myLayout, int layoutSize){	
	int  nValue, c;
	char *tValue, *oValue;

	
	opterr = 0;
	nValue=0;
	tValue=NULL;
	oValue=NULL;
	
	while ((c = getopt (argc, argv, "n:t:o:")) != -1)
		switch (c){
			case 'n':
				nValue = atoi(optarg);
				
				if (nValue>MAX_NUM_FRAME){
					printf("Non e' possibile inserire piu' di 6 foto\n\n");
					return -1;
				}
				else
					if ((nValue<MIN_NUM_FRAME) & (nValue!=0)){
						printf("Non e' possibile inserire meno di 2 foto\n\n");
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

	c=0;
//controlliamo di aver messo tutti i parametri
	if (nValue==0){
		printf("Non e' stato inserito un numero di foto valido.\nUseremo il valore di default n=\e[36m2\e[0m\n");
		nValue=2;
		c=1;
	}
	if (tValue==NULL){
		printf("Non e' stato inserito un nome valido per l'estensione della foto prodotta.\nUseremo l'estensione di default \e[36m\"png\"\e[0m\n");
		tValue="png";
		c=1;
	}
	if (oValue==NULL){
		printf("Non e' stato inserito un nome valido per la foto prodotta.\nUseremo il nome di default \e[36m\"collageMakerOutput\"\e[0m\n");
		oValue="collageMakerOutput";
		c=1;
	}
//se un parametromancava stampiamo il formato corretto
	if (c==1)
		printf("\nFormato comando:\e[36m ./collageMaker  [-n num] [-t extension] [-o output]\e[0m\n");

	(*myLayout).number=nValue;
	(*myLayout).extension=tValue;
	(*myLayout).outputFileName=oValue;
	
	return 0;
}


void printSummary(struct layout* myLayout){
	
	printf("Summary\n");
	printf("\tNumero di foto da stampare:\t%i\n", (*myLayout).number);
	printf("\tNome del file finale:\t%s.%s\n", (*myLayout).outputFileName, (*myLayout).extension);

	int i;
	for (i= 0; i<(*myLayout).number; i++){
		printf("i %i\n",i);
		//char *temp=NULL;
		//sprintf(temp, "%d", i);
		char* out=malloc( strlen((*myLayout).outputFileName)+strlen((*myLayout).extension)+2);
		out= strcpy(out, (*myLayout).outputFileName);
		//out = strcat(out,temp) ;
		out = strcat(out,".") ;
		out = strcat(out, (*myLayout).extension);

		printf("out %s\n", out);

		vips_image_write_to_file (((*myLayout).arrayOfImages[i]).image, out, NULL);
	}
}



int setLayout ( struct layout* myLayout, int size)
{
	return 1;
}


int printASCIIArt(int numPhoto){
	/*switch (numPhoto)
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
	
	}*/
	return 0;
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

	return 0;
}


