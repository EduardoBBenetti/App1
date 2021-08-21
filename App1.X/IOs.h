#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

//some macros defined for the push buttons used in this project
#define PB1 PORTAbits.RA2
#define PB2 PORTAbits.RA4
#define PB3 PORTBbits.RB4
#define ALLPRESSED PORTAbits.RA2 + PORTAbits.RA4 + PORTBbits.RB4
#define LED PORTBbits.RB8

//create a structure to hold the application's timer data
typedef struct Time { uint8_t MINUTES; uint8_t SECONDS;} Time;


//function headers
void IOInit(void);
void updateTimer(int16_t);
void displayTimer();
void IOCheck(void);
void countDown(void);

void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void);

#endif	/* XC_HEADER_TEMPLATE_H */

