#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
    printf("CALL TO FORK\n");
    pid_t new_id;
    
    //create a new process
    new_id = fork();
    
    printf("My pid:[%d], my parent id: [%d], new_id: [%d]\n",getpid(), getppid(), new_id);
    
    //the child process
    if(new_id == 0)
    {
       printf("I am the child process\n"); 
    }
    else if(new_id > 0)
    {
       printf("I am the parent of %d\n", new_id); 
    }
    else
    {
       printf("Could not create a new process\n"); 
    }
    
    printf("BACK TO MY PROGRAM\n"); 
    
    return 0;
}