/** 
 * @file WindRose.h
 * @brief This file contains the definition of the WindRose class.
 * @author Sandro Benigno
 * @version 1.0
 * @date 2020-01-01
 */

#ifndef WindRose_h
#define WindRose_h

#include <Arduino.h>
#include "Directions.h"
#include "SSMachine.h" //Machine State for reading neighbours data
#include "WRMux.h" //WindRose Multiplexer class

/** @brief defines the LED pin */
#define WRLED A2 //redefine LED_BUILTIN to be the WR LED pin

/** @brief The class to control the LED */
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

/**
 * @brief The class to control the WindRose Board 
 * @details The WindRose class holds the functions related directly to the board (the body).
 * It controls the Multiplexer, the LEDs and runs the Serial State Machine.
*/
class WRBoard
{
    private:
    public:
        SSMachine ssm; //Serial State Machine
        WRMux mux; //Multiplexer controller
        LED led; //LED controller
        
        /** @brief Initializes the board by starting the LED, the Mux, the Serial Hardware, and the State Machine. */
        void init(uint16_t _baudrate){
            pinMode(WRLED, OUTPUT); //Set the LED pin as output
            this->led.on(); //Turn on the LED
            Serial.begin(_baudrate);
            this->ssm.setSerial(&Serial); //Set the serial port of the Serial State Machine
            this->mux.init(); //Set up the WindRose Mux
        }
};

#endif