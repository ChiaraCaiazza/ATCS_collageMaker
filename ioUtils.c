#include <getopt.h>

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "ioUtils.h"
#include "layout.h"
#include "imageUtils.h"


int extractExtension (char*);
int chooseLayout(struct collage_t*, int);
int chooseColor (struct collage_t*);
int chooseFiles (struct collage_t*);
void takeRGB (struct collage_t*);
void takeSingleValue(int*);


int retrieveInput(struct collage_t* myCollage, int size){
	int ret;

	printf ("\e[36;1m\nBenvenuto in collage maker\e[0m\n\nPremere un tasto per visualizzare i layout disponibili per %i foto\n", myCollage->num_images);
	getchar();
	
	ret=chooseLayout(myCollage, size);	
	if (ret<0)
		return -1;

	ret=chooseColor(myCollage);	
	if (ret<0)
		return -1;

	printf("\nQuesti sono i tuoi valori RGB:\t%i-%i-%i\n", myCollage->backgroundColour.r, myCollage->backgroundColour.g, myCollage->backgroundColour.b);

	myCollage->images = (VipsImage**)malloc(sizeof(VipsImage*) * myCollage->num_images);

	ret=chooseFiles(myCollage);	
	if (ret<0)
		return -1;
	
	return 0;
}


int scanInputValue (int argc, char** argv, struct collage_t* myCollage, int collage_size){	
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
				if ((strcmp(tValue,"png")!=0) && (strcmp(tValue,"jpg")!=0)){
					printf("\e[36m%s\e[0m non e' un nome valido per l'estensione della foto prodotta.\nUseremo l'estensione di default \e[36m\"png\"\e[0m\n", tValue);
					tValue="png";
				}
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
//se un parametro manca, stampiamo il formato corretto
	if (c==1)
		printf("\nFormato comando:\e[36m ./collageMaker  [-n num] [-t extension] [-o output]\e[0m\n");

	myCollage->num_images = nValue;
	myCollage->extension = tValue;
	myCollage->outputFileName = oValue;
	
	return 0;
}


void printSummary(struct collage_t* myCollage){
	printf("Summary\n");
	printf("\tNumero di foto da stampare:\t%i\n", myCollage->num_images);
	printf("\tNome del file finale:\t%s.%s\n", myCollage->outputFileName, myCollage->extension);
	printf("\tRGB:\t%i-%i-%i\n",myCollage->backgroundColour.r, myCollage->backgroundColour.g, myCollage->backgroundColour.b);
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


int chooseLayout(struct collage_t * myCollage, int size){
	int num_layouts = get_num_layouts(myCollage->num_images);

	print_layouts(myCollage->num_images);

	while(1)
	{
		printf("Scegli uno dei layout disponibili: \n");
		char layout_id = tolower(getchar());
		if(layout_id != '\n')
		{
			printf("Layout scelto: %c\n", layout_id);
			int layout_index = layout_id - 'a';
			if( layout_index < 0 || layout_index > num_layouts)
			{
				printf("ERROR! Type a letter between 'a' and '%c' \n", 'a' + num_layouts - 1);
			}
			else
			{
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

	//RGBArray=malloc((sizeof (int)*3*9)+1);
	while(1)
	{
		printf("Scegli uno tra i sequenti colori o premi p per inserire il tuo RGB (q per terminare)\n");
		RGBArray = printColor();
		if (RGBArray==NULL)
			return -1;
		
		scanf ("%s", colorID);
		if (strcmp(colorID,"p") == 0){
			printf("Inserire un RGB\n");
			takeRGB(myCollage);
			break;
		}

		if (strcmp(colorID,"q") == 0){
			printf("\e[91m\nOperazione terminata\n\n\e[0m");
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

	printf("\nHai inserito i seguenti RGB%i-%i-%i\n", R, G, B);

	myCollage->backgroundColour.r=R;
	myCollage->backgroundColour.g=G;
	myCollage->backgroundColour.b=B;
	
}

void takeSingleValue(int* value){
	while (1)
	{
		scanf("%i",value );
		printf("\n");
		
		if ((*value<0)||(*value>255)){
			printf("Inserire un valore tra 0 e 255\n");
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
	while(i != myCollage->num_images){
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
 		
		myCollage->images[i] = vips_image_new_from_file (photoName, NULL);
		i++;
	}

	return 1;
}

