/*
    Program to send signals
    
*/

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int main()
{
    pid_t target;
    
    printf("Enter PID of the target: ");
    scanf("%d", &target);
    
    kill(target, SIGTERM);
    
    return 0;
}