/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h> // SLUZI NA NACITAVANIE NAZVOV SUBOROV

#include "local_save_load.h"
#include "game.h"
#include "outputs.h"

#define DIR_SIZE 100
#define USER_DIR 30

// DOSTANEM CESTU
void getUserDir(char* cwd) {
    memset(cwd, 0, USER_DIR); //nastavim vsetky na 0
    
    getcwd(cwd, USER_DIR); //dostaneme cestu /home/straka33/Server/Files - to ale nie je ta cesta co chcem dostat
    cwd[strlen(cwd) - 13] = '\0'; // toto je potrebne aby sme dostali cestu /home/straka33 (cize to bude univerzalne pre kazdeho pouzivatela)
}

bool saveGameToLocalFile(Dimensions *dim, int board[dim->row][dim->column]) {
    FILE *myFile;
    char fileName[20], directory[DIR_SIZE];
    char cwd[USER_DIR];
    
    memset(directory, 0, DIR_SIZE);

    getUserDir(cwd);
    
    printf("Zadaj názov súboru (MAX 20 ZNAKOV), do ktorého sa hra uloží:\n");
    scanf("%20s", fileName);
   
    snprintf(directory, sizeof(directory), "%s/Client/Storage/%s.txt", cwd, fileName); // VYTVORIME STRING AJ S NAZVOM SUBORU tzv. DIREKTIVU K SUBORU
    myFile = fopen(directory, "w"); // OTVORIME SUBOR NA ZAPISOVANIE, AK SUBOR EXISTUJE PREPISE HO AK NEEXISTUJE TAK HO VYTVORI
    
    if (myFile == NULL){
        printf("Chyba pri vytvaraní súboru\n");
        return false;
    }
    
    //NAJSKVOR ZAPISEME PRVE DVE CISLA KTORE PREDSTAVUJU RIADOK A STLPEC
    fprintf(myFile,"%d ",dim->row);
    fprintf(myFile,"%d ",dim->column);
    
    // POTOM ZAPISUJEM DO SUBORU CELU MATICU
    for (int i = 0; i < dim->row; i++) {
        for (int j = 0; j < dim->column; j++) {
            fprintf(myFile,"%d ",board[i][j]);
        }
    }
    fclose(myFile); //ZAVRIEME SUBOR
    
    return true;
}

// METODA NAM VYPISE VSETKY SUBORY V DANEJ DIRECTORY
void writeFilesInDirectory(void) {
    DIR *dir;
    struct dirent *ent;
    char cwd[USER_DIR];
    char directory[DIR_SIZE];
    
    memset(directory, 0, DIR_SIZE);
    
    getUserDir(cwd);
    
    sprintf(directory,"%s/Client/Storage/", cwd);
    
    if ((dir = opendir (directory)) != NULL) {
        printf("Zoznam lokálne uložených hier: \n");
        while ((ent = readdir (dir)) != NULL) {
            if(ent->d_name[0] != '.') { // VYPISEME VSETKY SUBORY OKREM CIES '.' A '..'
                printf ("%s\n", ent->d_name);
            }
        }
        closedir (dir);
    } else {
        perror ("");
    }
}

