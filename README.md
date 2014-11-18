ECE382_Lab06
============

#Purpose


#Prelab
**Things to accomplish**

1. Use A regulator to power the MSP430.
2. How to use hardware to achieve robot control.
2. Which pins will output which signals you need.
3. Which side of the motor will you attach these signals to.
4. How to use these signals to achieve forward/back/left/right movement.
5. consider how you will setup the PWM subsystem to achieve control.
6. what are the registers you'll need to use.
7. which bits in the registers are important?
8. what's the initialization sequence you'll need?
9. Consider additional hardware you'll need (regulator, motor driver chip, decoupling capacitor).
10. How will you connect these extra parts?
11. Try to build simple commands, such as moving one motor one direction.
12. Combine simple commands to make complex ones, like just moving in a specific direction. 
13. Determine worst-case current draw from your motors (**motor stall current**)


**Remember**

1. Never send voltage to both terminals at once.  

##Planning Process

**How to Power the MSP430**
1. The MSP430 can only accept 3.3v, whereas the source we have can only output 5v.  We need to use a regulator to drop this voltage.  Should maybe use a small capacitor across the 5V rail to smoth high frequency noise.  This capacitor should be ~0.1uF.  
2. To determine how this should be hooked up, this datasheet was consulted.  It deals with [voltage regulators](http://www.ece382.com/datasheets/LD1117V33.pdf).  This schematic is used below:

![alt tag](https://raw.githubusercontent.com/JohnTerragnoli/ECE382_Lab06/master/2.%20Photos/Prelab/TO-220%20Datasheet%20Schematic.PNG "TO-220 Config")

This was then attached to the MSP430 in the following way:

INSERT PICTURE OF 5V, REGULATOR, AND MSP430 HOOKUP

**Adding Large Coupling Capacitor**

Only the first will be added at this moment.  It will be ~100uF, as indicated in the directions, and will be placed across the 5v bus.  Of course the positive end must be placed in the 5v bus!!  No individual schematic is needed for this.  

**Adding the Driver**
The purpose of the driver allows the MSP430 to control the motors without burning up due to the current requirements.  The datasheet for the Quadruple Half Driver can be seen [here](http://www.ece382.com/datasheets/SN754410.pdf).  

Its pinout and function table may be seen below: 

![alt tag](https://raw.githubusercontent.com/JohnTerragnoli/ECE382_Lab06/master/2.%20Photos/Prelab/Driver%20Pinout%20and%20Function%20Table.PNG "Driver Config")

Next, to understand how to hook this up to the motors and to the MSP430, we must first decide how the program will run.  For simplicity sake, I plan on using two individual output signals for the forward direction, and a common output signal for the reverse.  This should make the design easier since we are not required to turn backwards. Therefore, hooking up the driver to the MSP430 and the motos should be relatively easy.  

First, we note that V_cc1 must be between 4.5 and 5.5 volts, as seen in the datasheet.  Therefore, V_ccs1 must be hooked up to the 5v source, which is the only source within range available on the robot.  

Secondly, V_cc2 is meant for the outputs, as noted on the datasheet.  It must be between the range of 4.5 and 36v.  Knowing that the voltage to provide the motors must be greater than 5v, we know that V_cc2 must be hooked up to the 12v source, the only other available source on the chip.  Also, I remember when reading the prelab instructions that if 12v are used, then the duty cycle may not exceed 60%.  This will hurt the motor.  Just something to keep in mind for later.  

Thirdly, the 1,2 EN and 3,4 EN pins will be involved in the logic of the circuit.  Their main purpose is to enable the outputs when their voltage is high.  It would make the most sense for the purposes of this design to make them always high.  Then, the outputs 1-4, or the outputs which are sent to the motors, will be determined entirely by the code.  I think this will lead to fewer complications later on.  Therefore, to do this, the 5v bus will be connected to pins 1, 9, and 16.  

Fourth, the ground must be established for the driver chip.  Based on the pinout, GND is pins 4, 5, 12, and 13.  Therefore, all of these are connected to GND.  

Fifth, I will decide where the inputs (the outputs from the MSP430 go on the driver) will go on the driver chip.  I decide to put one set into pins 3A and 4A, which are pins 10 and 15, respectively.  These inputs will control the right motor through pins 11 and 14, which are ground and input for the right motor, respectively.  Then, the next set of intputs will go into 1A and 2A, which are pins 2 and 7, respectively.  It's outputs are pins 3 and 6, which correspond to input high voltage and the ground input voltage for the Left motor.  Don't worry, this will be shown in a schematic.  




#Required Functionality


#B Functionality


#A Functionality






#Documentation:
##Prelab
##Required Functionality
##B Functionality
##A Functionality
