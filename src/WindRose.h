#ifndef WindRose_h
#define WindRose_h

#include <Arduino.h>
#include "Directions.h"
#include "SSMachine.h" //Machine State for reading neighbours data
#include "WRMux.h" //WindRose Multiplexer class

#define WRLED A2 //redefine LED_BUILTIN to be the WR LED pin

class LED {
  public:
    void on(){
        digitalWrite(WRLED, HIGH);
    }
    void off(){
        digitalWrite(WRLED, LOW);
    }
    void toggle(){
        digitalWrite(WRLED, !digitalRead(WRLED));
    }
};

class WRBoard
{
    private:
    public:
        SSMachine ssm; //Serial State Machine
        WRMux mux; //Multiplexer controller
        LED led; //LED controller
        void init(uint16_t _baudrate){
            pinMode(WRLED, OUTPUT); //Set the LED pin as output
            this->led.on(); //Turn on the LED
            Serial.begin(_baudrate);
            this->ssm.setSerial(&Serial); //Set the serial port of the Serial State Machine
            this->mux.init(); //Set up the WindRose Mux
        }

        char getdirchar(){
            char dirs[4] = {'N', 'E', 'S', 'W'};
            return dirs[this->mux.getDir()];
        }

};

#endif