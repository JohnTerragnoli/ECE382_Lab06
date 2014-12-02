#include <msp430.h>
#include "header.h"

/*
 * implementation.c
 *
 *  Created on: Nov 20, 2014
 *      Author: C16John.Terragnoli
 *      Description: Contains useful functions for controlling the movement of the robot.
 *      For: Lab06
 */








//__________________________________________________
//FUNCTIONALITY FUNCTIONS
//__________________________________________________

/*
 * Goes forward, makes a full left and right turn, then makes
 *a half left and right turn, will eventually move it backwards.
 */
void basicFunctionality(){
	pauseBoth();
	pauseBoth();
	pauseBoth();

	moveBackward();
	halt();
	__delay_cycles(STRAIGHTTIME);

	moveForward();
	__delay_cycles(STRAIGHTTIME);
	fullTurnLeft();
	moveForward();
	__delay_cycles(STRAIGHTTIME);
	fullTurnRight();
	moveForward();
	__delay_cycles(STRAIGHTTIME);
	halfTurnLeft();
	moveForward();
	__delay_cycles(STRAIGHTTIME);
	halfTurnRight();
	moveForward();
	__delay_cycles(2*STRAIGHTTIME);

}

//__________________________________________________
//__________________________________________________















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
	bothOnBackward();
}




/*
 * Turns the robot 90 to the right.
 */
void fullTurnRight(){
    turnRight();
    __delay_cycles(1.5*STRAIGHTTIME);
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

/*
 * Stops the robot, whatever direction it is moving.
 */
void halt(){
	stopBoth();
	stopBothBackward();
}
//__________________________________________________
//__________________________________________________






























//_________________________________________________________________
//MEDIUM COMMANDS
//_________________________________________________________________

/*
 * Makes both motors move forwards.
 */
void bothOn(){
	rightOn();
	leftOn();
}


/*
 * Makes both motors move backwards!
 */
void bothOnBackward(){
	leftOnBackward();
	rightOnBackward();
}


/*
 * Stops the car until further notice.
 */
void stopBoth(){
    stopRight();
    stopLeft();
    __delay_cycles(STRAIGHTTIME);
    //rightOn();
    //leftOn();
}


/**
 * Stops the backwards movement.
 */
void stopBothBackward(){
	stopLeftBackward();
	stopRightBackward();
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
//_________________________________________________________________
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

/**
 * Stops the right wheel from moving backwards.
 */
void stopRightBackward(){
	P2DIR &= ~BIT5;
	P2SEL &= ~BIT5;
}


/*
 * Stops the left wheel from moving backwards.
 */
void stopLeftBackward(){
	P2DIR &= ~BIT1;
	P2SEL &= ~BIT1;
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

/*
 * Makes the left wheel turn backwards.
 */
void leftOnBackward(){
	P2DIR |=BIT1;
	P2SEL |=BIT1;
}

/*
 * Makes the right wheel turn backwards.
 */
void rightOnBackward(){
	P2DIR |=BIT5;
	P2SEL |=BIT5;
}


//_________________________________________________________________
//_________________________________________________________________
