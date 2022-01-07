/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   listeners.h
 * Author: ADO-PC
 *
 * Created on November 23, 2019, 7:55 PM
 */

#ifndef LISTENERS_H
#define LISTENERS_H

#ifdef __cplusplus
extern "C" {
#endif

void gameSetupMenuKeyPressed(void);
void *menuPausedGameKeyListener(void *par);

#ifdef __cplusplus
}
#endif

#endif /* LISTENERS_H */

