/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   outputs.h
 * Author: ADO-PC
 *
 * Created on November 23, 2019, 7:50 PM
 */

#ifndef OUTPUTS_H
#define OUTPUTS_H

#ifdef __cplusplus
extern "C" {
#endif

void writeBoard(int * pole, int row, int column);
void gameSetupMenu(void);
void menuPausedGame(void);

#ifdef __cplusplus
}
#endif

#endif /* OUTPUTS_H */

