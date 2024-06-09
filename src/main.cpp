/** 
 * @file main.cpp
 * @brief This file implements the setup and loop of a WindRose board as a cellular automaton.
 * @details The main.cpp file imports the headers of the two main classes of the project. \n
 * The class WRBoard is the body of the WindRose cell ,as it controls the multiplexer, the LEDs and runs the Serial State Machine. \n
 * The class Being represents the cell's being. It's the soul of each WindRose cell and drives the sensing data.
 * @author Sandro Benigno, thanks to fellow professor Marilia Lyra Bergamo for the inspirartion and all the applied resources. \n
 * She is the one who made this project possible and inspired us by her research on Robotic Plants and Complexity Theories.
 * @version 2.0
 * @date 2024-06-09
 */

#include <Arduino.h> //Arduino header
#include "Config.h" //Config header
#include "WindRose.h" //The body: The WindRose Board classes (State Machine, Mux, LED etc)
#include "Being.h" //The soul: The Being class for holding and driving the sensing data
#include "Circadian.h" //Circadian class for internal loopings at specified periods

/** @brief WRBoard instance for driving the Multiplexer, the LED and the Serial State Machine (the body). */
WRBoard wr; //The WindRose board object
/** @brief Being instance for holding and driving the sensing data (the soul). */
Being mySelf; //The current cell
/** @brief The array of Being instances for holding and driving the sensing data of the neighbours. */
Being ngHood[4]; //The four neighbours 

/** @brief The circadian cycle for reading the serial buffer through the Serial State Machine. */
Circadian cycle_buffering(300); //The time the automata has been buffering

void setup() {

  wr.init(WRBAUDRATE); //Initialize the board setting the baud rate
  wr.led.off(); //Turn off the LED
  wr.mux.setDir(N); //Set the mux to the north edge
  wr.mux.enable(); //Enable the WindRose Mux
  delay(100); //Wait for the serial buffer to get some data

  mySelf.born(); //Set the WindRose being to be alive (the breath of life)

  //Sensor initialization
  pinMode(A0, INPUT); //Set the A0 pin as input
  //Activate Sensors means to set which sensors would be broadcasted
  mySelf.activate(light); //Activate the light sensor
  mySelf.activate(wind); //Activate the wind sensor
  mySelf.activate(coord); //Activate the coordinate sensor
}

void loop() {
  
  // Sensor Reading Block //////////////////////////////////////////////////////
  /** @brief The sensor reading block for reading the sensors and store it in the Being instance. */

  mySelf.sensors.light.lux = analogRead(A0); //Read the light sensor
  //mySelf.sensors.light.lux = ngHood[W].sensors.light.lux; //Borrowing the light sensor from the west neighbour
  mySelf.sensors.light.temp = 3200; //Set the color temperature to 3200 Kelvin

  mySelf.sensors.coord.x = millis(); //Set the x coordinate to the current time
  mySelf.sensors.coord.y = millis()/2; //Set the y coordinate to the current time

  mySelf.sensors.wind.speed = int(sin(float(millis() / 10000.0)) * 1000); //Set the x coordinate to the current time
  mySelf.sensors.wind.dir = millis()/4; //Set the y coordinate to the current time

  mySelf.updateAge(); //Update myself Age 
  
  // Comunication Block //////////////////////////////////////////////////////
  /** @brief The communication block for 1) receiving the neighbours data and 2) sending the myself data to the neighbours*/

  if(cycle_buffering.isElapsed()) // Buffer the data following a circadian cycle before processing the collected data
  {
    wr.ssm.getData(&ngHood[wr.mux.getDir()]);//Get sensing data from the neighbour
    mySelf.updateNGS(ngHood); //Update the Neighbouthood Mapping  
    mySelf.calcDNGS(ngHood); //Inferring diagonal neighbours

    //Changing the direction of the mux to the next neighbour
    wr.mux.disable(); //Disable the WindRose Mux
    wr.ssm.clearBuffer(); //Clear the buffer
    wr.mux.next(); //Move to the next edge
    wr.mux.enable(); //Enable the WindRose Mux

    wr.led.toggle(); //Toggle the LED
  }
  else if(mySelf.alive) { // Keep transmitting data until the data buffering cycle is enlapsed
    wr.ssm.sendData(&mySelf); //sending data while buffering
  }
}