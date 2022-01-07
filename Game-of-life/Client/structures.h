/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   structures.h
 * Author: ADO-PC
 *
 * Created on November 23, 2019, 7:32 PM
 */

#ifndef STRUCTURES_H
#define STRUCTURES_H
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif
    
// rozmery hry, pouzili sme strukturu, pretoze mozme vracat 2 hodnoty vo funkcii
typedef struct dimensions {
    int column;
    int row;
}Dimensions;

// Tieto parametre nastavuju ci hra pokracuje alebo hra skonci
typedef struct gameParams {
    int continueGame;
    int endGame;
    int leaveGame;
    int saveGameLocal;
    int saveGameServer;
    double gameSpeed;
    pthread_mutex_t lock;
}GameParams;


#ifdef __cplusplus
}
#endif

#endif /* STRUCTURES_H */

