#include <Arduino.h> //Arduino header
#include "WindRose.h" //WindRose Board class (State Machine, Mux, LED etc)
#include "Being.h" //Being class for storing automata data and managing sensors

WRBoard wr; //The WindRose board object
Being mySelf; //The current cell
Being ngHood[4]; //The four neighbours

unsigned long lastcom = 0; //Timer for getting and sending data

void setup() {

  wr.init(9600); //Initialize the board setting the baud rate
  wr.mux.set(N); //Set the mux to the north edge
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
  mySelf.sensors.light.temp = 3200; //Set the color temperature to 3200 Kelvin

  mySelf.sensors.coord.x = millis(); //Set the x coordinate to the current time
  mySelf.sensors.coord.y = millis()/2; //Set the y coordinate to the current time

  mySelf.sensors.wind.speed = int(sin(float(millis() / 10000.0)) * 1000); //Set the x coordinate to the current time
  mySelf.sensors.wind.dir = millis()/4; //Set the y coordinate to the current time


  // Comunication Block ////////////////////////////////////////////////////////

  if(millis() - lastcom > 100) //10Hz block for Comuncate with the neighbours
  {
    wr.ssm.getData(&ngHood[wr.mux.getdir()]);//getting data from a neighbour
    
    /*
    ////// DEBUG CODE --> REMOVE LATER
    Serial.print(wr.getdirchar());
    if(ngHood[wr.mux.getdir()].alive){
      Serial.println(" -> Alive!");
    } else {
      Serial.println(" -> Dead!");
    }

    if(ngHood[wr.mux.getdir()].alive){ //if the neighbour is alive
      wr.ssm.sendData(&ngHood[wr.mux.getdir()]); //sending data of a neighbour for testing
    }
    ////// <-- DEBUG CODE
    */
    
    wr.ssm.sendData(&mySelf); //sending data of the current cell
    wr.mux.next(); //Move to the next edge
    wr.ssm.clearBuffer(); //Clear the buffer
    wr.mux.enable(); //Enable the WindRose Mux
    delay(50); //Wait for the serial buffer to get some data
    wr.led.toggle(); //Toggle the LED
    lastcom = millis(); //Reset the timer
  }
}