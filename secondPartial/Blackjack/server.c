/*

    LUCÍA GARZA 
    A01361235
    05/03/2017 
    
    PROGRAM WITH A SERVER AND A CLIENT PROGRAM 
    THAT IMITATES A GAME OF BLACKJACK 
    THAT CAN BE PLAYED ON DIFFERENT COMPUTERS
*/

/*SERVER PROGRAM*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

// Include libraries for sockets
#include <netdb.h>
#include <arpa/inet.h>

#define SERVICE_PORT 8642
#define MAX_QUEUE 5
#define BUFFER_SIZE 1024

void usage(char * program);
void startServer(char * port);
void waitForConnections(int server_fd);
void communicationLoop(int connection_fd, int player);

int main(int argc, char * argv[])
{
    
    printf("\n<<<<<<<WELCOME TO LUCIA´S BLACKJACK>>>>>\n");

    if (argc != 2)
        usage(argv[0]);

    startServer(argv[1]);

    return 0;
}

// Show the user how to run this program
void usage(char * program)
{
    printf("Usage:\n%s {port_number}\n", program);
    exit(EXIT_FAILURE);
}

// Initialize the server to be ready for connections
void startServer(char * port)
{
    struct addrinfo hints;
    struct addrinfo * server_info;
    int server_fd;

    // Prepare the information to determine the local address
    // Use internet sockets with IPv4
    hints.ai_family = AF_INET;
    // Use connected sockets
    hints.ai_socktype = SOCK_STREAM;
    // Determine the ip of this machine automatically
    hints.ai_flags = AI_PASSIVE;

    ///// GETADDRINFO
    // Get the actual address of this computer
    if ( getaddrinfo(NULL, port, &hints, &server_info) == -1 )
    {
        perror("ERROR: getaddrinfo");
        exit(EXIT_FAILURE);
    }

    ///// SOCKET
    // Use the information obtained by getaddrinfo
    server_fd = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
    if ( server_fd == -1 )
    {
        perror("ERROR: socket");
        exit(EXIT_FAILURE);
    }

    ///// BIND
    // Connect with the desired port
    if ( bind(server_fd, server_info->ai_addr, server_info->ai_addrlen) == -1 )
    {
        perror("ERROR: bind");
        exit(EXIT_FAILURE);
    }

    // Release the memory of the list of addrinfo
    freeaddrinfo(server_info);

    ///// LISTEN
    // Prepare for connections
    if ( listen(server_fd, MAX_QUEUE) == -1 )
    {
        perror("ERROR: listen");
        exit(EXIT_FAILURE);
    }

    printf("Dealer ready and waiting for players!\n");
    // Stand by to receive connections from the clients
    waitForConnections(server_fd);

    // Close the socket
    close(server_fd);
}

// Stand by for connections by the clients
void waitForConnections(int server_fd)
{
    struct sockaddr_in client_address;
    socklen_t client_address_size;
    char client_presentation[INET_ADDRSTRLEN];
    int connection_fd;
    int player = 0;


    // Loop to wait for client connections
    while (1)
    {
        ///// ACCEPT
        // Receive incomming connections
        // Get the size of the structure where the address of the client will be stored
        client_address_size = sizeof client_address;
        // Receive the data from the client and open a new socket connection
        connection_fd = accept(server_fd, (struct sockaddr *) &client_address, &client_address_size);
        if ( connection_fd == -1 )
        {
            perror("ERROR: accept");
            exit(EXIT_FAILURE);
        }

        // Identify the client
        // Get the ip address from the structure filled by accept
        inet_ntop(client_address.sin_family, &client_address.sin_addr, client_presentation, sizeof client_presentation);
        printf("Received connection from: %s : %d\n", client_presentation, client_address.sin_port);

        pid_t pid;
        pid = fork();       //create threads
        printf("Creating new thread...\n");
        player++;
        printf("Entering the game player number %d\n", player); //new player enters the game 
        if (pid < 0)
            printf("\nFork Error\n");
        if (pid == 0)  {
            communicationLoop(connection_fd, player);  //target thread workout
            exit(0);
        }
        else close(connection_fd);   //just kick out
     }
    
}

// Do the actual receiving and sending of data
void communicationLoop(int connection_fd, int player)
{
    char buffer[BUFFER_SIZE] ;
    int dealerCards[MAX_QUEUE];
    int playerCards[MAX_QUEUE];
    int i;
    int choice, p_choice, result;
    int rounds = 0;
    int delearsTotal = 0;
    int playerTotal = 0;
    int GAME = 1;
    int noPlayers = 0;
    int d;

    //Initialize dealer's firs card         
   for (i = 0; i < rounds + 1; i++ ) {
        int r = rand() % 11 + 1;  //random card between 0 and 12
        dealerCards[i] = r+1;
   }

   
   srand ( time(NULL) );
   //Initialize player's first two cards         
    for (i = 0; i < rounds + 2; i++ ) {
        int r = rand() % 11 + 1;  //random card between 0 and 12
        playerCards[i] = r+1;
   }

    write(connection_fd, &player, 4);
    printf("Delears card is:\n" );

    for (i = 0; i < rounds + 1; i++ ) {
        printf("Card %d = %d\n", i+1, dealerCards[i] ); //prints dealer first card
        delearsTotal += dealerCards[i];
    }

    for (i = 0; i < rounds + 1; i++ ) {
        write(connection_fd, &dealerCards[i], 4); //send dealers cards to player
    }

    printf("Player number %d cards are:\n", player );

    for (i = 0; i < rounds + 2; i++ ) {
        printf("Card %d = %d\n", i+1, playerCards[i] ); //prints player first two cards
        playerTotal += playerCards[i];
    }

    for (i = 0; i < rounds + 2; i++ ) {
        write(connection_fd, &playerCards[i], 4); //send player it's cards
    }

    write(connection_fd, &playerTotal, 4); //sends player their total 

    while(GAME == 1)
    {
        read(connection_fd, &p_choice, 4); //receive choice from player to stand or hit 

        if(p_choice == 1) //player hits
        {
            rounds++; //keep track of number of rounds player has played
            printf("Player %d requested another card\n", player);
            //generating a new card for player
            int r = rand() % 11 + 1; 
            playerCards[rounds + 2]  = r;
    
            write(connection_fd, &playerCards[rounds + 2], 4); //send player new card

            read(connection_fd, &result, 4); //receives code from player to check result
            if(result == 2)
            {
                printf("Player %d BUSTED. Cards total is more than 21.\n", player); //player has more than 21
            }
            if(result == 1)
            {
                printf("Player %d has BLACKJACK. Cards total is equal to 21.\n", player); //player has blackjack
                result = 1;
                write(connection_fd, &result, 4);
                GAME = 0;
                break;
            }
            
        }

        else{ //player stands
            read(connection_fd, &playerTotal, 4);
            printf("Player %d requested to stand with a total of: %d\n", player, playerTotal); //prints player total
        }

        printf("Have players finish their play? (YES = 1 / NO = 0)\n"); //dealer has to declare if all players have finish playing to continue to next round
        scanf("%d", &d);
        if(d == 1)
            GAME = 0; //end players game
        else{ 
            write(connection_fd, &d, 4);
            continue;
        }
           
    }


    GAME = 1;
    ////
    //Time for the dealer to play
    rounds = 0;
    while(GAME == 1)
    {
        printf("Your total is: %d\n", delearsTotal);
        printf("HIT or STAND? (HIT = 1 /STAND  = 0)\n "); //delaer wants to stand or hit?
        scanf("%d", &choice);
        if(choice == 1) //dealer hits
        {    
            //generating new card for dealer 
            int r = rand() % 11 + 1; 
            dealerCards[rounds + 1]  = r;
            printf("You requested another card, here it is: %d\n", dealerCards[rounds + 1] );
            printf("Your cards are:\n" );
            for (i = 0; i <= rounds + 1; i++ ) {
                printf("Card %d = %d\n", i+1, dealerCards[i] ); //prints dealer cards
            }

            delearsTotal += dealerCards[rounds + 1]; //add new card to total 
            rounds++; //keep track of rounds played by dealer 
            if(delearsTotal > 21) //dealer looses
            {   
                printf("Your total is: %d\n", delearsTotal);
                printf("BUSTED. Cards total is more than 21.\n");
                result = 1;
                write(connection_fd, &result, 4);
                break;
            }
            if(delearsTotal == 21) //dealer has blackjack
            {   
                printf("Your total is: %d\n", delearsTotal);
                printf("BLACKJACK. Cards total is equal to 21.\n");
               
                break;
            }

        }
        else //dealer stands
        {
            printf("You decided to stand with a total of: %d\n", delearsTotal );
            //
            if((playerTotal > delearsTotal && playerTotal <= 21) || delearsTotal > 21)
                printf("Player %d wins\n", player );
            else if (playerTotal > 21)
                printf("You lost\n" );
            else if(delearsTotal > playerTotal && delearsTotal <= 21)
                printf("House wins\n");
            else if(playerTotal == delearsTotal)
                printf("TIE\n");
            break;
        }
    
    }

    //Time to see who the winner is
     if((playerTotal > delearsTotal && playerTotal <= 21) || delearsTotal > 21)
        printf("Player %d wins\n", player );
    else if (playerTotal > 21)
        printf("You lost\n" );
    else if(delearsTotal > playerTotal && delearsTotal <= 21)
        printf("House wins\n");
    else if(playerTotal == delearsTotal)
        printf("TIE\n");
    
    printf("GAME OVER\n");
    printf("\n");
    close(connection_fd);

}
