#include <msp430.h>
#include "header.h"


void main(void) {

    WDTCTL = WDTPW|WDTHOLD;                 // stop the watchdog timer

    P2DIR |= BIT2;							// P2.2 is associated with TA1CCR1
    P2SEL |= BIT2;							// P2.2 is associated with TA1CCTL1

    P2DIR |= BIT4;							// P2.4 is associated with TA1CCR2
    P2SEL |= BIT4;							// P2.4 is associated with TA1CCTL2

	TA1CTL = ID_3 | TASSEL_2 | MC_1;		// Use 1:8 presclar off MCLK
    TA1CCR0 = 100;						// set signal period

    TA1CCR1 = 40;
    TA1CCTL1 = OUTMOD_7;					// set TACCTL1 to Reset / Set mode

    TA1CCR2 = 60;
    TA1CCTL2 = OUTMOD_3;					// set TACCTL2 to Set / Reset mode


    while (1) {
/*
    	pauseBoth();
    	moveForward();
    	 __delay_cycles(STRAIGHTTIME);


    	 //code for backwards!!!
    	halt();

    	moveBackward();

    	halt();

    	stopBothBackward();

*/





    	basicFunctionality();

    } // end loop

} // end main
