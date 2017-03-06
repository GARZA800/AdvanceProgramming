/*
   Example program to show the use of mkfifo to open a file
   This program will read from the fifo

   Gilberto Echeverria
   gilecheverria@yahoo.com
   09/02/2017
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

#define BUFFER_SIZE 255

void readFifo();

int main()
{
    printf("\nPROGRAM TO TEST FIFOS (read)\n");

    readFifo();

    return 0;
}

void readFifo()
{
    FILE * file_ptr = NULL;
    char * fifo_name = "fifo";
    char buffer[BUFFER_SIZE];

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
    file_ptr = fopen(fifo_name, "r");
    // Check the file
    if (file_ptr == NULL)
    {
        perror("Could not open the fifo file\n");
        exit(EXIT_FAILURE);
    }
    
    // Get data into the fifo
    while ( fgets(buffer, BUFFER_SIZE, file_ptr) )
    {
        printf("%s", buffer);
    }

    fclose(file_ptr);
}
