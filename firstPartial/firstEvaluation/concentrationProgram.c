/*
	Name: LUCÍA GARZA 
	ID number: A01361235

    TC2025 Advanced Programming
    First Partial Evaluation
    Evaluates:
        - Pointers
        - Dynamic memory
        - Process management
        - Inter Process Communication

    Program to test concentration counting exercise

    The problem:
    There is an exercise to develop concentration, where two numbers are chosen,
    the first will count up from 0, and the second counts down from 100.
    The player must keep counting in his head, adding and substracting each time
    from both sides. Example, in the range 0-100, adding 3 and substracting 4:
     3 - 96
     6 - 92
     9 - 88
    12 - 84
    15 - 80
    etc.

    You must write a program that can do this calculations, but the increment
    must be done in one process and the decrement in a different process.
    The parent will be the one who decides when to stop, so the two processes
    must keep constant communication. The child will send the updated value
    to the parent, and the parent will reply with a boolean value indicating
    whether the child should continue or it has finished already.
    The program will ask the user for the size of the range, and the amounts
    to add and substract

    You must show visually the progress of these counts, by using a dynamically
    sized string, initially filled with spaces. As the counts progress, the
    string will be filled with the '\' character from left to right, and with
    the '/' character from right to left. If the two counts intersect, you
    should write the 'X' character instead.
    Example, in the range 0-20, adding 4 and substracting 2

		   4 -   18	\\\\              //
		   8 -   16	\\\\\\\\        ////
		  12 -   14	\\\\\\\\\\\\  //////
		  16 -   12	\\\\\\\\\\\\XXXX////
		  20 -   10	\\\\\\\\\\XXXXXXXXXX
		  20 -    8	\\\\\\\\XXXXXXXXXXXX
		  20 -    6	\\\\\\XXXXXXXXXXXXXX
		  20 -    4	\\\\XXXXXXXXXXXXXXXX
		  20 -    2	\\XXXXXXXXXXXXXXXXXX
		  20 -    0	XXXXXXXXXXXXXXXXXXXX

	You must use pointer arithmetic to modify the string. Index notation is not
	allowed for this part of the test.

    QUESTIONS:
    Complete the following three functions to make the program work:
        - countUp
        - countDown
        - drawLine
    The description of what each does is at the end of this file.
    Write your functions there 

    HINTS:
        - Write your name in this comment and in the "main" function
        - Read all of the existing code before answering anything. This will help you
            understand what needs to be done and how to do it
        - Keep calm. Think what you have to do before typing code
        - Write clean code and use correct indentation
        - Add comments to your code
        - You can add any other functions that you need to make your code more organized
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 20

///// FUNCTION DECLARATIONS /////
void createProcess();
void openPipe(int pipe_channel[]);
void preparePipes(int pipe_out[], int pipe_in[]);
void closePipes(int pipe_out[], int pipe_in[]);
void countUp(int pipe_out[], int pipe_in[]);
void countDown(int pipe_out[], int pipe_in[]);
void drawLine(char * line, int range, int count_up, int count_down);
//additional funtions
int increase(int min, int max, int increment);
int decreases(int min, int max, int decrement);

int main()
{
    printf("\n=== PROGRAM FOR CONCENTRATION EXERCISES ===\n");
    printf("By: Lucía Garza\n\n");

    createProcess();

    return 0;
}

///// FUNCTION DEFINITIONS /////

// Function to create the child process and declare the pipe variables
void createProcess()
{
    pid_t pid;
    // Variables for the pipes, indicating the direction of communication
    int pipe_parent_child[2];
    int pipe_child_parent[2];

    // Open the two channels
    openPipe(pipe_parent_child);
    openPipe(pipe_child_parent);

    // Create only one fork, the child will do the calculations
    pid = fork();

    // Parent process
    if (pid > 0)
    {
        // Open the corresponding pipes
        preparePipes(pipe_parent_child, pipe_child_parent);
        // Start the loop to count upwards from 0
        countUp(pipe_parent_child, pipe_child_parent);
        // Close the pipes
        closePipes(pipe_parent_child, pipe_child_parent);
        printf("Parent finishing\n");
    }
    // Child process
    else if (pid == 0)
    {
        // Open the corresponding pipes
        preparePipes(pipe_child_parent, pipe_parent_child);
        // Start the loop to count downwards from the range selected by the user
        countDown(pipe_child_parent, pipe_parent_child);
        // Close the pipes
        closePipes(pipe_child_parent, pipe_parent_child);
        printf("Child finishing\n");
    }
    // No process created
    else
    {
        perror("Unable to create a new process\n");
        exit(EXIT_FAILURE);
    }
}

// Verify the opening of a commuication pipe
void openPipe(int pipe_channel[])
{
    if (pipe(pipe_channel) == -1)
    {
        perror("Unable to create pipe\n");
        exit(EXIT_FAILURE);
    }
}

// Close the pipe directions that are not necessary
// Receive the file descriptors
void preparePipes(int pipe_out[], int pipe_in[])
{
    // Close the unwanted channels
    close(pipe_in[1]);
    close(pipe_out[0]);
}

// Close the remaining pipes
// Receive the file descriptors
void closePipes(int pipe_out[], int pipe_in[])
{
    // Close the file descriptors
    close(pipe_in[0]);
    close(pipe_out[1]);
}

// Main loop for the parent process, does the addition.
// It asks the user for the necessary inputs (range, increment, decrement)
// It passes initiates its variables and passes the variables range and
//  decrement to the child
// Then begins the main loop, where the two processes communicate the
//  updated values
// Receive: the arrays with the file descriptors for input and output
// Return: nothing
void countUp(int pipe_out[], int pipe_in[])
{
    //variable declaration
    int res = 0;
    int res_1 = 0;
    int range_min = 0;
    int range_max = 0;
    int increment = 0;
    int decrement = 0; 

    char buff[BUFFER_SIZE];
    
        printf("\n Enter minimum range: ");
        scanf("%d", &range_min);
 
       
        printf("\n Enter maximum range: ");
        scanf("%d", &range_max);
        
        
        printf("\n Enter decrement variable to substract: ");
        scanf("%d", &decrement);
        
        printf("\n Enter increment variable to add: ");
        scanf("%d", &increment);
        
        int up = range_max;
        int down = range_min;
        int range = range_max - range_min;
        char line[range];
    
    while(up >= range_min && down <= range_max)
    {
        //Send variables to the child 
        sprintf(buff, "%d %d %d", down, range_max, decrement);
        // Write all the characters in the buffer, and also the null character at the end
        write(pipe_out[1], buff, strlen(buff)+1);
     
        //Gets reply from child
        read(pipe_in[0], buff, BUFFER_SIZE);
        sscanf(buff, "%d", &range_max);
        
        //Function that returns the increasing values
        range_min = increase(range_min, up, increment);
        
        //prints results
        printf("%d - %d   ", range_min, range_max);
        
        //memory allocation for string
        char  * line = malloc(range * sizeof(char));
        //function to create string
        drawLine(line, range, range_min, range_max);
        //free
        free(line);
        
        //Program finishes when both variables reach their corresponding values
        if(range_min == up && range_max == down)
        {
            break;
        }
    }

}

// Loop for the child process to update the decrement
// It gets from the parent the values for the range and the decrement
// It then loops, updating the count and listening for a reply from
//  the parent, indicating whether it has finished or not.
// Receive: the arrays with the file descriptors for input and output
// Return: nothing
void countDown(int pipe_out[], int pipe_in[])
{
    //variable declaration
    int down = 0;
    int res = 0;
    int range_max = 0;
    int decrement = 0; 
    char buff[BUFFER_SIZE];

        
    while (1){
       
        //Listen for request from parent
        read(pipe_in[0], buff, BUFFER_SIZE);
        sscanf(buff, "%d %d %d", &down, &range_max, &decrement);
        //Function that returns decreasing values
        range_max = decreases(down, range_max, decrement);
        //Send result back to parent
        sprintf(buff, "%d", range_max);
        //Write characters in the buffer
        write(pipe_out[1], buff, strlen(buff) + 1);
        
    }
        
}

// Draw the line showing the progress of the counts
// Called at every loop
// Will modify the string, adding '\' characters from the left
//  and '/' characters fromt he right. 'X' characters are used
//  when the counts intersect.
// Use only pointer arithmetic to modify the string
// Receives: the string, the size, and the counters for left and right
// Return: nothing
void drawLine(char * line, int range, int count_up, int count_down)
{
    //variables declaration
  int middle = count_down - count_up;
  int middle_1 = count_up - count_down;
  int left = count_up;
  int left_1 = count_down;
  int right = range - count_down;
  int right_1 = range - count_up;
  int i = 0;

  if(middle_1 <0){
    for(i = 0; i<left; i++){
      *(line+i) = '\\';
    }
    for(i = 0; i<middle; i++){
      *(line+left+i) = ' ';
    }
    for(i = 0; i<right; i++){
      *(line+left+middle+i) = '/';
    }
  }
  else{
    for(i = 0; i <left_1; i++){
      *(line+i) = '\\';
    }
    for(i = 0; i<middle_1; i++){
      *(line+left_1+i) = 'X';
    }
    for(i = 0; i<right_1; i++){
      *(line+left+i) = '/';
    }
  }
  //print final string
  printf("%s \n", line );

}

//Increment function
int increase(int min, int max, int increment)
{
    if(min < max){
        min = min + increment;
        return min;
    }
    else 
        return max;
}

//Decrement function
int decreases(int min, int max, int decrement)
{
    if(max > min){
        max = max - decrement;
        return max;
    }
    else 
       return min;
    
}