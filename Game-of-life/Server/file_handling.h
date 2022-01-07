/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   file_handling.h
 * Author: ADO-PC
 *
 * Created on November 24, 2019, 5:55 PM
 */

#ifndef FILE_HANDLING_H
#define FILE_HANDLING_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "structures.h"    
    
void getUserDir(char* cwd);    
void readFile(Gamesize* gameSize,int board[gameSize->row][gameSize->column], char* fileName);
bool checkFileExists(char* fileName, Gamesize* gameSize);
void writeFilesInDirectory(char* savedFileNames);
bool saveGameToServer(char* fileName, int bufferSize, int* board);

#ifdef __cplusplus
}
#endif

#endif /* FILE_HANDLING_H */

