#ifndef IOUTILS_H_
#define IOUTILS_H_

#include "collageMaker.h"
#include <ctype.h>

//the function parses the command line arguments passed by the user
int scan_input(int, char**, struct collage_t*, int);

//the function sets the parameters needed to create the collage
int retrieve_input (struct collage_t*, int);

void print_summary(struct collage_t*);

#endif
