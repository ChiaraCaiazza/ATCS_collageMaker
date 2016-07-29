#define MAX_NUM_PHOTO 6
#define MIN_NUM_PHOTO 2

#ifndef COLLAGEMAKER_H_
#define COLLAGEMAKER_H_
struct position{
	int x;
	int y;
};

struct colour{
	int r;
	int g;
	int b;
};

struct image{
	struct position pos;
	struct position dim;
	int rot;
};

struct layout {
	struct image photo[100];
	struct position layoutDim;
	int number;
	struct colour backgroundColor;
	char* extension;
	char* outputFileName;
};


#endif