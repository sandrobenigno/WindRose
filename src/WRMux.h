#ifndef WRMux_h
#define WRMux_h

#include "Arduino.h"
#include "WindRose.h"

class WRMux
{
    private:
        uint8_t MUX_A = 5;
        uint8_t MUX_B = 6;
        uint8_t MUX_C = 7;
        uint8_t INB = 8;
        uint8_t current = 0;
    public:

        void init(){
          pinMode(this->MUX_A, OUTPUT);
          pinMode(this->MUX_B, OUTPUT);
          pinMode(this->MUX_C, OUTPUT);
          pinMode(this->INB, OUTPUT);
        }

        void enable(){
            digitalWrite(this->INB, LOW);
        }

        void disable(){
            digitalWrite(this->INB, HIGH);
        }

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

        uint8_t getDir(){
            return this->current;
        }

        void next(){
            this->current++;
            if(this->current > 3){
                this->current = 0;
            }
            this->setDir(this->current);
        }
};

#endif