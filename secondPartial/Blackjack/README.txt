/*

    LUCÍA GARZA 
    A01361235
   05/03/2017
    
    PROGRAM WITH A SERVER AND A CLIENT PROGRAM 
    THAT IMITATES A GAME OF BLACKJACK 
    THAT CAN BE PLAYED ON DIFFERENT COMPUTERS
*/

/*README FILE*/

The program includes two files, the server.c file which acts as the dealer. And the client.c file which acts as the player.

The program begins with dealer waiting for players to connect, once all the players are in the dealer gives the cards. Then the game begins, each player gets to decide if they hit or stand, after they take this decision every player will have to wait for the dealer to continue the game and make the decision again.
The dealer has to wait for all players to make the decision of stand or hit, after this he will have to answer to the question "Have players finish their play?" as many times as current players are in the game. (if a player has been busted, he's not a current player anymore)
After all players are either busted or standing dealer will answer that players have finish their play and then it's the dealers turn to decide if he wants to stand or hit. When dealer is either busted or standing, the results of who won is displayed. 
