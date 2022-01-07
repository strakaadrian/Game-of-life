/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>        
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <arpa/inet.h>

#include "structures.h"
#include "client_handling.h"
#include "file_handling.h"

#define FILE_SIZE 20
#define STRING_SIZE 2500
#define PORT 8000

void waitForClients(void) {
    struct sockaddr_in address;
    int server_fd, new_socket, client_socket;
    char buf[1024];
    int clilen = sizeof(address); 
    
    address.sin_family = AF_INET; // pouzijeme TCP ivp4
    address.sin_addr.s_addr = inet_addr("127.0.0.1"); // definujeme si vlastnu adresu, kde bude pocuvat | inet_addr prerobi IP string na IPv4 adresu
    address.sin_port = htons( PORT ); // pridame mu port
    
    // vytvorime socket na ktorom bude pocuvat server api
   
    // AF_INET - IPv4 Internet protocols 
    // SOCK_STREAM - Provides sequenced, reliable, two-way, connection-based byte streams.     
    
    if( (server_fd = socket(AF_INET,SOCK_STREAM,0)) == 0) { 
        fprintf(stderr, "Nepodarilo sa vytvorit socket, na ktorom server bude počúvať! \nError: %s\n", strerror(errno));
        return; 
    }
    
    // teraz musime socket priradit danej adsrese, ktoru sme si uz specifikovali vyssie
    
    if(bind(server_fd, (struct sockaddr *) &address, sizeof(address)) < 0) {
        fprintf(stderr, "Nepodarilo sa priradit adresu socketu! \nError: %s\n", strerror(errno));
        return; 
    }
    
    // server pocuva a prijme maximalne 3 klientov
    if(listen(server_fd,3) < 0) {
        fprintf(stderr, "Error: %s\n", strerror(errno));
        return;
    } 
   
    printf("Server je UP (port: %d)\n", PORT);
    fflush(stdout); // bez tohto nejde vypisat printf
    pthread_t thread;
    
    while(1) {
        // server prijme ziadost o spojenie od klienta
        if( (client_socket = accept(server_fd, (struct sockaddr *) &address, (socklen_t*)&clilen)) > 0) {
            // pre kazde spojenie vytvorime jedno vlakno
            if(pthread_create(&thread, NULL, client_handler, &client_socket)) {
                fprintf(stderr, "Error creating thread\n");
                close(client_socket); //ak sa nepodari spravit vlakno, zavrem socket od klienta
                continue;
            }
            
            printf("Pripojil sa klient.\n");
        } else {
            // ak sa spojenie nepodari upozornim na to
            fprintf(stderr, "Error: %s\n", strerror(errno));
            continue;
        }
        pthread_join(thread, NULL);
    }
    close(server_fd);
}

// funkcia pre spravu pripojeneho klienta
void* client_handler(void *par) {
    int *client_soc = (int *) par;
    int read_size, acl = 0, bufferSize = 0,boardSize[2], exitVal = 0; // velka velkost buffera je preto ze mozme mat hru velkosti az 1000x1000 co je 1000 000
    char clientString[21], savedFileNames[STRING_SIZE], fileName[FILE_SIZE];
    Gamesize gameSize;
    
    memset(clientString,0,21);
    memset(savedFileNames,0,STRING_SIZE);
    memset(fileName, 0, FILE_SIZE);

    //-----------------------------------------------
    //-------KOMUNIKACIA KLIENT - SERVER-------------
    //-----------------------------------------------
    read_size = recv(*client_soc, clientString, 21, 0); // nacitam spravu od klienta, na prvom znaku je bud s - chcem ulozit na server, alebo l - chcem nacitavat zo serveru, dalsich 20 znakov je nazov suboru
    
    if(read_size > 0) { 
        acl = 1;
    } 
    send(*client_soc, &acl, sizeof(int), 0); // ak sa mi podarilo nacitat spravu od klienta tak mu poslem odpoved, ze vsetko je v poriadku 1
    acl = 0;
    
    if(clientString[0] == 'l') { // chcem nacitavat zo servera
        writeFilesInDirectory(savedFileNames); // nacitame subory z priecinku
        
        send(*client_soc, savedFileNames, strlen(savedFileNames), 0); // posleme ulozene hry na servery rovno klientovi
        
        while(recv(*client_soc, fileName, FILE_SIZE, 0) > 0) {
            if(strcmp(fileName, "exit") == 0) {
                exitVal = 1;
                break;
            } else {
                if(checkFileExists(fileName, &gameSize) == true) {
                   acl = 1; 
                   send(*client_soc, &acl, sizeof(int), 0);
                   acl = 0;
                   break;
                } else {
                   memset(fileName, 0, FILE_SIZE);
                   send(*client_soc, &acl, sizeof(int), 0); 
                   continue;
                }
            }
        }
        if(exitVal == 0) {
            boardSize[0] = gameSize.row;
            boardSize[1] = gameSize.column;

            send(*client_soc, &boardSize, 2*sizeof(int), 0);  // posleme klientovi velkost plochy

            int board[gameSize.row][gameSize.column]; // vyvorime pomocnu plochu
            memset(board, 0, sizeof(board));
            
            readFile(&gameSize, board, fileName);

            send(*client_soc, board, boardSize[0]*boardSize[1]*sizeof(int), 0);
        }
    } else { // chcem ukladat na server
        read_size = recv(*client_soc, &bufferSize, sizeof(int), 0); // teraz chceme od klienta zistit velkost plochy
    
        if(read_size > 0) { 
            acl = 1;
        }
        send(*client_soc, &acl, sizeof(int), 0); // ak sa mi podarilo nacitat spravu od klienta tak mu poslem odpoved, ze vsetko je v poriadku 1
        acl = 0;

        int bufferBoard[bufferSize]; // vytvorim buffer velkosti hracej plochy kam plochu nacitam od klienta
        memset(bufferBoard, 0, bufferSize);

        read_size = recv(*client_soc, bufferBoard, bufferSize*sizeof(int), 0);

        if(saveGameToServer(clientString + 1, bufferSize, bufferBoard) == true) {
            acl = 1;
        }
        send(*client_soc, &acl, sizeof(int), 0);
    }
    
    printf("Client sa odpojil!\n");
    fflush(stdout);
    close(*client_soc); //zavrieme socket klienta
    pthread_exit(NULL); // ukoncime vlakno, ktore klienta obsluhovalo
}
