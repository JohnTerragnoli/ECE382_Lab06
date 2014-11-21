#include <msp430.h>
#include "header.h"

/*
 * implementation.c
 *
 *  Created on: Nov 20, 2014
 *      Author: C16John.Terragnoli
 */




void basicFunctionality(){
	moveForward();
	fullTurnRight();
	moveForward();
	fullTurnLeft();
	moveForward();
	halfTurnRight();
	moveForward();
	halfTurnLeft();
	moveForward();
	pause();
	pause();
	pause();
	pause();
	pause();
}


/*
 * Turns the robot 90 to the right.
 */
void fullTurnRight(){
    P2SEL |= BIT2;
    P2SEL |= BIT4;
    P2SEL &= ~BIT4;
    __delay_cycles(FULLTURN);
}

/*
 * Turns the robot 45 to the right.
 */
void halfTurnRight(){
    P2SEL |= BIT2;
    P2SEL |= BIT4;
    P2SEL &= ~BIT4;
    __delay_cycles(HALFTURN);
}

/*
 * Turns the robot 90 to the left.
 */
void fullTurnLeft(){
    P2SEL |= BIT2;
    P2SEL |= BIT4;
    P2SEL &= ~BIT2;
    __delay_cycles(FULLTURN);
}

/*
 * Turns the robot 45 to the left.
 */
void halfTurnLeft(){
    P2SEL |= BIT2;
    P2SEL |= BIT4;
    P2SEL &= ~BIT2;
    __delay_cycles(HALFTURN);
}


/*
 * moves forward for one second.
 */
void moveForward(){
    //goes straight.
    P2SEL |= BIT2;
    P2SEL |= BIT4;
    __delay_cycles(STRAIGHTTIME);
}

/*
 * Move it backwards
 */
void moveBackward(){
	P2OUT |= (BIT1 | BIT3);
	P2DIR |= (BIT1 | BIT3);

	//P2OUT ^=BIT3;
	//P2DIR ^=BIT3;
    __delay_cycles(STRAIGHTTIME);
}

/*
 * pauses the car for a desired amount of time.
 */
void pause(){
    //STOP THE WHEELS.
    P2SEL &= ~BIT2;
    P2SEL &= ~BIT4;
    __delay_cycles(STRAIGHTTIME);
}




/*
 * stops the wheels, then lets them run forward again.
 */
void startStop(){
	pause();
	straightForward();
}


/*
 * Moves the Car forward then backwards.
 */
void backAndForth(){
	moveForward();
	pause();
	moveBackward();
	pause();
}




