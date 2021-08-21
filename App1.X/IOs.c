/*
 * This is file to keep track of all the functions used on the main program (main.c)
 * Taken from Driver Projects 2-4 of ENCM 511
 * 
 * Authors: Eduardo B. , Tahseen I. , William M.
 *
 * Created on October 12, 2020, 9:38 PM
 * 
 */

#include "xc.h"
#include "UART2.h"
#include "IOs.h"
#include "TimeDelay.h"

//flag for countdown finish: { 1 = finish, 0 = not finished }
uint8_t alarm = 0;

//initialize application timer
Time currentTime = {.MINUTES = 0, .SECONDS = 0};

void IOInit(void)
{    
    // setting up the push buttons
    TRISAbits.TRISA2 = 1;       // making RA2 a digital input --> PB1
    TRISAbits.TRISA4 = 1;       // making RA4 a digital input --> PB2
    TRISBbits.TRISB4 = 1;       // making RB4 a digital input --> PB3
    TRISBbits.TRISB8 = 0;       // setting up the output --> LED
    
    //pull up resistors (CN Registers)
    CNPU1bits.CN0PUE = 1;       // pull up resistor for RA4 (CN0)
    CNPU1bits.CN1PUE = 1;       // pull up resistor for RB4 (CN1)
    CNPU2bits.CN30PUE = 1;      // pull up resistor for RA2 (CN30)
    
    // setting up CN register interrupts
    CNEN1bits.CN0IE = 1;        // enable CN for RA4
    CNEN1bits.CN1IE = 1;        // enable CN for RB4
    CNEN2bits.CN30IE = 1;       // enable CN for RA2
    
    IPC4 = IPC4 | 0x7000;       // button CN interrupt priority set to 7
    
    IEC1bits.CNIE = 1;          // enable CN interrupt requests
    IFS1bits.CNIF = 0;          // clear T2 interrupt flag
        
    return;
}

void updateTimer(int16_t increment)
{
    //increment is a signed value
    currentTime.SECONDS += increment;
    
    if(currentTime.SECONDS < 0) // go from :00 back to :59
    {
        currentTime.SECONDS = 58 - currentTime.SECONDS; //change seconds on timer
        currentTime.MINUTES--; //decrement minute
    }
    
    else if(currentTime.SECONDS > 59)
    {
        currentTime.SECONDS %= 60; //change seconds on timer (n % 60)
        currentTime.MINUTES++; //increment minute
    }
    
    return;
}

void displayTimer(void)
{
    //update clock speed for displaying onto terminal faster?
    NewClk(8);
   
    //create string buffer for displaying time on terminal
    char displayBuffer[50];
    
    //10 characters, ensure format for timer is XXm:XXs
    sprintf(displayBuffer, "\r%02d m: %02d s" , currentTime.MINUTES, currentTime.SECONDS);
    
    //--ALARM display : 8 characters
    if(alarm)
        strcat(displayBuffer, "-- ALARM"); //when timer finishes, display --ALARM
    else
        strcat(displayBuffer, "        "); //fill vacant spaces of --ALARM
    
    //display buffer on terminal
    Disp2String(displayBuffer);
    
    //revert to regular clock speed when done displaying?
    NewClk(32);
    
    return;
}

void IOCheck(void)
{
    //timer on terminal display
    displayTimer();
    Idle(); //wait until button is pressed
    
    //only PB1 pressed : add a minute
    while(PB1 == 0 && (ALLPRESSED == 2))
    {
        alarm = 0;          //clear alarm message
        updateTimer(60);    //increase the seconds by 60 (1 minute)
        displayTimer();     //display timer
        delay_ms(200);      //0.2s to increment on the display        
    }
            
    //only PB2 pressed : add a second
    while(PB2 == 0 && (ALLPRESSED == 2))
    {
        alarm = 0;          //clear alarm message
        updateTimer(1);     //increase the seconds by 1
        displayTimer();     //display timer
        delay_ms(200);      //0.2s to increment on the display        
    }
    
    //only PB3 is pressed
    if(PB3 == 0 && (ALLPRESSED == 2))
    {
        T3Init(); //initialize and turn on timer 3 for PB3
        
        while(PB3 == 0) //check for signal to stabilize (debounce)
            Idle();
        
        IEC0bits.T3IE = 0; //disable the interrupt request of T3
        T3CONbits.TON = 0; //turn off the T3 timer for PB3
        
        //check if flag has been set 
        if(IFS0bits.T3IF == 0)
        {
            countDown();
        }
        
        else //flag for T3 was set, therefore reset to 00:00
        {
            currentTime.MINUTES = 0;   
            currentTime.SECONDS = 0;
        }
               
    }
    
}

void countDown() {
    //Clear CN 
    IFS1bits.CNIF = 0;
    while (alarm == 0)
    {
        //pause timer if a pb is pressed
        if (IFS1bits.CNIF == 1) {
            break;
        }
        //Once SEC reaches 0, revert SEC to 59 and decrease MIN by 1
        else if(currentTime.SECONDS == 0 && currentTime.MINUTES !=0 )
        {
            currentTime.MINUTES--;
            currentTime.SECONDS = 59;
        }
        //Once timer is finished, display ALARM and end loop
        else if(currentTime.SECONDS ==0 && currentTime.MINUTES == 0)
        {
            alarm = 1;
            LATBbits.LATB8 = 1;

            displayTimer();

            break;
        }
        else {
            currentTime.SECONDS--;
        }
        //if PB3 is pressed during countdown: either pause or reset
        if(PB3 == 0)
        {
             T3Init(); //turn on timer 3 for PB3
        
            //check for signal to stabilize (debounce)
            while(PB3 == 0)
            {
                Idle(); //when TR3 = PR3, then PB signal is stabilized
            }
            
            //check if PB3 held down for < 3 seconds, therefore pause
            if(IFS0bits.T3IF == 0)
            {
                break; //return to IOCheck()
            }

            //PB3 held down for > 3 seconds, therefore reset timer to 00:00
            else
            {
                currentTime.MINUTES = 0;   
                currentTime.SECONDS = 0;
                displayTimer();
                break;
            }
            
        }
        delay_ms(500);      // delay

        displayTimer();
        LATBbits.LATB8 = !LATBbits.LATB8;
        delay_ms(500);      //delay


    }


}

void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void)
{
    IEC1bits.CNIE = 0;   // turn off button interrupts
    delay_ms(30);        // 30ms debounce so that input stabilizes
    IEC1bits.CNIE = 1;   // turn button interrupts back on
    
    IFS1bits.CNIF = 0;   //clear interrupt flag for button press
    
    Nop();
    
}

