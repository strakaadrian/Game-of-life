/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <errno.h>

#include "connection.h"

#define PORT 8000 

//PRIPOJENIE NA SERVER
int connection(void) {
    int new_socket = 0;
    struct sockaddr_in server_add;

    //naplnime si strukturu server_add
    server_add.sin_family = AF_INET; // AF_INET refers to addresses from the internet, IP addresses specifically.
    server_add.sin_addr.s_addr = inet_addr("127.0.0.1"); // The inet_addr() function shall convert the string pointed to by cp, in the standard IPv4 dotted decimal notation
    server_add.sin_port = htons( PORT ); // The htons() function converts the unsigned short integer hostshort from host byte order to network byte order.
    
    // pre klienta vytvorime novy socket, ak je mensi ako 0 nepodarilo sa ho vytvorit
    if ((new_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        fprintf(stderr, "Nepodarilo sa vytvorit socket! \nError: %s\n", strerror(errno));
        return -1; 
    }
    
    // socket pripojime na SERVER, ak je odpoved mensia ako 0 nepodarilo sa pripojit na server
    if( connect(new_socket, (struct sockaddr *)&server_add, sizeof(server_add)) < 0 ) {
        fprintf(stderr, "Nepodarilo sa pripojiť na server! \nError: %s\n", strerror(errno));
        return -1; 
    }

    // ak sa vsetko podarilo ako odpoved bude nas novy socket
    return new_socket;
}
