/*
   Library with common functions to work with strings and files
   
   Gilberto Echeverria
   gilecheverria@yahoo.com
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRING_SIZE 50

FILE * openAndCheck(char file_name[], char mode[]);
void clearStringEnter(char string[]);
void clearBufferEnter();
void getClean(char string[]);
