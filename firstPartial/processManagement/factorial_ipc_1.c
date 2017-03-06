/*
    Program to compute the factorial of different numbers
    The program will create another process, that will do the calculations
    The parent process will hear requests by the user
    Uses the 'pipe' function for communication

    Gilberto Echeverria
    gilecheverria@yahoo.com
    12/02/2017
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

///// FUNCTION DECLARATIONS /////
void createProcess();
void userMenu(FILE * fp_out, FILE * fp_in);
void attendRequests(FILE * fp_out, FILE * fp_in);
void openPipe(int pipe_channel[]);
void preparePipes(int pipe_out[], int pipe_in[], FILE ** fp_out, FILE ** fp_in);
void closePipes(int pipe_out[], int pipe_in[], FILE ** fp_out, FILE ** fp_in);
unsigned long int factorial(int number);

int main()
{
    printf("\nPROGRAM TO COMPUTE FACTORIALS\n");
    printf("Only one child process to find the factorials\n");

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
    // Variables for the file pointers
    FILE * fp_in = NULL;
    FILE * fp_out = NULL;

    // Open the two channels
    openPipe(pipe_parent_child);
    openPipe(pipe_child_parent);

    // Create only one fork, the child will do the calculations
    pid = fork();

    // Parent process
    if (pid > 0)
    {
        // Open the corresponding pipes
        preparePipes(pipe_parent_child, pipe_child_parent, &fp_out, &fp_in);
        // Start the loop to hear user requests and send them to child
        userMenu(fp_out, fp_in);
        // Close the pipes
        closePipes(pipe_parent_child, pipe_child_parent, &fp_out, &fp_in);
        printf("Parent finishing\n");
    }
    // Child process
    else if (pid == 0)
    {
        // Open the corresponding pipes
        preparePipes(pipe_child_parent, pipe_parent_child, &fp_out, &fp_in);
        // Start the loop to hear user requests and get the factorial
        attendRequests(fp_out, fp_in);
        // Close the pipes
        closePipes(pipe_child_parent, pipe_parent_child, &fp_out, &fp_in);
        printf("Child finishing\n");
    }
    // No process created
    else
    {
        perror("Unable to create a new process\n");
        exit(EXIT_FAILURE);
    }
}

// Main loop for the parent process, listens for the user requests
// Receive: the file pointers to write and read to the child
void userMenu(FILE * fp_out, FILE * fp_in)
{
    int number = 0;
    unsigned long int result = 0;

    while (number >= 0)
    {
        printf("\nEnter a number to compute its factorial (enter a negative number to finish): ");
        scanf("%d", &number);
        // Send the number to the child, and a new line
        fprintf(fp_out, "%d\n", number);
        // Write immediately to the stream
        fflush(fp_out);

        // Compute factorials for positive numbers and 0 only
        if (number >= 0)
        {
            // Get the reply from the child
            fscanf(fp_in, "%lu", &result);
            printf("The result sent by child is %lu\n", result);
        }
    }
}

// Loop for the child process to wait for requests from the parent
// Receive: the file pointers to write and read to the parent
void attendRequests(FILE * fp_out, FILE * fp_in)
{
    int number = 0;
    unsigned long int result = 0;

    while (1)
    {
        // Listen for requests from the parent
        fscanf(fp_in, "%d", &number);
        // Finish with a negative number
        if (number < 0)
        {
            break;
        }
        result = factorial(number);
        // Send the result back to the parent, with a new line
        fprintf(fp_out, "%lu\n", result);
        // Write immediately to the stream
        fflush(fp_out);
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

// Close the pipe directions that are not necessary, and open file pointers
// Receive the file descriptors and pointers to the file pointers
void preparePipes(int pipe_out[], int pipe_in[], FILE ** fp_out, FILE ** fp_in)
{
    // Close the unwanted channels
    close(pipe_in[1]);
    close(pipe_out[0]);

    // Open the file pointers
    *fp_out = fdopen(pipe_out[1], "w");
    if (*fp_out == NULL)
    {
        perror("Unable to open pipe for writing\n");
        exit(EXIT_FAILURE);
    }
    *fp_in = fdopen(pipe_in[0], "r");
    if (*fp_in == NULL)
    {
        perror("Unable to open pipe for reading\n");
        exit(EXIT_FAILURE);
    }
}

// Close the remaining pipes and file pointers
// Receive the file descriptors and pointers to the file pointers
void closePipes(int pipe_out[], int pipe_in[], FILE ** fp_out, FILE ** fp_in)
{
    // Close the file pointers
    fclose(*fp_in);
    fclose(*fp_out);
    // Close the file descriptors
    close(pipe_in[0]);
    close(pipe_out[1]);
}

// Compute the factorial of a number
unsigned long int factorial(int number)
{
    unsigned long int result = 1;
    for (; number > 0; number--)
    {
        result *= number;
    }
    return result;
}
