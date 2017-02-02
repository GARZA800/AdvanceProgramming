#include "ppmTools.h"

static PPMImage *readPPM(const char *filename)
{
         char buff[16];
         PPMImage *img;
         FILE *fp;
         int rgb_color;
         //open and reads PPM file
         fp = fopen("skull-icon.ppm", "rb");
         if (!fp) {
              fprintf(stderr, "Unable to open file '%s'\n", filename);
              exit(1);
         }

    //read image format
     if (!fgets(buff, sizeof(buff), fp)) {
          perror(filename);
          exit(1);
     }

    //checkfor P6 format
    if (buff[0] != 'P' || buff[1] != '6') {
         fprintf(stderr, "ERROR. Invalid image format, the image is not P6\n");
         exit(1);
    }

    //alloc memory form image
    img = (PPMImage *)malloc(sizeof(PPMImage));

    //read image size 
    if (fscanf(fp, "%d %d", &img->x, &img->y) != 2) {
         fprintf(stderr, "ERROR");
         exit(1);
    }

    //read rgb components
    if (fscanf(fp, "%d", &rgb_color) != 1) {
         fprintf(stderr, "ERROR");
         exit(1);
    }

    while (fgetc(fp) != '\n') ;
    //memory allocation for pixel data
    img->data = (PPMPixel*)malloc(img->x * img->y * sizeof(PPMPixel));


    //read pixel data
    if (fread(img->data, 3 * img->x, img->y, fp) != img->y) {
         fprintf(stderr, "Error loading image '%s'\n", filename);
         exit(1);
    }

    fclose(fp); //close file
    return img;
}


void writePPM(const char *filename, PPMImage *img)
{
    FILE *fp;
    //open file for writing
    fp = fopen("n_skull-icon.ppm", "wb");
    if (!fp) {
         fprintf(stderr, "Unable to open file '%s'\n", filename); //error message
         exit(1);
    }

    //write image format
    fprintf(fp, "P6\n");

    //writes image size
    fprintf(fp, "%d %d\n",img->x,img->y);

    //writes rgb component depth
    fprintf(fp, "%d\n",RGB);

    //writes pixel data
    fwrite(img->data, 3 * img->x, img->y, fp);
    fclose(fp);
}