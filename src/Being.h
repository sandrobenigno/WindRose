#ifndef Being_h
#define Being_h

#include "Sensing.h"

class Being
{
  private:

  public:
    boolean alive = 1; //Starts Alive (0 dead or 1 alive)
    uint32_t lifetime = 0; //It just born (how long it's up)
    SensingData sensors; //Sensing Data Struct
    byte NGS =0x00; //Neigborhood Status Map
    long ASBM = 0x00; //Active Sensor Byte Maps starts unset (32 bit total)

    void activate(uint8_t idx){ //Activate a sensor
        this->ASBM |= 0x01<<idx; //Set sense bit at idx
    }

    void deactivate(uint8_t idx){ //Deactivate a sensor
        this->ASBM &= ~(0x01<<idx); //Unset sense bit at idx
    }

    void updateLifetime(){ //Update lifetime
        this->lifetime = millis();
    }

    boolean isactive(uint8_t bit_pos){ //Check if a sensor is active
        //Check bit on ASBM at bit_pos
        if((this->ASBM>>bit_pos) & 0x01){
            return true;//It's up
        }
        else return false;//It's down
    }

    void updateNGS(Being _ng[4]){ //Update Neighborhood Status Map
        this->NGS = 0x00; //Reset ngstatus
        for(uint8_t i=0;i<4;i++){
            if(_ng[i].alive){
                this->NGS |= 0x01<<i; //Set sense bit at idx
            }
        }
    }
};

#endif