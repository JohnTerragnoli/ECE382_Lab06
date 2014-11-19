ECE382_Lab06
============

#Purpose

The purpose of this lab is to use pulse width modulation to use an MSP430 to control a small robot car.  This will be done with the help of other devices, such as driver and regulator chips, IR sensors, and a remote.  

#Prelab
**Things to accomplish**

1. Use A regulator to power the MSP430.
2. How to use hardware to achieve robot control (driver).
3. Consider additional hardware you'll need (regulator, motor driver chip, decoupling capacitor).
4. Which pins will output which signals you need.
5. Which side of the motor will you attach these signals to.
6. How will you connect these extra parts?
7. How to use these signals to achieve forward/back/left/right movement.
8. consider how you will setup the PWM subsystem to achieve control.
9. what are the registers you'll need to use.
10. which bits in the registers are important?
11. what's the initialization sequence you'll need?
12. Try to build simple commands, such as moving one motor one direction.
13. Combine simple commands to make complex ones, like just moving in a specific direction.
14. Determine worst-case current draw from your motors (**motor stall current**)


**Remember**

1. Never send voltage to both terminals at once.  

##Hardware Planning Process

**How to Power the MSP430**
1. The MSP430 can only accept 3.3v, whereas the source we have can only output 5v.  We need to use a regulator to drop this voltage.  Should maybe use a small capacitor across the 5V rail to smoth high frequency noise.  This capacitor should be ~0.1uF.  
2. To determine how this should be hooked up, this datasheet was consulted.  It deals with [voltage regulators](http://www.ece382.com/datasheets/LD1117V33.pdf).  This schematic is used below:

![alt tag](https://raw.githubusercontent.com/JohnTerragnoli/ECE382_Lab06/master/2.%20Photos/Prelab/TO-220%20Datasheet%20Schematic.PNG "TO-220 Config")

This was then attached to the MSP430 in the following way:

![alt tag](https://raw.githubusercontent.com/JohnTerragnoli/ECE382_Lab06/master/2.%20Photos/Prelab/Regulator%20Schematic.JPG "MSP Regulator")


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


![alt tag](https://raw.githubusercontent.com/JohnTerragnoli/ECE382_Lab06/master/2.%20Photos/Prelab/Driver%20Schematic.JPG "Driver")


**Note:** The four bottom boxes should all be for the "left" motor. I am sorry for writing this down wrong.  

**Choose Outputs for the MSP340**

For simplicity sake, the connections to GND and the regulator made earlier will not be shown again. The outputs will be chosen as below: 

P1.0 = Input Left Low

P1.1 = Input Left High

P2.0 = Input Right High

P2.1 = Input Right Low



![alt tag](https://raw.githubusercontent.com/JohnTerragnoli/ECE382_Lab06/master/2.%20Photos/Prelab/MSP%20Outputs%20Schematic.JPG "MSP Outputs")

At this point in the lab I am just assigning the titles "high" and "low" with consistency.  These names might change later if they prove inconvenient.  


**Adding Extra Parts to Circuit**

Only the decoupling capacitor will be added at this moment.  It will be ~100uF, as indicated in the directions, and will be placed across the 5v bus.  Of course the positive end must be placed in the 5v bus!!  No individual schematic is needed for this.  

At this point, all of the hardware for basic functionality was completed.  When A functionality is pursued later, hardware will be developed for that.  


The total schematic can be seen below: 
![alt tag](https://raw.githubusercontent.com/JohnTerragnoli/ECE382_Lab06/master/2.%20Photos/Prelab/Complete%20Hookup.PNG "Total Schematic")

This was done using Fritzing software.  To view the file for this software, use [this link](https://github.com/JohnTerragnoli/ECE382_Lab06/blob/master/2.%20Photos/Prelab/Lab06%20Prelab%20Schematic.fzz).  


##Software Planning Process


After this hardware was determined, the software was handled.  This is picking up around number 7 in the list made up top. 

**Achieving Movement**
When a positive voltage is placed across a motor, it will spin forward.  When a negative voltage is applied, it will spin backwards.  To achieve forward movement then, a positive voltage must be applied to both motors.  To make the car turn right, a positive voltage should be applied to the left wheel and the right wheel with no voltage.  To turn left, do the opposite.  The duration of how long a wheel is stopped will determine how sharp the turn is.  To move the car backwards, just apply a negative voltage to the motors.  This can be done easily done by feeding the input to the usually lower terminal of the motor to be above the usually high voltage of the motor.  

**Setting up the PWM**
This will be fairly complicated, since it has never been approached before.  To do this, we will first approximate a PWM of 50%.  It was suggested by Captain Trimble to not exceed 60%, but I would like to play it safe and not blow anything up.  To set this up, I just need to transmit a "high" or the desired voltage difference between my two output pins 50% of the time by constantly turning it on and off.  To do this, I will need two different timers.  This will be useful in case I decide to change the duty cycle later on.  

**Registers to Use**
The TACCTLx, Capture/Compare Control Register will give into on when certain numbers are hit.  
Other handy registers are TACCR0 and TACCR1.  These will be the different numbers we are checking for when counting.  These two numbers will determine the PWM.  When we count to the lower one, turn on the power.  When we count to the higher one, turn off the power.  Start from the beginning and repeat!!  To test this out I will first start with creating a simple program, like a light, and trying to change the brightness using PWM.  

In these registers, the most important bits will be TACTL, the timer which will be stopped and cleared when needed P1/P2DIR and P1/P2SEL, which will help determine the outputs

**Initialization Sequence**
1. Stop the watchdog timer.  
2. Set up the outputs (of the MSP430)
3. Clear the timers
4. decide which clock to use
5. Set the values for TACCR0 and TACCR1
6. Determine the value of the voltage which will be sent to the driver.  
6. Begin the while loop.  



**Simple Commands**
1. spinForward().  This will be done by ensuring that a positive voltage is applied across the motor.  
2. stopSpin().  This will be done by ensuring that no voltage is applied across the motor.
3. spinBackwards().   This will be done by ensuring that a negative voltage is applied across the motor. 


**Medium Commands**
1. moveForward(): ensure both wheels are spinning forward.  Calls the spinForward() command on both wheels.  Ensure that the voltage applied to each wheel is the same.  
2. halt(): makes the car stop moving.  Calls stopSpin() on each motor at the same time. 
3. moveBackwards(): ensure both wheels are spinning backwards.  Calls the spinBackwards() command on both wheels.  Ensure that the voltage applied to each wheel is the same.  
4. fullRight(): calls spinForward() for left motor.  calls stopSpin on right motor for 1 second, then calls spinForward for the right motor as well.  
5. fullLeft(): same concept as fullRight() except switching the wheels used. 
6. halfRight(): same as fullRight except calls stopSpin for 0.5 seconds.  
7. halfLeft():  same as fullLeft except calls stopSpin for 0.5 seconds.  
8. hold(time):  keeps the voltage state of the motors (and the PWM) for a certain amount of seconds).  

**High Commands/Demonstration Commands**
1. basicFunctionality(): moves the car backwards.  halts, moves forwards.  Makes a half right turn, goes straight.  Makes a half left turn, goes straight.  Makes a full right turn, goes straight.  Makes a full left turn, goes straight; moveBackwards(), hold(2), halt(), hold(1), moveForward(), hold(2), halfRight(), hold(1), halfLeft(), hold(1), fullRight(), hold(1), fullLeft(), hold(1), moveStraight(1000).  
2. move will be developed for bragging rights and such later on.  


The flowchart for the code may be seen below: 
![alt tag](https://raw.githubusercontent.com/JohnTerragnoli/ECE382_Lab06/master/2.%20Photos/Prelab/Prelab%20Flowchart.JPG "Flowchart")

**Worst Case Draw Current**
This has not been found yet.  It will be found in the lab soon.  

##Software Planning Process



#Required Functionality



#A Functionality






#Documentation:
##Prelab
Only used the datasheets and the picture of the hardware hook up that was give to us by our teachers.  

##Required Functionality

##A Functionality
