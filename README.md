#CollageMaker
CollageMaker is a free image processing system to create collages of photos. 
It has multiple layouts to choose from and many colours for the background, 
and supports the most used image formats (PNG, JPEG and GIF).


###Dependencies
1. [libconfig 1.6](https://github.com/hyperrealm/libconfig)
2. [libvips 8.4](https://github.com/jcupitt/libvips)
	 * glib2.0-dev
	 * libpng
	 * libjpeg
	 * libgif

###Execution:

```
./collageMaker [-n num (default=2)] [-t string (default=null)] [-o string (default=null)]
```