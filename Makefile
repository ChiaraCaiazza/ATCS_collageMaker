CFLAGS = -c -Wall
LDFLAGS = -lconfig

CFLAGS += `pkg-config --cflags vips`
LDFLAGS += `pkg-config --libs vips`


collageMaker: collageMaker.o ioUtils.o layout.o imageUtils.o
		cc -o collageMaker collageMaker.o ioUtils.o layout.o imageUtils.o $(LDFLAGS)
		
collageMaker.o: collageMaker.c collageMaker.h ioUtils.h layout.h imageUtils.h
		cc $(CFLAGS) collageMaker.c
		
ioUtils.o: ioUtils.c ioUtils.h layout.h imageUtils.h
		cc $(CFLAGS) ioUtils.c
		
layout.o: layout.c layout.h
		cc $(CFLAGS) layout.c
		
imageUtils.o: imageUtils.c imageUtils.h
		cc $(CFLAGS) imageUtils.c

