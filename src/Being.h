/** @file Being.h
 * @brief This file defines the WindRose's Being class.
 * @author Sandro Benigno
 * @version 1.0
 * @date 2021-12-31
 */

#ifndef Being_h
#define Being_h

#include "Directions.h"
#include "Sensing.h"

/**
 * @brief The base class (the soul) for all the beings in the simulation.
 * @details The Being class contains the basic attributes of a being, such as its age, its state, \n
 * its Active Sensors Bit Mapping (ASBM), \n
 * the Neighborhood State Mapping (NGS). \n
 * It used for storing automata data and managing sensors.
 */

class Being
{
  private:

  public:
    /** @brief The state of the being (dead = 0, alive = 1) */
    boolean alive = 0; //It doesn't born yet
    /** @brief The age of the being, since the board is up. */
    uint32_t age = 0; //It just borns (0)
    /** @brief Sensor dada holder 
     * It has all the sensor types defined in the Sensing.h file. \n
     * Think of it as a holder for data captured from sensors, organized by sensor types.
    */
    SensingData sensors; //Sensing Data Struct
    /** @brief The neighborhood state mapping (NGS) */
    byte NGS =0x00; //Neigborhood Status Map
    /** @brief The active sensor mapping (ASBM) */
    long ASBM = 0x00; //Active Sensor Byte Maps starts unset (32 bit total)

    /**
     * @brief Sets the cell's state to alive.
     */
    void born(){
        this->alive = 1;
    }

    /**
     * @brief Sets the cell's state to dead.
     */
    void dye(){
        this->alive = 0;
    }

    /** @brief Activates a sensor in the 32bit Active Sensor Bit Map (ASBM) 
     * @param idx The id of the sensor to be activated
    */
    void activate(uint8_t idx){ //Activate a sensor
        this->ASBM |= 0x01<<idx; //Set sense bit at idx (maches with the bit in the ASBM)
    }
    /** @brief Deactivates a sensor in the 32bit Active Sensor Bit Map (ASBM)
     *  @param idx The id of the sensor to be deactivated
    */
    void deactivate(uint8_t idx){ //Deactivate a sensor
        this->ASBM &= ~(0x01<<idx); //Unset sense bit at idx (matches with the bit in the ASBM)
    }
    /** @brief Checks if a sensor is active in the 32bit Active Sensor Bit Map (ASBM)
     * @param bit_pos The bit position(same as the id) of the sensor to be checked in the ASBM
    */
    boolean isactive(uint8_t bit_pos){ //Check if a sensor is active
        //Check bit on ASBM at bit_pos
        if((this->ASBM>>bit_pos) & 0x01){
            return true;//It's up
        }
        else return false;//It's down
    }
    /** @brief Updates the Age of the being by seting it to the current time */
    void updateAge(){ //Update age (uptime)
        this->age = millis();
    }

    /** @brief Updates the 8bit Neighborhood State Map (NGS)
     * by setting it's four less significant bits to dead(0) or alive(1)
     * @param *_ng The being to be checked against (is a pointer to the being to be checked)
    */
    void updateNGS(Being * _ng){ //Update Neighborhood Status Map
        this->NGS = 0x00; //Reset ngstatus
        for(uint8_t i=0;i<4;i++){
            if(_ng[i].alive){
                this->NGS |= 0x01<<i; //Set sense bit at idx
            }
        }
    }

    /** @brief Calculates the Diagonal neighbors of the being
     * and sets the four most significant bits of the 8bit Neighborhood State Map (NGS)
     * @param *_ng The being to be checked against (is a pointer to the being to be checked)
    */
    void calcDNGS(Being * _ng){ //Infer Neighborhood Status Map
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