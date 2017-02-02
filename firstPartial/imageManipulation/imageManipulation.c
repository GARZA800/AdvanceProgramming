/*
    LUCÍA GARZA ESCOBAR 
    A01361235 
    01/02/2017
    
    PROGRAM THAT READS A PPM FILE, TRANSFORM THE IMAGE TO NEGATIVE AND WRITES A PPM FILE. 
    
*/

#include "ppmTools.c"

//Function declaration
void negativePPM(PPMImage *img);

int main(){
    PPMImage *image;
    image = readPPM("skull-icon.ppm");
    negativePPM(image);
    writePPM("n_skull-icon.ppm",image);
    printf("PPM file created succesfully\n");
}

void negativePPM(PPMImage *img)
{
    int i;
    if(img){

         for(i=0;i<img->x*img->y;i++){
              img->data[i].red=RGB-img->data[i].red;
              img->data[i].green=RGB-img->data[i].green;
              img->data[i].blue=RGB-img->data[i].blue;
         }
    }
}

