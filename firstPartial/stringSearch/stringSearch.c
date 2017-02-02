/*
    LUCÍA GARZA ESCOBAR 
    A01361235 
    21/01/2017
    
    PROGRAM THAT COUNTS HOW MANY TIMES A WORD APPEARS INSIDE OF A SENTENCE
    
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 100

int getLength(char *n);
void searchWord(char* sentence, char* word);

int main()
{
    printf("Enter a sentence: ");
    char sentence[MAX_LENGTH]; //gets the sentence enter by the user
    scanf("%[^'\n']s", sentence);
    
    printf("Enter a word: ");
    char word[MAX_LENGTH]; //gets the word enter by the user
    scanf("%s", word);
    
    searchWord(sentence, word); //function to search the word inside the sentence
     
    return 0;
}

int getLength(char *n) //function to get the length from each word in the sentence as well as the length of the word given
{
    int length = 0;
    
    while(*n != '\0') //counts length until a space is found
    {
        length++;
        n++;
    }
    return length; //return length of word
}

void searchWord(char *sentence, char* word)
{
    int i, j = 0; //declaration of variables
    int counter = 0;
    
    for(i = 0; i < getLength(sentence); i++) 
    {
        if(sentence[i] == word[j]) //compares the word of the sentence with the word given
        {
            if(j == getLength(word) - 1) //checks if word is over, if it is counter increases by one
            {
                counter++;
                j = 0; //restart word
            }
        }
        else 
        {
            j = 0; //restart word
        }
        j++;
    }
    printf("Word '%s' found: %d times \n",word, counter);
}