//-----------------------------------------------------------------
// Name:	Coulston
// File:	lab5.c
// Date:	Fall 2014
// Purp:	Demo the decoding of an IR packet
//-----------------------------------------------------------------
// Name:	JP Terragnoli
// Date:	Fall 2014
// Purp:	Demo the decoding of an IR packet, and noving a robot car
//			with an IR remote.
//-----------------------------------------------------------------

#include <msp430g2553.h>
#include "A_Functionality.h"


int32	packet;
int16	packetData[48];
int8	newIRPacket = FALSE;
int8	packetIndex = 0;

// -----------------------------------------------------------------------
// -----------------------------------------------------------------------
void main(void) {

	initMSP430();				// Setup MSP to process IR and buttons

	moveForward();

	TA1CTL = ID_3 | TASSEL_2 | MC_1;		// Use 1:8 presclar off MCLK
	TA1CCR0 = 100;						// set signal period

	TA1CCR1 = 50;
	TA1CCTL1 = OUTMOD_7;					// set TACCTL1 to Reset / Set mode

	TA1CCR2 = 50;
	TA1CCTL2 = OUTMOD_3;					// set TACCTL2 to Set / Reset mode







	while(1)  {

		//only go if we have a new packet coming in.
		if(newIRPacket){

			_disable_interrupt();

			//control the car with the remote ;)
			//this should be killer, like my squadron.
			if(packet == CH_UP){
				moveForward();
			}else if(packet == CH_DW){
				moveBackward();
			}else if(packet == VOL_UP ){
				fullTurnRight();
				moveForward();
			}else if(packet == VOL_DW){
				fullTurnLeft();
				moveForward();
			}else{
				moveForward();
			}



			//makes sure the interrupt is ready for use again!.
			_enable_interrupt();



			//makes sure we dont decode the signal until the complete next on is received again.
			initMSP430();
			newIRPacket= FALSE;
		}


	} // end infinite loop
} // end main

// -----------------------------------------------------------------------
// In order to decode IR packets, the MSP430 needs to be configured to
// tell time and generate interrupts on positive going edges.  The
// edge sensitivity is used to detect the first incoming IR packet.
// The P2.6 pin change ISR will then toggle the edge sensitivity of
// the interrupt in order to measure the times of the high and low
// pulses arriving from the IR decoder.
//
// The timer must be enabled so that we can tell how long the pulses
// last.  In some degenerate cases, we will need to generate a interrupt
// when the timer rolls over.  This will indicate the end of a packet
// and will be used to alert main that we have a new packet.
// -----------------------------------------------------------------------
void initMSP430() {

	IFG1=0; 					// clear interrupt flag1
	WDTCTL=WDTPW+WDTHOLD; 		// stop WD

	BCSCTL1 = CALBC1_8MHZ;
	DCOCTL = CALDCO_8MHZ;

	P2SEL  &= ~BIT6;						// Setup P2.6 as GPIO not XIN
	P2SEL2 &= ~BIT6;
	P2DIR &= ~BIT6;
	P2IFG &= ~BIT6;						// Clear any interrupt flag
	P2IE  |= BIT6;						// Enable PORT 2 interrupt on pin change

	HIGH_2_LOW;
	//P1DIR |= BIT0 | BIT6;				// Enable updates to the LED
	//P1OUT &= ~(BIT0 | BIT6);			// An turn the LED off

	TA0CCR0 = 0xFFFF;					// create a 16mS roll-over period
	TACTL &= ~TAIFG;					// clear flag before enabling interrupts = good practice
	TACTL = ID_3 | TASSEL_2 | MC_1;		// Use 1:1 presclar off MCLK and enable interrupts

	_enable_interrupt();
}

// -----------------------------------------------------------------------
// Since the IR decoder is connected to P2.6, we want an interrupt
// to occur every time that the pin changes - this will occur on
// a positive edge and a negative edge.
//
// Negative Edge:
// The negative edge is associated with end of the logic 1 half-bit and
// the start of the logic 0 half of the bit.  The timer contains the
// duration of the logic 1 pulse, so we'll pull that out, process it
// and store the bit in the global packet variable. Going forward there
// is really nothing interesting that happens in this period, because all
// the logic 0 half-bits have the same period.  So we will turn off
// the timer interrupts and wait for the next (positive) edge on P2.6
//
// Positive Edge:
// The positive edge is associated with the end of the logic 0 half-bit
// and the start of the logic 1 half-bit.  There is nothing to do in
// terms of the logic 0 half bit because it does not encode any useful
// information.  On the other hand, we going into the logic 1 half of the bit
// and the portion which determines the bit value, the start of the
// packet, or if the timer rolls over, the end of the ir packet.
// Since the duration of this half-bit determines the outcome
// we will turn on the timer and its associated interrupt.
// -----------------------------------------------------------------------
#pragma vector = PORT2_VECTOR			// This is from the MSP430G2553.h file

__interrupt void pinChange (void) {

	int8	pin;
	int16	pulseDuration;			// The timer is 16-bits

	if (IR_PIN)		pin=1;	else pin=0;

	switch (pin) {					// read the current pin level
		case 0:						// !!!!!!!!!NEGATIVE EDGE!!!!!!!!!!


			pulseDuration = TAR;
			//determines if the incomming signal is a 0 or a 1.  This is determined by
			//the length with the signal is held high, as defined in the header.
			if((pulseDuration<maxLogic0Pulse) && (pulseDuration>minLogic0Pulse)){
				packet = (packet << 1) | 0;
			}
			if((pulseDuration<maxLogic1Pulse) && (pulseDuration>minLogic1Pulse)){
				packet = (packet << 1) | 1;
			}


				packetData[packetIndex++] = pulseDuration;
			TACTL = 0;				//turns off the timer
			LOW_2_HIGH; 				// Setup pin interrupr on positive edge
			break;

		case 1:							// !!!!!!!!POSITIVE EDGE!!!!!!!!!!!
			TAR = 0x0000;						// time measurements are based at time 0
			TA0CCR0=0xFFFF;		//need to make sure we set the number we are counting up to.
								//this should be enough time.
			TACTL = ID_3 | TASSEL_2 | MC_1 | TAIE;	//this just sets up how we are counting
											//each count will take 8 times as long.
			HIGH_2_LOW; 						// Setup pin interrupr on positive edge
			break;
	} // end switch

	P2IFG &= ~BIT6;			// Clear the interrupt flag to prevent immediate ISR re-entry

} // end pinChange ISR



// -----------------------------------------------------------------------
//	The main purpose of this interrupt is to create a time gap for every wave that comes in,
//	to ensure that the end part of it is not cut off.  Also, it serves to reset the packet index
//	so that the new packet can be recorded in it's entirety and recorded from the beginning, so
//	the signal is not mixed up or placed out of order.  Also, it lets the system
//	know that a new packet has arrived and that it has been recorded.  This allows the
//	if statement in the while loop to be satisfied so that we can carry out the purpose
//	of the signal received.
// -----------------------------------------------------------------------
#pragma vector = TIMER0_A1_VECTOR			// This is from the MSP430G2553.h file
__interrupt void timerOverflow (void) {

	TACTL = 0;
	TACTL &= ~TAIE;
	packetIndex = 0;  //get ready for the next one.
	newIRPacket = TRUE;  //decode the new signal!!!
	TACTL &= ~TAIFG;  //resets the flag :)
}
