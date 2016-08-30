#ifndef IOUTILS_H_
#define IOUTILS_H_

#include "collageMaker.h"
#include <ctype.h>

//utility functions
int extractExtension (char*);
int chooseLayout(struct collage_t*, int);
int chooseColor (struct collage_t*);
int chooseFiles (struct collage_t*);
void takeRGB (struct collage_t*);
void takeSingleValue(int*);
void printSummary(struct collage_t*);

//the function parses the command line arguments passed by the user
int scanInputValue (int, char**, struct collage_t*, int);

//the function sets the parameters needed to create the collage
int retrieveInput (struct collage_t*, int);

#endif
