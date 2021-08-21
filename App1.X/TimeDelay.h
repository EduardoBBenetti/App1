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

//function headers
void delay_ms(uint16_t);
void T3Init(void);
void __attribute__ ((interrupt,no_auto_psv)) _T2Interrupt(void);
void __attribute__ ((interrupt,no_auto_psv)) _T3Interrupt(void);

#endif	/* XC_HEADER_TEMPLATE_H */

