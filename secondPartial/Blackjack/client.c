/*

    LUCÍA GARZA 
    A01361235
    05/03/2017
    
    PROGRAM WITH A SERVER AND A CLIENT PROGRAM 
    THAT IMITATES A GAME OF BLACKJACK 
    THAT CAN BE PLAYED ON DIFFERENT COMPUTERS
*/

/*CLIENT PROGRAM*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <netdb.h>

#define SERVICE_PORT 8642
#define MAX_QUEUE 5
#define BUFFER_SIZE 1024

void usage(char * program);
void connectToServer(char * address, char * port);
void gameCommunication(int connection_fd);

int main(int argc, char * argv[])
{
    printf("\n<<<<<<<WELCOME TO LUCIA´S BLACKJACK>>>>>\n");
    
    if (argc != 3)
        usage(argv[0]);

    connectToServer(argv[1], argv[2]);

    return 0;
}

// Show the user how to run this program
void usage(char * program)
{
    printf("Usage:\n%s {server_address} {port_number}\n", program);
    exit(EXIT_FAILURE);
}

// Establish a connection with the server indicated by the parameters
void connectToServer(char * address, char * port)
{
    struct addrinfo hints;
    struct addrinfo * server_info;
    int connection_fd;

    // Prepare the information to determine the local address
    // Use internet sockets with IPv4
    hints.ai_family = AF_INET;
    // Use connected sockets
    hints.ai_socktype = SOCK_STREAM;
    // Determine the ip of this machine automatically
    hints.ai_flags = AI_PASSIVE;

    ///// GETADDRINFO
    // Get the actual address of this computer
    if ( getaddrinfo(address, port, &hints, &server_info) == -1 )
    {
        perror("ERROR: getaddrinfo");
        exit(EXIT_FAILURE);
    }

    ///// SOCKET
    // Use the information obtained by getaddrinfo
    connection_fd = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
    if ( connection_fd == -1 )
    {
        perror("ERROR: socket");
        exit(EXIT_FAILURE);
    }

    ///// CONNECT
    // Connect with the desired port
    if ( connect(connection_fd, server_info->ai_addr, server_info->ai_addrlen) == -1 )
    {
        perror("ERROR: connect");
        exit(EXIT_FAILURE);
    }

    // Release the memory of the list of addrinfo
    freeaddrinfo(server_info);

    // Establish the communication
    gameCommunication(connection_fd);

    // Close the socket
    close(connection_fd);
}

// Do the actual receiving and sending of data
void gameCommunication(int connection_fd)
{
    char buffer[BUFFER_SIZE];
    int playerCards[MAX_QUEUE];
    int dealerCards[MAX_QUEUE];
    int playerTotal = 0;
    int dealerTotal = 0;
    int p_choice;
    int i, player, d;
    int rounds = 0;
    int result;
    int GAME = 1;
    srand ( time(NULL) );

    read(connection_fd, &player, 4);

    printf("GET READY PLAYER NUMBER %d, GAME IS ABOUT TO START!\n", player);
    sleep(1);

    for (i = 0; i < rounds + 1; i++ ) {
        read(connection_fd, &dealerCards[i], 4); //receive dealers cards
    }
    printf("Dealer card is:\n");
    for (i = 0; i < rounds + 1; i++ ) {
        printf("Card %d = %d\n", i+1, dealerCards[i] );
    }

    for (i = 0; i < rounds + 2; i++ ) {
        read(connection_fd, &playerCards[i], 4); //receive player cards
    }

    printf("Your cards are:\n");
    for (i = 0; i < rounds + 2; i++ ) {
        printf("Card %d = %d\n", i+1, playerCards[i] );
    }

     read(connection_fd, &playerTotal, 4); //reads player total from dealer
     printf("Your total is: %d\n",playerTotal);
     if(playerTotal > 21) //player looses
        {   
            printf("BUSTED. Cards total is more than 21.\n");
            result = 2;
            write(connection_fd, &result,4);
            GAME = 0;
        }
        if(playerTotal == 21) //player has blackjack
        {   
            printf("BLACKJACK. Cards total is equal to 21.\n");
            result = 1;
            write(connection_fd, &result,4);
            GAME = 0;
        }

    while (GAME == 1)
    { 
        printf("HIT or STAND? (HIT = 1 /STAND  = 0) ");
        scanf("\n%d", &p_choice);
        
        if(p_choice == 1) //hit
        {   
            write(connection_fd, &p_choice, 4);   //Sends player choice to stand or hit to dealer
            read(connection_fd, &playerCards[rounds + 2], 4); //receive player card

            //printf("You requested another card, here it is: %d\n", playerCards[rounds + 2] );
            printf("Your cards are:\n" );

            for (i = 0; i < rounds + 3; i++ ) {
                printf("Card %d = %d\n", i+1, playerCards[i] ); //prints player cards
            }

            playerTotal += playerCards[rounds + 2]; //add new card to players total
            printf("Total: %d\n",playerTotal ); //debug

            rounds++; //keeps track of number of rounds played

            if(playerTotal > 21) //player looses
            {   
                printf("BUSTED. Cards total is more than 21.\n");
                result = 2;
                write(connection_fd, &result,4);
                break;
            }
            if(playerTotal == 21) //player has blackjack
            {   
                printf("BLACKJACK. Cards total is equal to 21.\n");
                result = 1;
                write(connection_fd, &result,4);
                break;
            }
            
            write(connection_fd, &playerTotal, 4); //sends player's total to dealer
        }
        else //stand
        {
            write(connection_fd, &p_choice, 4);   //Sends player choice to stand or hit to dealer
            printf("You decided to stand with a total of: %d\n", playerTotal );
            printf("Let's wait for other players to decide\n");
            write(connection_fd, &playerTotal, 4); //sends player's total to dealer
            read(connection_fd, &result, 4);
            GAME = 0;
        }
        printf("Wait for dealer to continue\n"); //wait for the dealer to change round
        read(connection_fd, &d, 4);
        if(d == 0)
            continue;

       
    }

    //checks who the winner is 
    if((playerTotal > dealerTotal && playerTotal <= 21) || dealerTotal > 21)
        printf("Player %d wins\n", player );
    else if (playerTotal > 21)
        printf("House wins\n" );
    else if(dealerTotal > playerTotal && dealerTotal <= 21)
        printf("House wins\n");
    else if(playerTotal == dealerTotal) 
            printf("TIE\n");
    
    printf("Finishing the connection\n");

    
}
