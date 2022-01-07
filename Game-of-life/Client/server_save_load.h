/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   server_save_load.h
 * Author: ADO-PC
 *
 * Created on November 23, 2019, 8:17 PM
 */

#ifndef SERVER_SAVE_LOAD_H
#define SERVER_SAVE_LOAD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "structures.h"

bool saveGameToServer(Dimensions *dim, int board[dim->row][dim->column]);
void loadFromServer(void);

#ifdef __cplusplus
}
#endif

#endif /* SERVER_SAVE_LOAD_H */

