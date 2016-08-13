

#ifndef IOUTILS_H_
#define IOUTILS_H_

#include "collageMaker.h"
#include <ctype.h>


int retrieveInput (struct collage_t* , int );
int scanInputValue (int, char**, struct collage_t* , int);
void printSummary(struct collage_t*);

#endif
