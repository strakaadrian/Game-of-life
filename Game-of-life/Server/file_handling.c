/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <dirent.h> // SLUZI NA NACITAVANIE NAZVOV SUBOROV
#include <string.h>

#include "file_handling.h"

#define DIR_SIZE 100
#define USER_DIR 30

// DOSTANEM CESTU
void getUserDir(char* cwd) {
    memset(cwd, 0, USER_DIR); //nastavim vsetky na 0
    
    getcwd(cwd, USER_DIR); //dostaneme cestu /home/straka33/Server/Files - to ale nie je ta cesta co chcem dostat
    cwd[strlen(cwd) - 13] = '\0'; // toto je potrebne aby sme dostali cestu /home/straka33 (cize to bude univerzalne pre kazdeho pouzivatela)
}

void readFile(Gamesize* gameSize,int board[gameSize->row][gameSize->column], char* fileName) {
    FILE *myFile;
    char directory[DIR_SIZE];
    char cwd[USER_DIR];
    
    memset(directory, 0, DIR_SIZE);

    getUserDir(cwd);
 
    snprintf(directory, sizeof(directory), "%s/Server/Storage/%s.txt", cwd, fileName); // VYTVORIME STRING AJ S NAZVOM SUBORU tzv. DIREKTIVU K SUBORU
    myFile = fopen(directory, "r"); // VYTVORENU DIREKTIVU K SUBORU OTVORIME
    
    if (myFile == NULL){
        printf("Takýto súbor neexistuje\n");
    } 
    
    fscanf(myFile, "%d", &(gameSize->row) );
    fscanf(myFile, "%d", &(gameSize->column) );
    
    for (int i = 0; i < gameSize->row; i++) {
        for (int j = 0; j < gameSize->column; j++) {
            fscanf(myFile, "%d", &board[i][j]); // PRE KAZDY PRVOK V POLI NACITAVAME ZO SUBORU, NACITAVANIE POKRACUJE OD 3 CISLA
        }
    }
    fclose(myFile); //ZAVRIEME SUBOR
}

// FUNKCIA SKONTROLUJE CI ZADANY SUBOR EXISTUJE
bool checkFileExists(char* fileName, Gamesize* gameSize) {
    FILE *myFile;
    char directory[DIR_SIZE];
    char cwd[USER_DIR];
    
    memset(directory, 0, DIR_SIZE);

    getUserDir(cwd);
    
    snprintf(directory, sizeof(directory), "%s/Server/Storage/%s.txt", cwd, fileName); // VYTVORIME STRING AJ S NAZVOM SUBORU tzv. DIREKTIVU K SUBORU
    myFile = fopen(directory, "r"); // VYTVORENU DIREKTIVU K SUBORU OTVORIME
    
    if (myFile == NULL){
        printf("Súbor neexistuje\n");
        return false;
    } else {
        fscanf(myFile, "%d", &(gameSize->row)); // PRVE NACITANE CISLO JE POCET RIADKOV
        fscanf(myFile, "%d", &(gameSize->column)); // DRUHE NACITANE CISLO JE POCET STLPCOV
        fclose(myFile);
        return true;
    }
}

// METODA NAM VYPISE VSETKY SUBORY V DANEJ DIRECTORY
void writeFilesInDirectory(char* savedFileNames) {
    DIR *dir;
    struct dirent *ent;
    char directory[DIR_SIZE];
    char cwd[USER_DIR];
    
    memset(directory, 0, DIR_SIZE);
    
    getUserDir(cwd);
    
    sprintf(directory,"%s/Server/Storage/", cwd);
    
    if ((dir = opendir (directory)) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            if(ent->d_name[0] != '.') { // VYPISEME VSETKY SUBORY OKREM CIES '.' A '..'
                strcat(savedFileNames, ent->d_name); // nacitamavame nazvy suborov do stringu
                strcat(savedFileNames, "\n"); // za kazdy nazov suboru dame novy riadok
            }
        }
        closedir (dir);
    } else {
        strcat(savedFileNames, "Nepodarilo sa nacitat ulozene hry na servery !");
        perror (""); 
    }
}

bool saveGameToServer(char* fileName, int bufferSize, int* board) {
    FILE *myFile;
    char directory[DIR_SIZE];
    char cwd[USER_DIR];
    
    memset(directory, 0, DIR_SIZE);

    getUserDir(cwd);
 
    snprintf(directory, sizeof(directory), "%s/Server/Storage/%s.txt", cwd, fileName); // VYTVORIME STRING AJ S NAZVOM SUBORU tzv. DIREKTIVU K SUBORU
    myFile = fopen(directory, "w"); // OTVORIME SUBOR NA ZAPISOVANIE, AK SUBOR EXISTUJE PREPISE HO AK NEEXISTUJE TAK HO VYTVORI
    
    if (myFile == NULL){
        printf("Chyba pri vytváraní súboru\n");
        return false;
    }
    
    //NAJSKVOR ZAPISEME PRVE DVE CISLA KTORE PREDSTAVUJU RIADOK A STLPEC
    fprintf(myFile,"%d ",board[0]);
    fprintf(myFile,"%d ",board[1]);
    
    // POTOM ZAPISUJEM DO SUBORU CELU MATICU
    for (int i = 2; i < bufferSize; i++) {
        fprintf(myFile,"%d ",board[i]);
    }
    fclose(myFile); //ZAVRIEME SUBOR
    
    return true;
}