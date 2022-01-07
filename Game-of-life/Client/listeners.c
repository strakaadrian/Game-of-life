/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "listeners.h"
#include "structures.h"
#include "outputs.h"
#include "local_save_load.h"
#include "server_save_load.h"

// pre metodu gameSetupMenu()
// cakam kým nieje stlacene tlacidlo a potom vykonam co uzivatel zadal
void gameSetupMenuKeyPressed(void) {
    char keyPressed;
    //cakam, az uzivatel stlaci tlacidlo
    while(1) {
        scanf(" %c", &keyPressed);
        if(keyPressed == 49) {
            ownLifeCellsDefinition(); // zavola vlastne definovanie zivych buniek
            break;
        } else if(keyPressed == 50) {
            randomCellDefinition(); // zavola random definovanie zivych buniek
            break;   
        } else if(keyPressed == 51) {
            loadFromServer();
            break;
        } else if(keyPressed == 52) {
            loadFromLocalFile(); // zavola nacitanie buniek z lokalneho pocitaca
            break;
        } else if(keyPressed == 27) {
            exit(0);
            break;
        } else {
            printf("Zadal si neexistujúcu možnosť, zadaj znovu !!\n");
        }
    }
}

// pocas spustenej hry caka na vstupy od uzivatela
void *menuPausedGameKeyListener(void *par) {
    GameParams *gameParams = (GameParams *) par;
    char a;
    double speed = 0.0;
     
    while(1) {
        scanf("%c", &a);
        pthread_mutex_lock(&gameParams->lock); // zamkni mutex
        if (a == 27 && gameParams->continueGame == 1) { // POZASTAV HRU A VYPIS MENU
            menuPausedGame();
            gameParams->continueGame = 0;
        } else if(a == 27 && gameParams->continueGame == 0) { // ESC - POKRACUJ V HRE
            gameParams->continueGame = 1;
        } else if( (a == 88 || a == 120) && gameParams->continueGame == 0) { // X - VYPNI HRU
            gameParams->endGame = 1;
            printf("Hra sa ukončuje...DOVIDENIA\n");
            break;
        } else if((a == 72 || a == 104) && gameParams->continueGame == 0) { // H - NAVRAT DO HLAVNEHO MENU
            gameParams->leaveGame = 1;   
            break;
        } else if((a == 76 || a == 108) && gameParams->continueGame == 0) { // L - ULOZI HRU DO LOKALNEHO SUBORU
            gameParams->saveGameLocal = 1;
            break;
        } else if(a == 82 || a == 114 && gameParams->continueGame == 0) { // R -  ZMEN RYCHLOST HRY
            printf("Zadaj rýchlosť medzi <0.1, 100> sekúnd\n");
            scanf("%lf", &speed);
            
            if(speed < 0.1 || speed > 100.0) {
                printf("Zadal si nesprávnu rýchlosť. Rýchlosť sa NEZMENILA! \n");
            } else {
                gameParams->gameSpeed = speed; // zadame rýchlosť hry
                printf("Rýchlosť hry sa úspešne zmenila na %.2f sekundy\n", speed);
            }
        } else if((a == 83 || a == 115) && gameParams->continueGame == 0) { // S - ULOZ HRU NA SERVER
            gameParams->saveGameServer = 1;
            break;
        }
        pthread_mutex_unlock(&gameParams->lock); // odomkni mutex
    }
    pthread_mutex_unlock(&gameParams->lock);
    pthread_exit(NULL); // ukonci vlakno
}
