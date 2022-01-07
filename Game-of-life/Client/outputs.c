/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "outputs.h"
#include "listeners.h"

// vypiseme hru
void writeBoard(int * pole, int row, int column){
    system("clear");
    
    printf("%s", "║");
    for (int j = 0; j < column*2; j++) {
        printf("=");
    }
    printf("%s", "║");
    printf("\n");
    printf("%s", "║");
    for (int i = 0; i < column*row; i++) {
        if(*(pole + i) == 0){
            printf("%c ",' ');
        } else {
            printf("%s", "■");
        }
        if((i+1) % column == 0) {
            printf("%s", "║");
            printf("\n");
            printf("%s", "║");
        }
    }
    for (int j = 0; j < column*2; j++) {
        printf("=");
    }
    printf("%s", "║");
    printf("\n");
}

// menu na uvod do hry
void gameSetupMenu(void) {
    //zmazeme plochu
    system("clear");
    
    printf("MENU NASTAVENIA HRY \n");
    printf("1 - Vlastná definícia živých buniek \n");
    printf("2 - Náhodné rozdelenie živých buniek \n");
    printf("3 - Načítanie vzoru zo servera \n");
    printf("4 - Načítanie vzoru z lokálneho súboru \n");
    printf("ESC - UKONCI HRU\n");
    printf("****************************************\n");
    
    gameSetupMenuKeyPressed();
}

// MENU PRERUSENIA HRY
void menuPausedGame(void) {
    printf("**********MENU PRERUSENA HRA**********\n");
    printf("ESC - pokračuj v hre\n");
    printf("R - zmeň rýchlosť hry\n");
    printf("L - uložiť hru lokálne\n");
    printf("S - uložiť hru na server\n");
    printf("H - návrat do hlavného menu\n");
    printf("X - vypni hru\n");
    printf("**************************************\n");
}
