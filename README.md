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

####Install Dependencies
1. Use a pkg manager like apt-get(Linux-like) or brew(Mac OS)
```
apt-get install libconfig
apt-get install vips
```
2. Compile and install the libraries inside "lib" directory.
Change the current directory to the desired library and execute the following commands
```
./configure
make
make install
```

###Execution:

```
./collageMaker [-n num (default=2)] [-t string (default=null)] [-o string (default=null)]
```