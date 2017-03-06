/*
   Example program to show the use of mkfifo to open a file
   This program will write to the fifo

   Gilberto Echeverria
   gilecheverria@yahoo.com
   09/02/2017
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

#define BUFFER_SIZE 255

void writeFifo();
unsigned long int fibonacci(unsigned int num);

int main()
{
    printf("\nPROGRAM TO TEST FIFOS\n");

    writeFifo();

    return 0;
}

void writeFifo()
{
    FILE * file_ptr = NULL;
    char * fifo_name = "fifo";

    // Check if the fifo file already exists
    if(access(fifo_name, F_OK) == -1 )
    {
        // Create the fifo
        if ( mkfifo(fifo_name, 0666) == -1)
        {
            perror("Could not create the fifo file\n");
            exit(EXIT_FAILURE);
        }
    }
     
    // Open the file for writing
    file_ptr = fopen(fifo_name, "w");
    // Check the file
    if (file_ptr == NULL)
    {
        perror("Could not open the fifo file\n");
        exit(EXIT_FAILURE);
    }

    // Write data into the fifo
    for (int i=1; i<=20; i++)
    {
        fprintf(file_ptr, "f(%d) = %lu\t", i, fibonacci(i));
        if (i%5 == 0)
            fprintf(file_ptr, "\n");
    }

    fclose(file_ptr);
}

unsigned long int fibonacci(unsigned int num)
{
    unsigned long int a = 0;
    unsigned long int b = 1;
    unsigned long int c;

    if (num < 2)
        return num;

    for (int i=2; i<=num; i++)
    {
        c = a + b;
        a = b;
        b = c;
    }
    return c;
}
