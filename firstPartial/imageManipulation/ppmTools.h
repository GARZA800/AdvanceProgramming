#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RGB 255

typedef struct {
     char red,green,blue;
} PPMPixel;

typedef struct {
     int x, y;
     PPMPixel *data;
} PPMImage;

static PPMImage *readPPM(const char *filename); //function to read ppm file
void writePPM(const char *filename, PPMImage *img); //fuction to write a ppm file
