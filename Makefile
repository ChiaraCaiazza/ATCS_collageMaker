collageMaker:	collageMaker.c ioUtils.c layout.c imageInfo.c
	gcc -lconfig -Wall collageMaker.c ioUtils.c layout.c imageInfo.c `pkg-config vips --cflags --libs` -o collageMaker
