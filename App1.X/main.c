/*
 * File:   main.c
 * Author: Rushi V
 * Edited by Eduardo B. , Tahseen I. and William M. 
 * Created on September 26, 2020, 9:17 PM
 */

// MPLAB header libraries
#include <xc.h>

#include <p24fxxxx.h>
#include <p24F16KA101.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>

// User header files
#include "IOs.h"
#include "timeDelay.h"
#include "ChangeClk.h"
#include "UART2.h"


//Preprocessor directives - Configuration bits for MCU start up
#pragma config FCKSM = CSECMD // Clock switching is enabled, clock monitor disabled

// MACROS for Idle, Sleep modes
#define Nop() {__asm__ volatile ("nop");}
#define ClrWdt() {__asm__ volatile ("clrwdt");}
#define Sleep() {__asm__ volatile ("pwrsav #0");}   //Sleep() - put MCU in sleep mode - CPU and some peripherals off
#define Idle() {__asm__ volatile ("pwrsav #1");}    //Idle() - put MCU in idle mode - only CPU off
#define dsen() {__asm__ volatile ("BSET DSCON, #15");} //

//MAIN
void main(void) 
{     
    //NOTE: if this is changed, then the PR2 in timeDelay.c must be changed (should be 32kHz for Application 1)
    NewClk(32); // 8 for 8 MHz; 500 for 500 kHz; 32 for 32 kHz
    
    AD1PCFG = 0xFFFF; // Set all analog pins as digital
    
    IOInit();  //initialize IOs
    
    //note: Timer 2 gets initialized whenever the delay function gets called
    
    while(1)
    {
        IOCheck();
    }
    
    return;
}
