import processing.serial.*;
import controlP5.*;

ControlP5 cp5;
DropdownList serialPorts;

Serial _stmRef;  // Create object from Serial class
final int BAUDRATE = 9600;

//Drraw Control Variables
boolean ready = false;
int panelX = 10;
int panelY = 15;

PImage bg; //Image for background

SensingData_t sensdata;
String[] portNames;

/* ================================================== */

void setup() 
{
  size(1280, 800);
  frameRate(30);
  portNames = Serial.list();
  
  cp5 = new ControlP5(this);
  serialPorts = cp5.addDropdownList("Serial Port...").setPosition(1080,10);
  for(int i = 0 ; i < portNames.length; i++) serialPorts.addItem(portNames[i], i);
  customize(serialPorts);

  bg = loadImage("bg.png"); //Getting the bg image file
  sensdata = new SensingData_t();
  println("Program started!");
}

void draw()
{
  if(ready){
    if(millis() - lastget > 100){ //Loop of data grabbing (10Hz)
      getData(); //Call the state machine
      lastget = millis();
    }
    if(pack_ok){
        drawBG(bg);
        drawGrid();
        drawLPPS();
      if(millis() - lastlost > 15000){ //Loop of Lost Packs Calculation (4 p/ min)
        lostrate = lostpack * 4; //Four times the loss of 15sec = loss per minute
        lostpack = 0;
        lastlost = millis();
      }
      drawLifetime();
      drawSensors();
      //println("ASBM: "+binary(int(ASBM))+" Light.lux="+sensdata.light.lux);
    }
  }
  else{
    drawBG(bg);
    drawGrid();
  }
}
