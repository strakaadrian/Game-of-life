/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: ADO-PC
 *
 * Created on October 5, 2019, 7:05 PM
 */
#include <stdlib.h>

#include "client_handling.h"

int main(int argc, char** argv) {
    //ZAPNEME SERVER
    waitForClients();
    
    return (EXIT_SUCCESS);
}

