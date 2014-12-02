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

**IR Sensor**
After I did all of this, I thought it would be smart to add the IR sensor into the circuit early.  To do this, all I needed was ground, power, and the output of the IR (which will be input into the MSP430).

Unfortunately, there was no datasheet for the IR sensor used in class.  However, the pinout is still known.  If looking at the front dot, the right pin is power, the middle pin is ground, and the left pin is the output.  The input voltage will be 3.3v, since this is traveling right to the MSP430.  The ground will be connected witht the rest.  The output of the IR will go to pin P1.7, which was chosen at random.  

The total schematic including the IR sensor can be seen below: 
![alt tag](https://raw.githubusercontent.com/JohnTerragnoli/ECE382_Lab06/master/2.%20Photos/Prelab/Total%20Circuit%20with%20IR%20Sensor.PNG "Everything Hardware")

Note: Unfortunately, the correct Fritzing part for the IR sensor could not be found.  Therefore, I just included a part which looked exactly like the sensor, because the only reason for Fritzing is to get the picture anyway.  

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
This has not been found yet.  It will be found in the lab soon, since it is not required for the prelab.  Basically what will be done is to run a specific PWM through the circuit, stopping the wheel forcefully, and measuring the spike in current.  




#Required Functionality

##Creation/Debugging Process

1. For safety purposes, the motor stall current for the motor was then found.  To do this I simply put the voltage source in series with my robot.  I let the wheel run, and then abruptly stopped it.  I noticed that the ampere’s would jumped a little past 1A.  I lowered the voltage until it did not jump above 1A.  The voltage for this to occur was approximately 6.5v.  This means that 6.5/12 = 54%.  The duty cycle can then not go above 54%.  I made my duty cycle 50%, which is within the limits.  
2. The design chosen in the prelab was over complicated.  I did take the time to physically construct my circuit.  When I tried to write my own code though, I got fairly confused and could not get anything to work.
3. To fix this issue, I changed my design to match the schematic Dr. Coulston wrote on the board to help us.  The purpose of his schematic was to give us a starting point.  He also provided code for this starting point.  This starting point can be seen below: 


![alt tag](https://raw.githubusercontent.com/JohnTerragnoli/ECE382_Lab06/master/2.%20Photos/Functionality/White%20Board%20Diagram.JPG "White Board Drawing")


4. The actual and total pinout I used can be seen below: 

![alt tag](https://raw.githubusercontent.com/JohnTerragnoli/ECE382_Lab06/master/2.%20Photos/Functionality/A%20Functionality%20Schematic.PNG "Full Pinout")


5. The handrawn connections, which were helpful for me to look at, can be seen here: 

![alt tag](https://raw.githubusercontent.com/JohnTerragnoli/ECE382_Lab06/master/2.%20Photos/Functionality/Handrawn%20Connections.JPG "Full Pinout")


6. I then built the schematic on the fly.  Using the code given, I was able to get my car to spin in a circle.  This seemed really exciting to me.  I then commented out different sections of the code to understand what they do.  I realized that P2SEL and P2DIR were both needed to set the wheel direction.  I decided to make functions for these then commands.   I created four functions.  One to either turn forward or backward, or on or off forwards and backwards.  
7. One of the hardest parts was to understand how to get the wheel to move backwards.  What I realized that that the second input to the motor P2.1, was going to be the other input to the motor.  Therefore, if I would P2.1 to a high of 12v, then this would jump over the approximately 6v input to the other side of the motor to make the wheel spin forward.  Because the motor is DC, if a negative voltage is applied across the terminals, then the wheel will spin backwards.  I messes around with what bits to use, but finally settled on BIT1 and BIT5 to make the left and right wheel move back respectively. 
8. A simple problem I had was that once I set the bits 1 and 5 to make the wheel move backwards, I could never change the direction again.  I realized that this was because I never reset the bits 1 and 5 when I wanted to move forward again.  To fix this I simply reset the “backwards” bits when I wanted to travel forward and reset the “forward” bits when I wanted to ravel backwards.  After this occurred, I was able to output what I wanted to the car.  
9. Using the simple commands for starting/stopping and wheel forward/backwards, I was able to create the medium level commands.  These were somewhat unnecessary, if only for simplicity sake.  What they do is just make the wheels work in synchronous, when desired.  
10. Then, I combined these medium commands to functional commands, or moving the car in four directions and two half directions.  This was fairly simple.  A command which was very useful was the delay command, as seen in Dr. Coulston’s code, which was useful in creating turns of different angles.  To do a left turn, the left wheel needs to stop for a certain amount of time while the right one continues to spin.  To do a right turn, the wheels need to do the opposite.  
11. The, I used the delay command to separate the movements of my robot so that it would be easily viewed.  I just combined all of the directional commands into a basic functionality function for simplicity sake.  Captain Trimble actually checked off on this on 11/21/14.  


The text files for required functionality are shown below: 

[Basic Main](https://raw.githubusercontent.com/JohnTerragnoli/ECE382_Lab06/master/1.%20Code/Basic%20Functionality/main.c)
[Basic Header](https://raw.githubusercontent.com/JohnTerragnoli/ECE382_Lab06/master/1.%20Code/Basic%20Functionality/header.h)
[Basic Implementation](https://raw.githubusercontent.com/JohnTerragnoli/ECE382_Lab06/master/1.%20Code/Basic%20Functionality/implementation.c)

#A Functionality

##Creation/Debugging Process

1. First, to do this, I used my code from lab 5 and made sure my remote kept working.  To do this, I temporarily removed the MSP430 from the robot, wired up the IR sensor, and ran my lab05 code for basic functionality.  It still worked, which was a good thing. 
2. I then copied all of my code for the lab 05 basic functionality into my new file for A functionality in lab 06.  This included the header file.  Originally, I used my lab06 basic functionality as the basic shell to achieve A functionality with.  However, this led to multiple issues, and so when I started over I decided to do it the other way.  It worked  
3. The, what I did was wire the IR sensor to the logic analyzer.  I ran the code, pressed a button, and looked at the waveform on the screen.  To my surprise, nothing showed up.  At first I had no idea what was wrong.  Irrationally, I spent a great deal of time checking the code for the MSP430.  What I they did was take a different remote from the class and press a button on it.  The IR sensor and the logic analyzer did in fact pick up the signal from the second remote.  This meant that these two pieces of equipment were working and that my remote probably just wasn’t outputting anything.  Therefore, I went to the lab table and replaced the batteries.  I tried my remote again, and the waveform did show upon the logic analyzer. 
4. I then did a quick check to see if my waveform matched the values I will be using in my header.  They still matched form the last lab.  
5. Once I did this, I tried creating logic statement within the infinite while loop, just to see if the MSP430 was reading the signals correctly.  To try and test this, I tried using a variable, but CCS gave me a really hard time.  I tried using an LED, but this did not work either.  Then, I decided to just go for the gold and include a couple functions, like start not moving and move forward when the up button was pressed.  To my surprise it actually worked and I went over and told Sabin right away.  
6. Once I realized these commands worked, I made four different if statements for the four directions.  I played it
7. When I did this, I realized that on turns the wheel are only stopping half of the time.  This actually did not make sense to me.  While it is not in line with my predictions, it actually make the car turn faster, which is awesome.  Additionally, the button for right or left can just be held down to make it move that direction.  I thought it might have something to do with the reset button, but seeing as that there were no additional problems, I decided not to spend the time “fixing” it.  It was 0100 on a Saturday and I had not started packing for my trip to Japan.  I regret that decision now as I do not have enough sweatshirts and they do not have heat in these buildings.  


The video showing A Functionality can be seen [here](https://www.youtube.com/watch?v=v359o0dWESg&feature=youtu.be).  This was submitted before the due date via email.  

The text files for A functionality are shown below: 

[A Main](https://raw.githubusercontent.com/JohnTerragnoli/ECE382_Lab06/master/1.%20Code/A%20Functionality/A_functionality.c)
[A Header](https://raw.githubusercontent.com/JohnTerragnoli/ECE382_Lab06/master/1.%20Code/A%20Functionality/A_Functionality.h)
[A Implementation](https://raw.githubusercontent.com/JohnTerragnoli/ECE382_Lab06/master/1.%20Code/A%20Functionality/A_Function_Imp.c)

#Analysis/Conclusion/Lessons Learned: 

As a result of this process, I created a working robot which will serve as the backbone for future projects.  The process was difficult, but much was learned.  Motor stall current was finally understood, and how this tells use maximum PWM available to use in design.  Additionally, I learned not to wire up an LED in series by itself because it will get REALLY hot.  I learned ways to get around the fact that we cannot output a negative voltage.  I also learned to place capacitors across voltages so smooth the voltage for certain outputs/inputs.  As a result of doing so, the machine instantly became more reliable. Many other lessons learned are listed in the design process.  

#Grading: 
I got a 100% on all of the functionality and on the prelab.  I have to give you the grading sheet later when I come back from Japan.  



#Documentation:
##Prelab
Only used the datasheets and the picture of the hardware hook up that was give to us by our teachers.  

##Required Functionality

##A Functionality
