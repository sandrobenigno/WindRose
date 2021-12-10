#ifndef Being_h
#define Being_h

#include "Sensing.h"

class Being
{
  private:

  public:
    boolean alive = 1; //Starts Alive (0 dead or 1 alive)
    unsigned long lifetime = 0; //It just born (how long it's up)
    SensingData sensors; //Sensing Data Struct
    long ASBM = 0x00; //Active Sensor Byte Maps starts unset (64 bit total)

    void activate(uint8_t idx){ //Activate a sensor
        ASBM |= 0x01<<idx; //Set sense bit at idx
    }

    void deactivate(uint8_t idx){ //Deactivate a sensor
        ASBM &= ~(0x01<<idx); //Unset sense bit at idx
    }

    boolean isactive(uint8_t bit_pos){ //Check if a sensor is active
        //Check bit on ASBM at bit_pos
        if((ASBM>>bit_pos) & 0x01){
            return true;//It's up
        }
        else return false;//It's down
    }
};

#endif