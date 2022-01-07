/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   local_save_load.h
 * Author: ADO-PC
 *
 * Created on November 23, 2019, 8:30 PM
 */

#ifndef LOCAL_SAVE_LOAD_H
#define LOCAL_SAVE_LOAD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "structures.h"
    
void getUserDir(char* cwd);    
bool saveGameToLocalFile(Dimensions *dim, int board[dim->row][dim->column]);
void writeFilesInDirectory(void);
void loadFromLocalFile(void);
void ownLifeCellsDefinition(void);
void randomCellDefinition(void);

#ifdef __cplusplus
}
#endif

#endif /* LOCAL_SAVE_LOAD_H */

