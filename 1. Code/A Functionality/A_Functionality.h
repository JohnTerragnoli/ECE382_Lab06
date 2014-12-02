/*
 * header.h
 *
 *  Created on: Nov 20, 2014
 *      Author: C16John.Terragnoli
 *      Description: Contains the linkage between the function defitions and the main method where they are called.
 *      For: Lab06
 */

typedef		unsigned char		int8;
typedef		unsigned short		int16;
typedef		unsigned long		int32;
typedef		unsigned long long	int64;

#define		TRUE				1
#define		FALSE				0



//-----------------------------------------------------------------
// Function prototypes found in lab5.c
//-----------------------------------------------------------------
void initMSP430();
__interrupt void pinChange (void);
__interrupt void timerOverflow (void);


//-----------------------------------------------------------------
// Each PxIES bit selects the interrupt edge for the corresponding I/O pin.
//	Bit = 0: The PxIFGx flag is set with a low-to-high transition
//	Bit = 1: The PxIFGx flag is set with a high-to-low transition
//-----------------------------------------------------------------

#define		IR_PIN			(P2IN & BIT6)
#define		HIGH_2_LOW		P2IES |= BIT6
#define		LOW_2_HIGH		P2IES &= ~BIT6


#define		averageLogic0Pulse	590
#define		averageLogic1Pulse	1626
#define		averageStartPulse	4411

#define		marginOfError		150
#define 	packetLength		33
#define		otherData			0xFF

#define		minLogic0Pulse		averageLogic0Pulse - marginOfError
#define		maxLogic0Pulse		averageLogic0Pulse + marginOfError
#define		minLogic1Pulse		averageLogic1Pulse - marginOfError
#define		maxLogic1Pulse		averageLogic1Pulse + marginOfError
#define		minStartPulse		averageStartPulse - marginOfError
#define		maxStartPulse		averageStartPulse + marginOfError




//all of the buttons specific to remote #3.
#define		PWR		0x02FD48B7
#define		ONE		0x02FD807F
#define		TWO		0x02FD40BF
#define		THR		0x02FDC03F
#define 	ZERO	0x02FD00FF

#define		VOL_UP	0x02FD58A7
#define		VOL_DW	0x02FD7887
#define		CH_UP	0x02FDD827
#define		CH_DW	0x02FDF807


#define STRAIGHTTIME 1000000
#define FULLTURN (STRAIGHTTIME*0.92)
#define HALFTURN (FULLTURN*0.5)

#ifndef HEADER_H_

#define HEADER_H_







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

