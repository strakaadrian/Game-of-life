/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h> 
#include <unistd.h>

#include "server_save_load.h"
#include "connection.h"
#include "outputs.h"
#include "game.h"

bool saveGameToServer(Dimensions *dim, int board[dim->row][dim->column]) {
    int new_socket = 0, read_size = 0;
    char fileName[20], msgToServer[21] = {'s'};
    int buffer[(dim->row * dim->column) + 2]; // potrebujem poslat velkost riadku velkost stlpca a cele pole tak preto +2
    int msg = 0, serverMsg = 0, boardSize = (dim->row * dim->column + 2), index = 2;
    
    memset(buffer,0, (dim->row * dim->column + 2)*sizeof(int) );
    
    new_socket = connection(); // pripoj sa na server
    
    if(new_socket < 0) { // ak je mensi ako 0, to znamena, ze pripojenie sa nezdarilo
        close(new_socket);
        return false;
    }
    
    printf("Zadaj názov súboru (MAX 20 ZNAKOV), do ktorého sa hra uloží:\n");
    scanf("%20s", fileName);
    strcpy(msgToServer + 1, fileName);
    
    //-----------------------------------------------
    //-------KOMUNIKACIA KLIENT - SERVER-------------
    //-----------------------------------------------
    send(new_socket, msgToServer, strlen(msgToServer), 0); // poslem spravu na server, obsahuje informaciu ze chcem ukladat na server + nazov suboru
    
    read_size = recv(new_socket, &serverMsg, sizeof(int), 0);
    
    if(read_size <= 0 || serverMsg == 0) { 
        close(new_socket);
        return false;
    }
    serverMsg = 0;
    
    send(new_socket, &boardSize, sizeof(int), 0); // posielam velkost plochy na server
    
    read_size = recv(new_socket, &serverMsg, sizeof(int), 0);
    
    if(read_size <= 0 || serverMsg == 0) { 
        close(new_socket);
        return false;
    }
    serverMsg = 0;
    
    // ako prve do pola vlozime pocet riadkov a pocet stlpcov
    buffer[0] = dim->row;
    buffer[1] = dim->column;
    
    // nasledne do pola, ktore posielame na server, vlozime plochu
    memcpy(buffer + 2, board,boardSize * sizeof(int));
 
    send(new_socket, buffer, boardSize*sizeof(int), 0); // na server posielam board
 
    read_size = recv(new_socket, &serverMsg, sizeof(int), 0);
    if(read_size <= 0 || serverMsg == 0) { 
        close(new_socket);
        return false;
    }
    close(new_socket);
    return true;
}

void loadFromServer(void) {
    int new_socket = 0, read_size = 0, serverMsg = 0, boardSize[2];
    char msgToServer = 'l';
    char loadedSaveNames[2500], fileName[20]; // 2500 by malo byt postacujuce je to min 100 saveov
    Dimensions dim;
    
    memset(loadedSaveNames, 0, 2500);
    memset(fileName, 0, 20);
    
    new_socket = connection(); // pripoj sa na server
    
    if(new_socket < 0) { // ak je mensi ako 0, to znamena, ze pripojenie sa nezdarilo
        close(new_socket);
        sleep(3);
        gameSetupMenu();
    } else {
        send(new_socket, &msgToServer, sizeof(char), 0); // na server poslem pismeno "l", co znamena ze chcem nacitavat
        
        read_size = recv(new_socket, &serverMsg, sizeof(int), 0); // cakam na odpoved zo servera ci sa nacitalo spravne
            
        if(read_size <= 0 || serverMsg == 0) { 
            close(new_socket);
            printf("Pri nacitavani doslo k chybe, budete vrátený naspäť do hlavného menu\n");
            sleep(3);
            gameSetupMenu();
            return;
        }
        
        read_size = recv(new_socket, loadedSaveNames, 2500, 0); // klient bude od serveru cakat spravu s nazvami ulozenych hier
        
        printf("%s", "//////////Uložené hry, ktoré sa nachádzajú na servery/////////\n");
        // vypiseme subory zo servera
        printf("%s", loadedSaveNames);
        
        while(1) {
            memset(fileName, 0, 20);
            //uzivatel napise, ktory subor chce nacitat
            printf("\n");
            printf("%s", "//////////////////////////////////////////////////////////\n");
            printf("Ak chcete načítať hru zo súboru, napíšte iba jeho názov (MAX 20 znakov).\n");
            printf("Pre návrat do hlavného menu napíš -exit-.\n");
            scanf("%s", fileName);
            
            if( strcmp(fileName,"EXIT") == 0 || (strcmp(fileName,"exit") == 0)) { // AK CHCEM SKONCIT TAK ZAVOLAM HLAVNE MENU
                strcpy(fileName,"exit");
                send(new_socket, fileName, strlen(fileName), 0);
                close(new_socket);
                gameSetupMenu();
                return;
            }
            
            send(new_socket, fileName, strlen(fileName), 0); // nazov suboru posleme na server
        
            read_size = recv(new_socket, &serverMsg, sizeof(int), 0); // server skontroluje ci dany subor existuje 
            
            if(read_size <= 0 || serverMsg == 0) { 
                printf("Daný súbor sa na servery nenachádza, názov zadajte znovu.\n");
                continue;       
            }
            break; // ak som uz tu, znamena to ze subor je na servery a mozem ho prijat
        }
        read_size = recv(new_socket, boardSize, 2*sizeof(int), 0); // server posiela velkost plochy
        
        int board[boardSize[0]][boardSize[1]]; // vyvorime pomocnu plochu
        memset(board, 0, sizeof(board));
        
        read_size = recv(new_socket, board, boardSize[0]*boardSize[1]*sizeof(int), 0); // server posiela plochu
        
        dim.row = boardSize[0];
        dim.column = boardSize[1];
             
        runGame(&dim, board); // posielam board + 2 lebo prve dva parametre su row a column a to nas uz nezaujima
    }
}