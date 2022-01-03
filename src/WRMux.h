/**
 * @file WRMux.h
 * @brief This file defines the WindRose's Multiplexer Control class WRMux.
 * @author Sandro Benigno
 * @version 1.0
 * @date 2022-01-01
 */

#ifndef WRMux_h
#define WRMux_h

#include "Arduino.h"
#include "WindRose.h"

/**
 * @brief The WRMux class The class to operate the WindRose Multiplexer Control.
 */
class WRMux
{
    private:
        /** @brief MUX_A defines the pin for the MUX_A selector */
        uint8_t MUX_A = 5;
        /** @brief MUX_B defines the pin for the MUX_B selector */
        uint8_t MUX_B = 6;
        /** @brief MUX_C defines the pin for the MUX_C selector */
        uint8_t MUX_C = 7;
        /** @brief INB defines the pin for the Inhibit pin */
        uint8_t INB = 8;
        /** @brief current_mux defines the current mux direction */
        uint8_t current = 0;
    public:

        /**
         * @brief Initializes the WRMux class
         * by seting the control pins as OUTPUT.
         */
        void init(){
            pinMode(this->MUX_A, OUTPUT);
            pinMode(this->MUX_B, OUTPUT);
            pinMode(this->MUX_C, OUTPUT);
            pinMode(this->INB, OUTPUT);
        }

        /** @brief Unsets the inhibit pin and enable the mux. */
        void enable(){
            digitalWrite(this->INB, LOW);
        }
        
        /** @brief Sets the inhibit pin and disable the mux. */
        void disable(){
            digitalWrite(this->INB, HIGH);
        }

        /** 
         * @brief Sets the mux to the direction specified.
         * @param dir The direction to set the mux (N,S,E,W).
         */
        void setDir(uint8_t dir){
            this->current = dir;
            uint8_t code_mux = 0b111 - dir; // 0b111 - dir = (0bCBA)
            //0b111 - N -> 0b111 - 0b000 = 0b111
            //0b111 - E -> 0b111 - 0b001 = 0b110
            //0b111 - S -> 0b111 - 0b010 = 0b101
            //0b111 - W -> 0b111 - 0b011 = 0b100
            digitalWrite(this->MUX_A, code_mux & 1); // set A
            digitalWrite(this->MUX_B, (code_mux>>1) & 1); // set B
            digitalWrite(this->MUX_C, (code_mux>>2) & 1); // set C
        }

        /** 
         * @brief Returns the current direction of the mux.
         * @return The current direction of the mux as N,S,E,W.
         */
        uint8_t getDir(){
            return this->current;
        }

        /** @brief Returns the current direction of the mux as a string. */
        char getdirchar(){
            char dirs[4] = {'N', 'E', 'S', 'W'};
            return dirs[this->getDir()];
        }

        /** 
         * @brief Sets the mux from the current to the next direction as N,S,E,W.
         */
        void next(){
            this->current++;
            if(this->current > 3){
                this->current = 0;
            }
            this->setDir(this->current);
        }
};

#endif