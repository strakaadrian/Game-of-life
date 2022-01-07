
/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: Ado
 *
 * Created on October 5, 2019, 4:51 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "outputs.h"


int main(int argc, char** argv) {
    printf("VITAJTE V HRE GAME OF LIFE !!!\n");
    // zavolame uvodne menu
    gameSetupMenu();
    
    return (EXIT_SUCCESS);
}

