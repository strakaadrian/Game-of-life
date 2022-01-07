/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   client_handling.h
 * Author: ADO-PC
 *
 * Created on November 24, 2019, 5:51 PM
 */

#ifndef CLIENT_HANDLING_H
#define CLIENT_HANDLING_H

#ifdef __cplusplus
extern "C" {
#endif

void waitForClients(void);
void* client_handler(void *par);

#ifdef __cplusplus
}
#endif

#endif /* CLIENT_HANDLING_H */

