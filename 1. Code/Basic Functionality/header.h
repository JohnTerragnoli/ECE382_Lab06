/*
 * header.h
 *
 *  Created on: Nov 20, 2014
 *      Author: C16John.Terragnoli
 *      Description: Contains the linkage between the function defitions and the main method where they are called.
 *      For: Lab06
 */

#ifndef HEADER_H_

#define HEADER_H_


#define STRAIGHTTIME 1000000
#define FULLTURN (STRAIGHTTIME*0.92)
#define HALFTURN (FULLTURN*0.5)




void basicFunctionality();










//testing half-functions;
void backAndForth();
void fullTurnRight();
void fullTurnLeft();
void halfTurnRight();
void halfTurnLeft();
void moveForward();
void moveBackward();
void halt();

void startStop();
void pauseBoth();
void stopBoth();
void stopRight();
void stopLeft();
void rightOn();
void leftOn();
void bothOn();



//backwards methods
void leftOnBackward();
void rightOnBackward();
void stopRightBackward();
void stopLeftBackward();

void stopBothBackward();
void bothOnBackward();





#endif /* HEADER_H_ */