// FUNKCIA NACITA HRU Z LOKALNE ULOZENEHO SUBORU
void loadFromLocalFile(void) { 
    char fileName[20], directory[DIR_SIZE];
    FILE *myFile = NULL;
    Dimensions dim;
    char cwd[USER_DIR];
    
    getUserDir(cwd);
    
    system("clear");
    
    //NAJSVOR POTREBUJME POUZIVATELOVI VYPISAT VSETKY ULOZENE SUBORY, ABY VEDELE, KTORY MA NACITAT
    writeFilesInDirectory();
    
    while(1) {
        printf("\n");
        printf("Ak chcete načítať hru zo súboru, napíšte iba jeho názov.\n");
        printf("Pre návrat do hlavného menu napíš -exit-.\n");
        scanf("%s",fileName); // NACITAME NAZOV SUBORU
        
        if( strcmp(fileName,"EXIT") == 0 || (strcmp(fileName,"exit") == 0)) { // AK CHCEM SKONCIT TAK ZAVOLAM HLAVNE MENU
            gameSetupMenu();
            break;
        }
        
        snprintf(directory, sizeof(directory), "%s/Client/Storage/%s.txt", cwd, fileName); // VYTVORIME STRING AJ S NAZVOM SUBORU tzv. DIREKTIVU K SUBORU
        myFile = fopen(directory, "r"); // VYTVORENU DIREKTIVU K SUBORU OTVORIME
    
        if (myFile == NULL){
            printf("Takýto súbor neexistuje\n");
            continue;
        } else {
            break;
        }
    }
    
    fscanf(myFile, "%d", &dim.row); // PRVE NACITANE CISLO JE POCET RIADKOV
    fscanf(myFile, "%d", &dim.column); // DRUHE NACITANE CISLO JE POCET STLPCOV
    
    int board[dim.row][dim.column]; // VYTVORIME DVOJROZMERNE POLE O VELKOSTI ROW A COLUMN
    memset(board, 0, sizeof(board));
    
    for (int i = 0; i < dim.row; i++) {
        for (int j = 0; j < dim.column; j++) {
            fscanf(myFile, "%d", &board[i][j]); // PRE KAZDY PRVOK V POLI NACITAVAME ZO SUBORU, NACITAVANIE POKRACUJE OD 3 CISLA
        }
    }
    fclose(myFile); //ZAVRIEME SUBOR
    myFile = NULL;
    
    runGame(&dim, board);
}

// uzivatel si sam zada bunky, ktore budu zive
void ownLifeCellsDefinition(void) {
    Dimensions dim;
    int x = -2,y = -2;
    char out[DIR_SIZE];
    
    // nacitame rozmery hry
    dim = setDimensions();
    
    // vytvorime hru, ktora bude zatial len s mrtvymi bunkami
    int board[dim.row][dim.column];
    memset(board, 0, sizeof(board));
    
    while(1) {
        system("clear");
        printf("Pre ukoncenie zadajte [-1,-1]\n");
        
        // uzivatelovi vypisujeme suradnice, kam moze zadat zive bunky 
        for (int i = 0; i < dim.row; i++) {
            for (int j = 0; j < dim.column; j++) {
                if(board[i][j] == 0) {
                    printf("[%d,%d] ", i, j);
                } else if (board[i][j] == 1) {
                    printf("%c ",35); // ak bunka uz je ziva tak nevypisujem suradnice
                }
            }
            printf("\n");
        }
  
        do {
            printf("Zadajte suradnicu X: ");
            if( scanf("%d",&x) == 0 ) {
                scanf("%s", out);
                continue;
            }
            getchar();
            printf("Zadajte suradnicu Y: ");
            if( scanf("%d",&y) == 0 ) {
                scanf("%s", out);
                continue;
            }    
            getchar();
        } while ( (x < -1 || x > dim.row - 1) && (y < -1 || y > dim.column - 1) );

        if(x == -1 && y == -1) {
            break;
        }
        
        // skontrolujem ci uzivatel zadal dobre suradnice
        if(x > 0 || x < dim.row || y > 0 || y < dim.column) {
            board[x][y] = 1;
        } else {
            printf("Zadali ste nespravne suradnice !!\n");
        }
    }
    // spustime hru s danym rozlozenim buniek, ktore si zadal uzivatel
    runGame(&dim, board);
}

//RANDOM DEFINOVANIE ZIVYCH BUNIEK
void randomCellDefinition(void) {
    Dimensions dim;
    double probability = 0.0;
    srand(time(0)); // nahodny seed
    
    // nacitame rozmery hry
    dim = setDimensions();
    
    // vytvorime hru, ktora bude zatial len s mrtvymi bunkami
    int board[dim.row][dim.column];
    memset(board, 0, sizeof(board));
    
    // teraz pre kazdu bunku v hre zistim pravdepodobnost a ak bude mensia ako dane cislo tak bunka bude ziva
    
    for (int i = 0; i < dim.row; i++) {
        for (int j = 0; j < dim.column; j++) {
            probability = (rand() % 101 ) / 100.0; // dostaneme cisla medzi 0.00 az 100.0
            
            if(probability <= 0.30) {
                board[i][j] = 1; // ak padneme do nasej pravdepodobnosti na dane miesto dame zivu bunku
            }   
            probability = 0.0;
        }
    }
    // spustime hru s danym rozlozenim buniek, ktore si zadal uzivatel
    runGame(&dim, board);
}