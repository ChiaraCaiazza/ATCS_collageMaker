collageMaker:	collageMaker.c ioUtils.c layout.c imageInfo.c
	gcc -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0 -lconfig -Wall collageMaker.c ioUtils.c layout.c imageInfo.c `pkg-config --cflags glib-2.0` `pkg-config --libs glib-2.0` `pkg-config vips --cflags --libs` -o collageMaker
