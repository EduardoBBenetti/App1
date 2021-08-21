// TimeDelay.c
// taken and modified from Project 3

/* The following source code utilizes timers (T2CON) and interrupts to control 
 * the MCU delay
 * Edited by Eduardo B. , Tahseen I. and William M. 
*/

#include "xc.h"
#include "TimeDelay.h"
#include "UART2.h"

void delay_ms (uint16_t t) 
{
    //we use timer 2 every time delay function called
    
    NewClk(32); //ensure 32kHz is the operating time
    
    T2CONbits.TSIDL = 0;    // keep timer operation during idle
    T2CONbits.T32 = 0;      // 32-bit timer mode off - 16-bit timer mode on
    T2CONbits.TCS = 0;      // use internal clock source for timer2
    T2CONbits.TCKPS1 = 0;   // turn off pre-scaling
    T2CONbits.TCKPS0 = 0;   // turn off pre-scaling
    
    IPC1 = 0x6000;          // T2 interrupt priority level set to 6
    
    IEC0bits.T2IE = 1;      // enable timer interrupt
    IFS0bits.T2IF = 0;      // clear flag for when interrupt event occurs
        
    TMR2 = 0x0000;          // TMR2 reset to 0 
    PR2  = 16*t;            //16 * 2(1 / 32000) = 1 millisecond  
    
    T2CONbits.TON = 1;      // start 16bit timer2
    
    Idle(); //idle until timer is interrupted by PR2
    
    return;
}

void T3Init()
{
    //we use timer 3 to count the amount of time PB3 is pressed down for
    
    TMR3 = 0;       //reset timer 3
    PR3 = 3000*16;  //3000*16*2(1/32000) = 3000ms = 3 seconds
    
    IEC0bits.T3IE = 1; //enable interrupt exceptions
    IFS0bits.T3IF = 0; //T3 interrupt flag clear
    
    T3CONbits.TON = 1; //turn on Timer 3
}

// Interrupt Sub-Routine of timer 2 
void __attribute__ ((interrupt,no_auto_psv)) _T2Interrupt(void)
{
    IFS0bits.T2IF = 0; //clear interrupt flag for T2
    T2CONbits.TON = 0; //turn off timer
}

// Interrupt Sub-Routine of timer 3
void __attribute__ ((interrupt,no_auto_psv)) _T3Interrupt(void)
{
    T3CONbits.TON = 0; //turn off Timer 3
}