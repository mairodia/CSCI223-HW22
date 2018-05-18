// ==================================================================
// File: client03.c (Spring 2018)
// ==================================================================
// Programmer: Jennifer King
// Date: 5/17/2018
// Project: Programming with Unix Sockets
// Professor: Scott Edwards
// Class: CSCI 223 F | TR 4:30PM
// Description::
//      The server does the following:
//      -- delete the local socket file, if it exists
//              -- create a socket
//              -- bind the socket to the local socket file
//              -- put the socket in a listening state
//              -- wait for a client connection
//
//      Once a client connects, the server recieves a string message
//      from the client and writes it to stdout. Then the server
//      closes the conncetion and shuts down.
// ==================================================================

#include    <sys/types.h>
#include    <sys/socket.h>
#include    <stdio.h>
#include    <stdlib.h>
#include    <sys/un.h>
#include    <unistd.h>
#include    <string.h>
#include    <netinet/in.h>

#define LEN             256
#define SOCKET_NAME     "server_socket"
#define PORT                    50011

// == main ==========================================================
//
// ==================================================================
int     main(void)
{
    auto        int                     socketID;
    auto        struct sockaddr_in      client;
    auto        char                    buf[LEN];
    auto        char*                   result;

    //Create an AF_UNIX/SOCK_STREAM socket
    socketID = socket(AF_INET, SOCK_STREAM, 0);

    //zero out structure
    memset(&client, 0, sizeof(client));

    //initialize struct members
    client.sin_family = AF_INET;
    client.sin_port = htons(PORT);
    client.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if(connect(socketID, (struct sockaddr*)&client, sizeof(client)))
    {
        perror("client -- connect failed");
        exit(EXIT_FAILURE);
    }

    //get string of text from user
    while(1)
    {
        printf("Enter a string of text: ");
        result = fgets(buf, LEN, stdin);
        if(*result == -1)
        {
            perror("client -- fgets failed");
            exit(EXIT_FAILURE);
        }
        else if((NULL == result) || ('\n' == *result))
        {
            continue;
        }
        else
        {
            //send user text
            send(socketID, buf, LEN, 0);

            //if "quit"
            if(0 == strcmp(buf, "quit\n"))
            {
                close(socketID);
                break;
            }
        }
    }

    return 0;
}
