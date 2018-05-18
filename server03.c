// ==================================================================
// File: server03.c (Spring 2018)
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
#include    <ctype.h>
#include        <netinet/in.h>

#define LEN             256
#define SOCKET_NAME             "server_socket"
#define PORT                    50011

// == main ==========================================================
//
// ==================================================================
int     main(void)
{
    auto        char                    buf[LEN];
    auto        int                     server_sockfd;
    auto        int                     client_sockfd;
    auto        socklen_t               client_len;
    auto        ssize_t                 num_client_bytes;
    auto        struct sockaddr_in      server_address;
    auto        struct sockaddr_in      client_address;

    // remove any existing socket, create a new unnamed socket for the server
    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == server_sockfd)
    {
        perror("server -- socket failed");
        exit(EXIT_FAILURE);
    }

    //zero out the sockaddr_in struct
    memset(&server_address, 0, sizeof(server_address));

    // bind the socket to a name
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);

    //check for bind
    if(bind(server_sockfd, (struct sockaddr*)&server_address, sizeof(server_address)))
    {
        perror("server -- bind failed");
        exit(EXIT_FAILURE);
    }

    //create a connection queue and wait for a client
    if(listen(server_sockfd, 5))
    {
        perror("server -- listen failed");
        exit(EXIT_FAILURE);
    }

    //announce server is waiting for client connections
    puts("server waiting...");

    //accept a connection
    client_len = sizeof(client_address);
    client_sockfd = accept(server_sockfd, (struct sockaddr*)&client_address, &client_len);

    //fetch a message from the client and write it to stdout
    while(1)
    {
        num_client_bytes = recv(client_sockfd, buf, LEN, 0);
        if(-1 == num_client_bytes)
        {
            perror("server -- recv failed");
            exit(EXIT_FAILURE);
        }
        else
        {
            if(0 != strcmp(buf, "quit\n"))
            {
                printf("client message: %s", buf);
            }
            else
            {
                puts("Exiting program...");

                //remove the local socket file & close sockets
                close(client_sockfd);
                close(server_sockfd);
                break;
            }
        }
    }

    return 0;
} //end of main

