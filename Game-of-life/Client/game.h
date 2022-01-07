/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   game.h
 * Author: ADO-PC
 *
 * Created on November 23, 2019, 8:06 PM
 */

#ifndef GAME_H
#define GAME_H
#include "structures.h"

#ifdef __cplusplus
extern "C" {
#endif

Dimensions setDimensions(void);
void tick(int row,int column,int board[row][column]);
void runGame(Dimensions *dim, int board[dim->row][dim->column]);

#ifdef __cplusplus
}
#endif

#endif /* GAME_H */

