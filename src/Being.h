#ifndef Being_h
#define Being_h

#include "Directions.h"
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

    void calcDNGS(Being _ng[4]){ //Infer Neighborhood Status Map
        uint8_t DNGS = 0x00; //Inferred Neighborhood Status Map
        for(uint8_t i=0;i<4;i++){
            switch (i){
                case N:
                    if(_ng[N].alive){
                        DNGS |= (_ng[N].NGS >> W & 0x01) << NW;  //Inferring NW
                        DNGS |= (_ng[N].NGS >> E & 0x01) << NE;  //Inferring NE
                    }
                    break;
                case E:
                    if(_ng[E].alive){
                        DNGS |= (_ng[E].NGS >> N & 0x01) << NE;  //Inferring NE
                        DNGS |= (_ng[E].NGS >> S & 0x01) << SE;  //Inferring SE
                    }
                    break;
                case S:
                    if(_ng[S].alive){
                        DNGS |= (_ng[S].NGS >> E & 0x01) << SE;  //Inferring SE
                        DNGS |= (_ng[S].NGS >> W & 0x01) << SW;  //Inferring SW
                    }
                    break;
                case W:
                    if(_ng[W].alive){
                        DNGS |= (_ng[W].NGS >> S & 0x01) << SW;  //Inferring SW
                        DNGS |= (_ng[W].NGS >> N & 0x01) << NW;  //Inferring NW
                    }
                    break;
                default:
                    break;
            }
        }
        this->NGS |= DNGS; //Update NGS with inferred diagonal neighbors
    }
};

#endif