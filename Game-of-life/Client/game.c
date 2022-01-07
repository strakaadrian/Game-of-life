/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#include "game.h"
#include "structures.h"
#include "outputs.h"
#include "listeners.h"
#include "server_save_load.h"
#include "local_save_load.h"

// nastavime rozmery hry
Dimensions setDimensions(void) {
    Dimensions dim;
    char str[100] = {0};
    dim.row = 0;
    dim.column = 0;
    
    do {
        printf("Zadaj pocet riadkov: ");
        scanf("%s",str);
        dim.row = atoi(str);
    } while (dim.row == 0);
   
    memset(str,0, 4);
    
    do {
        printf("Zadaj pocet stlpcov: ");
        scanf("%s",str);
        dim.column = atoi(str);
    } while (dim.column == 0);
    
    return dim;
}

//hraciePole tick(hraciePole pole){
void tick(int row,int column,int board[row][column]){
    int newBoard[row][column];
    memset(newBoard, 0, sizeof(newBoard)); 
    
    for(int i = 0; i < row; ++i){
        for(int j = 0; j < column; ++j){   //pre kazdu bunku
            int neighbors = 0;
            for(int k = i - 1; k <= i + 1; ++k){
                for(int l = j - 1; l <= j + 1; ++l){       
                    if(k < 0 || l < 0 || k > row - 1 || l > column - 1){//out of bounds
                        continue;
                    }else if(board[k][l]){
                        if(!(k == i && l == j)){ 
                            neighbors++;
                        }                        
                    }
                }
            }
            if(neighbors < 2){ //1. Any live cell with fewer than two live neighbours dies, as if by underpopulation.
                newBoard[i][j] = 0;
            }else if((board[i][j] && neighbors == 2) || (board[i][j] && neighbors == 3)){ //2. Any live cell with two or three live neighbours lives on to the next generation.
                newBoard[i][j] = 1;
            }else if(board[i][j] && neighbors > 3){//3. Any live cell with more than three live neighbours dies, as if by overpopulation.
                newBoard[i][j] = 0;
            }else if(!board[i][j] && neighbors == 3){//4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
                newBoard[i][j] = 1;
            }else{//pre istotu
                newBoard[i][j] = 0;
            }
        }
    }

    memcpy(board,newBoard,column*row*sizeof(int));
    writeBoard((int *)board,row,column);
}

// TATO METODA SPUSTI HRU A  TIEZ KEY LISTENER
void runGame(Dimensions *dim, int board[dim->row][dim->column]) {
    GameParams gameParams;
    gameParams.continueGame = 1; // 1 ak chcem aby hra pokracovala, 0 ak chcem aby zastalo vypisovanie
    gameParams.endGame = 0; // 1 ak chcem hru vypnut a 0 ak nie
    gameParams.saveGameLocal = 0; // ak chceme ulozit hru tak 1 inak 0
    gameParams.gameSpeed = 1; // default je rychlost hry 1 sekunda
    gameParams.saveGameServer = 0; // ak chceme ulozit hru na server tak 1 inak 0
    gameParams.leaveGame = 0;
    pthread_t thread1; // zadeklarujeme vlakno, ktore bude pocuvat vstupy od uzivatela
    
    // vytvorime mutex
    if (pthread_mutex_init(&gameParams.lock, NULL) != 0) 
    { 
        printf("\n mutex init has failed\n"); 
    } 
    
    // vytvorime vlakno a povieme mu ze nech spusti funkciu menuPausedGameKeyListener 
    if(pthread_create(&thread1, NULL, menuPausedGameKeyListener, &gameParams)) {
        fprintf(stderr, "Error creating thread\n");
    }
    
    writeBoard((int *)board, dim->row, dim->column); // NAJSKVOR VYPISEME PLOCHU, KTORU SI UZIVATEL ZADAL AKO VSTUPNU
    // spustime hru
    while(1) {
        sleep(gameParams.gameSpeed);
        pthread_mutex_lock(&gameParams.lock);  
        if(gameParams.continueGame == 1) { // ak ma pokracovat hra tak pokracuje
            tick(dim->row, dim->column, board);
        } else {
            if(gameParams.endGame == 1) { // ak je hra pozastavena cakam ci ju uzivatel nevypne
                break;
            } else if(gameParams.leaveGame == 1) { // ak sa chcem vratit do hlavneho menu
                pthread_join(thread1, NULL); // joinem vlakno
                pthread_mutex_destroy(&gameParams.lock); // znic mutex
                gameSetupMenu(); // zavolam hlavne menu
                break;
            } else if(gameParams.saveGameLocal == 1) { // ak hru pozastavim a chcem ju ulozit 
                if(saveGameToLocalFile(dim, board) == true) { // hru ulozim, ak sa nepodari tak pride false
                    printf("Hra sa uložila lokálne \n");
                } else {
                    printf("Hru sa nepodarila uložiť \n");
                }
                pthread_join(thread1, NULL);
                //znova musim vytvorit vlakno pre pocuvanie vstupov od uzivatela
                if(pthread_create(&thread1, NULL, menuPausedGameKeyListener, &gameParams)) {
                    fprintf(stderr, "Error creating thread\n");
                }
                gameParams.saveGameLocal = 0;
            } else if(gameParams.saveGameServer == 1) {
                if(saveGameToServer(dim, board) == true) { // hru ulozim na server , ak sa nepodari tak pride false
                    printf("Hra sa uložila na server \n");
                } else {
                    printf("Hru sa nepodarilo uložiť \n");
                }
                pthread_join(thread1, NULL);
                //znova musim vytvorit vlakno pre pocuvanie vstupov od uzivatela
                if(pthread_create(&thread1, NULL, menuPausedGameKeyListener, &gameParams)) {
                    fprintf(stderr, "Error creating thread\n");
                }
                gameParams.saveGameServer = 0;
            }
        }
        pthread_mutex_unlock(&gameParams.lock);  
    }
    pthread_mutex_destroy(&gameParams.lock); // znic mutex
    pthread_join(thread1, NULL); // cakaj kym skonci thread
}