#include <msp430.h>
#include "header.h"

/*
 * implementation.c
 *
 *  Created on: Nov 20, 2014
 *      Author: C16John.Terragnoli
 */




void basicFunctionality(){
	pause();
	pause();
	pause();
	pause();
	pause();
	moveForward();
	fullTurnRight();
	moveForward();
	fullTurnLeft();
	moveForward();
	halfTurnRight();
	moveForward();
	halfTurnLeft();
	moveForward();

}




//__________________________________________________
//REQUIREMENT FUNCTIONS
//__________________________________________________


/*
 * Moves forward indefinitely
 */
void moveForward(){
	bothOn();
}




/*
 * Move it backwards.  DOES NOT WORK
 */
void moveBackward(){
	P2OUT |= (BIT1 | BIT3);
	P2OUT &= ~BIT2;
	P2DIR |= (BIT1 | BIT3);
	P2DIR &= ~BIT2;
	//P2OUT ^=BIT3;
	//P2DIR ^=BIT3;
    __delay_cycles(STRAIGHTTIME);
}




/*
 * Turns the robot 90 to the right.
 */
void fullTurnRight(){
    turnRight();
    __delay_cycles(FULLTURN);
}



/*
 * Turns the robot 45 to the right.
 */
void halfTurnRight(){
    turnRight();
    __delay_cycles(HALFTURN);
}



/*
 * Turns the robot 90 to the left.
 */
void fullTurnLeft(){
   turnLeft();
    __delay_cycles(FULLTURN);
}



/*
 * Turns the robot 45 to the left.
 */
void halfTurnLeft(){
	turnLeft();
    __delay_cycles(HALFTURN);
}
//__________________________________________________
//__________________________________________________






























//_________________________________________________________________
//MEDIUM COMMANDS
//_________________________________________________________________

void bothOn(){
	rightOn();
	leftOn();
	__delay_cycles(STRAIGHTTIME);
}


/*
 * Stops the car until further notice.
 */
void stopBoth(){
    stopRight();
    stopLeft();
    __delay_cycles(STRAIGHTTIME);
    rightOn();
    leftOn();
}



/*
 * Pauses the car for one second
 */
void pauseBoth(){
	stopRight();
	stopLeft();
	 __delay_cycles(STRAIGHTTIME);
	rightOn();
	leftOn();
}


/*
 * Sets the car up for any kind of right turn
 */
void turnRight(){
	leftOn();
	stopRight();
}


/*
 * Sets the care up for any kind of left turn.
 */
void turnLeft(){
	rightOn();
	stopLeft();
}
//_________________________________________________________________
//_________________________________________________________________



























//_________________________________________________________________
//VERY BASIC FUNCTIONS

/*
 * Stops the right wheen until further notice.
 */
void stopRight(){
    P2DIR &= ~BIT4;
    P2SEL &= ~BIT4;
}



/*
 * Stops the left wheel until further notice.
 */
void stopLeft(){
    P2DIR &= ~BIT2;
    P2SEL &= ~BIT2;
}



/*
 * Starts the right wheel until further notice.
 */
void rightOn(){
    P2DIR |= BIT4;
    P2SEL |= BIT4;
}


/*
 * Starts the left wheel until further notice.
 */
void leftOn(){
    P2DIR |= BIT2;
    P2SEL |= BIT2;
}


