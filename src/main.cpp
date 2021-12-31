#include <Arduino.h> //Arduino header
#include "Config.h" //Config header
#include "WindRose.h" //WindRose Board class (State Machine, Mux, LED etc)
#include "Being.h" //Being class for storing automata data and managing sensors

WRBoard wr; //The WindRose board object
Being mySelf; //The current cell
Being ngHood[4]; //The four neighbours

unsigned long timebuffering = 0; //The time the automata has been buffering

void setup() {

  wr.init(WRBAUDRATE); //Initialize the board setting the baud rate
  wr.mux.setDir(N); //Set the mux to the north edge
  wr.mux.enable(); //Enable the WindRose Mux
  delay(100); //Wait for the serial buffer to get some data

  //Sensor initialization
  pinMode(A0, INPUT); //Set the A0 pin as input
  //Activate Sensors means to set which sensors would be broadcasted
  mySelf.activate(light); //Activate the light sensor
  mySelf.activate(wind); //Activate the wind sensor
  mySelf.activate(coord); //Activate the coordinate sensor
}

void loop() {
  
  // Sensor Reading Block //////////////////////////////////////////////////////

  mySelf.sensors.light.lux = analogRead(A0); //Read the light sensor
  //mySelf.sensors.light.lux = ngHood[W].sensors.light.lux; //Read the light sensor from the west neighbour
  mySelf.sensors.light.temp = 3200; //Set the color temperature to 3200 Kelvin

  mySelf.sensors.coord.x = millis(); //Set the x coordinate to the current time
  mySelf.sensors.coord.y = millis()/2; //Set the y coordinate to the current time

  mySelf.sensors.wind.speed = int(sin(float(millis() / 10000.0)) * 1000); //Set the x coordinate to the current time
  mySelf.sensors.wind.dir = millis()/4; //Set the y coordinate to the current time

  mySelf.updateAge(); //Update myself Age
  
  // Comunication Block ////////////////////////////////////////////////////////

  wr.ssm.getData(&ngHood[wr.mux.getDir()]);//Keep trying to be sure
  mySelf.updateNGS(ngHood); //Update the Neighbouthood Mapping  
  mySelf.calcDNGS(ngHood); //Inferring diagonal neighbours

  //Changing the direction of the mux to the next neighbour
  wr.mux.disable(); //Disable the WindRose Mux
  wr.ssm.clearBuffer(); //Clear the buffer
  wr.mux.next(); //Move to the next edge
  wr.mux.enable(); //Enable the WindRose Mux
  timebuffering = millis(); //Reset the timer
  while (millis() - timebuffering < 300) //Wait for 200ms to buffer the data
  {
    wr.ssm.sendData(&mySelf); //sending data while buffering
  }
  wr.led.toggle(); //Toggle the LED
}