#ifndef SSMachine_h
#define SSMachine_h

#include "Config.h" //Config header
#include "Sensing.h"
#include "Being.h"
#include "WRMux.h"

class SSMachine
{
  private:
    Stream *_stmRef; // Stream reference object
    WRMux _mux; //Multiplexer controller
    uint8_t ck_a=0; //Checksum accumulator A
    uint8_t ck_b=0; //Checksum accumulator B
  public:
    boolean trustpack = false; //Flag to indicate if the packet is trusted
    uint32_t lostpack = 0; //Number of lost packets

    void setSerial(Stream *stmObject)
    {
      this->_stmRef=stmObject; //Set the reference to the serial port
    }
    
    void printLostPack()
    {
      this->_stmRef->print("Lost packets: ");
      this->_stmRef->println(this->lostpack);
    }

    void clearBuffer()
    {
      while (this->_stmRef->available()) this->_stmRef->read(); //Clearing the Serial buffer
    }

    void sendData(Being *me)
    {
      byte DATA_buffer[MAX_SENSING_BYTES_PER_PACKET + 4]; // All sensors plus 4 extra bytes for ASBM
      uint8_t ck = 0; //Byte checksum accumulator
      byte CRC_ck_a = 0x06; //Checksum accumulator A (Starting by the seed A)
      byte CRC_ck_b = 0x02; //Checksum seed B (Starting by the seed B)

      this->_stmRef->write("WRoS"); //Write the header
      this->_stmRef->write(CRC_ck_a); //Write the seed A
      this->_stmRef->write(CRC_ck_b); //Write the seed B

      //Write the Being Lifetime (4 bytes)
      uint32_t lifetime = millis();
      DATA_buffer[ck] = byte(lifetime >> 24 & 0xFF);
      DATA_buffer[ck+1] = byte(lifetime >> 16 & 0xFF);
      DATA_buffer[ck+2] = byte(lifetime >> 8 & 0xFF);
      DATA_buffer[ck+3] = byte(lifetime & 0xFF);
      ck += 4;      

      //Write the ASBM (4 bytes)
      DATA_buffer[ck] = byte(me->ASBM >> 24 & 0xFF);
      DATA_buffer[ck+1] = byte(me->ASBM >> 16 & 0xFF);
      DATA_buffer[ck+2] = byte(me->ASBM >> 8 & 0xFF);
      DATA_buffer[ck+3] = byte(me->ASBM & 0xFF);
      ck += 4;

      //HERE STARTS THE DYNAMIC SENSOR REPORTING

      //Check the ASBM and write the data for Coordinates Sensor
      if(me->isactive(coord)){ //Coord_t
        DATA_buffer[ck] = byte(me->sensors.coord.x >> 8 & 0xFF);
        DATA_buffer[ck+1] = byte(me->sensors.coord.x & 0xFF);
        DATA_buffer[ck+2] = byte(me->sensors.coord.y >> 8 & 0xFF);
        DATA_buffer[ck+3] = byte(me->sensors.coord.y & 0xFF);
        DATA_buffer[ck+4] = byte(me->sensors.coord.z >> 8 & 0xFF);
        DATA_buffer[ck+5] = byte(me->sensors.coord.z & 0xFF);
        ck += 6;
      }

      //Check the ASBM and write the data for Angle Sensor
      if(me->isactive(angle)){ //Angle_t
        DATA_buffer[ck] = byte(me->sensors.angle.pitch >> 8 & 0xFF);
        DATA_buffer[ck+1] = byte(me->sensors.angle.pitch & 0xFF);
        DATA_buffer[ck+2] = byte(me->sensors.angle.roll >> 8 & 0xFF);
        DATA_buffer[ck+3] = byte(me->sensors.angle.roll & 0xFF);
        DATA_buffer[ck+4] = byte(me->sensors.angle.yaw >> 8 & 0xFF);
        DATA_buffer[ck+5] = byte(me->sensors.angle.yaw & 0xFF);
        ck += 6;
      }

      //Check the ASBM and write the data for Accelerometer Sensor
      if(me->isactive(accel)){ //Accel_t
        DATA_buffer[ck] = byte(me->sensors.accel.x >> 8 & 0xFF);
        DATA_buffer[ck+1] = byte(me->sensors.accel.x & 0xFF);
        DATA_buffer[ck+2] = byte(me->sensors.accel.y >> 8 & 0xFF);
        DATA_buffer[ck+3] = byte(me->sensors.accel.y & 0xFF);
        DATA_buffer[ck+4] = byte(me->sensors.accel.z >> 8 & 0xFF);
        DATA_buffer[ck+5] = byte(me->sensors.accel.z & 0xFF);
        ck += 6;
      }

      //Check the ASBM and write the data for Gyroscope Sensor
      if(me->isactive(ang_rate)){ //Ang_rate_t
        DATA_buffer[ck] = byte(me->sensors.ang_rate.x >> 8 & 0xFF);
        DATA_buffer[ck+1] = byte(me->sensors.ang_rate.x & 0xFF);
        DATA_buffer[ck+2] = byte(me->sensors.ang_rate.y >> 8 & 0xFF);
        DATA_buffer[ck+3] = byte(me->sensors.ang_rate.y & 0xFF);
        DATA_buffer[ck+4] = byte(me->sensors.ang_rate.z >> 8 & 0xFF);
        DATA_buffer[ck+5] = byte(me->sensors.ang_rate.z & 0xFF);
        ck += 6;
      }

      //Check the ASBM and write the data for GPS Sensor
      if(me->isactive(gps_pos)){ //GPS_pos_t
        DATA_buffer[ck] = byte(me->sensors.gps_pos.lat >> 24 & 0xFF);
        DATA_buffer[ck+1] = byte(me->sensors.gps_pos.lat >> 16 & 0xFF);
        DATA_buffer[ck+2] = byte(me->sensors.gps_pos.lat >> 8 & 0xFF);
        DATA_buffer[ck+3] = byte(me->sensors.gps_pos.lat & 0xFF);
        DATA_buffer[ck+4] = byte(me->sensors.gps_pos.lon >> 24 & 0xFF);
        DATA_buffer[ck+5] = byte(me->sensors.gps_pos.lon >> 16 & 0xFF);
        DATA_buffer[ck+6] = byte(me->sensors.gps_pos.lon >> 8 & 0xFF);
        DATA_buffer[ck+7] = byte(me->sensors.gps_pos.lon & 0xFF);
        DATA_buffer[ck+8] = byte(me->sensors.gps_pos.alt >> 24 & 0xFF);
        DATA_buffer[ck+9] = byte(me->sensors.gps_pos.alt >> 16 & 0xFF);
        DATA_buffer[ck+10] = byte(me->sensors.gps_pos.alt >> 8 & 0xFF);
        DATA_buffer[ck+11] = byte(me->sensors.gps_pos.alt & 0xFF);
        DATA_buffer[ck+12] = byte(me->sensors.gps_pos.gspeed >> 24 & 0xFF);
        DATA_buffer[ck+13] = byte(me->sensors.gps_pos.gspeed >> 16 & 0xFF);
        DATA_buffer[ck+14] = byte(me->sensors.gps_pos.gspeed >> 8 & 0xFF);
        DATA_buffer[ck+15] = byte(me->sensors.gps_pos.gspeed & 0xFF);
        ck += 16;
      }

      //Check the ASBM and write the data for Barometer Sensor
      if(me->isactive(wind)){ //Wind_t
        DATA_buffer[ck] = byte(me->sensors.wind.speed >> 8 & 0xFF);
        DATA_buffer[ck+1] = byte(me->sensors.wind.speed & 0xFF);
        DATA_buffer[ck+2] = byte(me->sensors.wind.dir >> 8 & 0xFF);
        DATA_buffer[ck+3] = byte(me->sensors.wind.dir & 0xFF);
        ck += 4;
      }

      //Check the ASBM and write the data for Lightmeter Sensor
      if(me->isactive(light)){ //Light_t
        DATA_buffer[ck] = byte(me->sensors.light.lux >> 24 & 0xFF);
        DATA_buffer[ck+1] = byte(me->sensors.light.lux >> 16 & 0xFF);
        DATA_buffer[ck+2] = byte(me->sensors.light.lux >> 8 & 0xFF);
        DATA_buffer[ck+3] = byte(me->sensors.light.lux & 0xFF);
        DATA_buffer[ck+4] = byte(me->sensors.light.temp >> 8 & 0xFF);
        DATA_buffer[ck+5] = byte(me->sensors.light.temp & 0xFF);
        ck += 6;
      }
      
      //HERE ENDS THE DYNAMIC SENSOR REPORTING


      for (int i=0;i<ck;i++) this->_stmRef->write(DATA_buffer[i]); //Write the data to the output stream
      for (int i=0;i<ck;i++) //
      {
        CRC_ck_a += DATA_buffer[i]; //Calculate the CRC checksum for the data
        CRC_ck_b += CRC_ck_a;
      }
      this->_stmRef->write(CRC_ck_a); //Write the CRC checksum to the output stream
      this->_stmRef->write(CRC_ck_b);
    }

    void checksum(byte dt)
    {
      this->ck_a += dt; //Calculate the CRC checksum for the data
      this->ck_b += this->ck_a; 
    }

    byte readsum(){
      byte _data = this->_stmRef->read(); //Read the data from the input stream
      this->checksum(_data); //Calculate the CRC checksum for the data
      return _data;
    }

    void getData(Being *nb)
    {
      byte data; //Variable to store the read data
      boolean keepgoing =true; //Variable to keep track of the data read
      uint8_t chances = 5; //Variable to keep track of the number of chances to read the data
      SensingData sensdata; //Sensor class object to store the data
      uint8_t DT_step=0; //Variable to step through the machine state
      uint8_t DT_ck_a, DT_ck_b = 0;  //Variables to store the external CRC checksum
      this->ck_a = 0; //Reset the internal CRC checksum accumulator A
      this->ck_b = 0; //Reset the internal CRC checksum accumulator B
#ifdef WRDEBUG
      _stmRef->println("Entered getData");
#endif
      nb->alive = false; //Reset the neighbour being alive flag
      this->trustpack = false; //Reset the trust packet flag

      uint8_t numc = this->_stmRef->available();
      if (numc > 0){
#ifdef WRDEBUG
      _stmRef->println("Ther is something to read");
#endif
        //for(int i=0;i<numc;i++)  //Process bytes received
        while(this->_stmRef->available() && keepgoing)
        {
#ifdef WRDEBUG
      _stmRef->println("Entered the read loop");
#endif
          data = this->_stmRef->read();
          switch(DT_step)  //We start from zero. Here starts the State Machine
          {
          case 0:  
            if(data==char('W')) // Data sync char 1 (0x57)
            { // DATA sync char 1
              DT_step++;   //Hmmm... first data packet is correct, lets jump to the next step
            }
            break; 
          case 1:  
            if(data==char('R'))  // DATA sync char 2 (0x52)
            {
              DT_step++;   //Oooh... second data packet is correct, lets jump to the step 2
            }
            else 
              DT_step=0;   //Nop, not this time... so restart to step zero and try again.     
            break;
          case 2:  
            if(data==char('o'))  // DATA sync char 3 (0x6F)
              DT_step++;   //Yayyy! third data packet is correct, lets jump to the step 3
            else 
              DT_step=0;   //Nop, so close, but not this time... so restart from step zero and try again.     
            break;
          case 3:  
            if(data==char('S')){  // DATA sync char 4 (0x53)
              DT_step++;   //YEESSSS!!! The fourth data packet is correct, jump to the step 4
#ifdef WRDEBUG
              _stmRef->println("Header OK");
#endif
            }
            else 
              DT_step=0;   //Nop, is not correct, so restart from step zero and try again.     
            break;
          case 4:
            this->ck_a = data; //Getting the first chk sum seed
            DT_step++;
            break;
          case 5:
            this->ck_b = data;//Getting the second chk sum seed
            DT_step++;
            break;

          //HERE IS (LIFETIME AND ASBM) GRABBING (MSB -> Most Significant Byte First Order)

          case 6:
            nb->lifetime = 0; //Reset the neighbour lifetime;
            this->checksum(data); //First checksum (data taken from the while loop)
            nb->lifetime |= uint32_t(data) << 24; //Get the MSB of the lifetime (Byte 4)
            data = this->readsum(); //Starting to use the function readsum (read+checksum)
            nb->lifetime |= uint32_t(data) << 16; //Get the MSB of the lifetime (Byte 3)
            data = this->readsum();
            nb->lifetime |= uint32_t(data) << 8; //Get the MSB of the lifetime (Byte 2)
            data = this->readsum();
            nb->lifetime |= uint32_t(data); //Get the MSB of the lifetime (Byte 1)

            nb->ASBM = 0x00;
            data = this->readsum();
            nb->ASBM |= uint32_t(data)<<24; //Getting the Active Sensor Map (Byte 4)
            data = this->readsum();
            nb->ASBM |= uint32_t(data)<<16;  //Getting the Active Sensor Map (Byte 3)
            data = this->readsum();
            nb->ASBM |= uint32_t(data)<<8;  //Getting the Active Sensor Map (Byte 2)
            data = this->readsum();
            nb->ASBM |= data;  //Getting the Active Sensor Map (Byte 1)
            
            //HERE STARTS THE DYNAMIC SENSOR GRABBING

            for(int i=0;i<32;i++){ //Looping through the 32 bits of the Active Sensor Map
              if(nb->ASBM & (1<<i)){ //Checking if the bit is set (active sensor)
                switch(i){ //Checking the sensor type
                  case 0: //Coord Sensor (Coord_t)
                    //Clearing the Coord_t struct
                    sensdata.coord.x = 0;
                    sensdata.coord.y = 0;
                    sensdata.coord.z = 0;
                    //Getting the data
                    data = this->readsum();
                    sensdata.coord.x |= int16_t(data) << 8;
                    data = this->readsum();
                    sensdata.coord.x |= data;
                    data = this->readsum();
                    sensdata.coord.y |= int16_t(data) << 8;
                    data = this->readsum();
                    sensdata.coord.y |= data;
                    data = this->readsum();
                    sensdata.coord.z |= int16_t(data) << 8;
                    data = this->readsum();
                    sensdata.coord.z |= data;
                    break;
                  case 1: //Angle Sensor (Angle_t)
                    //Clearing the Angle_t struct
                    sensdata.angle.pitch = 0;
                    sensdata.angle.roll = 0;
                    sensdata.angle.yaw = 0;
                    //Getting the data
                    data = this->readsum();
                    sensdata.angle.pitch |= int16_t(data) << 8;
                    data = this->readsum();
                    sensdata.angle.pitch |= data;
                    data = this->readsum();
                    sensdata.angle.roll |= int16_t(data) << 8;
                    data = this->readsum();
                    sensdata.angle.roll |= data;
                    data = this->readsum();
                    sensdata.angle.yaw |= int16_t(data) << 8;
                    data = this->readsum();
                    sensdata.angle.yaw |= data;
                    break;
                  case 2: //Accelerometer Sensor (Accel_t)
                    //Clearing the Accel_t struct
                    sensdata.accel.x = 0;
                    sensdata.accel.y = 0;
                    sensdata.accel.z = 0;
                    //Getting the data
                    data = this->readsum();
                    sensdata.accel.x |= int16_t(data) << 8;
                    data = this->readsum();
                    sensdata.accel.x |= data;
                    data = this->readsum();
                    sensdata.accel.y |= int16_t(data) << 8;
                    data = this->readsum();
                    sensdata.accel.y |= data;
                    data = this->readsum();
                    sensdata.accel.z |= int16_t(data) << 8;
                    data = this->readsum();
                    sensdata.accel.z |= data;
                    break;
                  case 3: //Ang_rate Sensor (Ang_rate_t)
                    //Clearing the Ang_rate_t struct
                    sensdata.ang_rate.x = 0;
                    sensdata.ang_rate.y = 0;
                    sensdata.ang_rate.z = 0;
                    //Getting the data
                    data = this->readsum();
                    sensdata.ang_rate.x |= int16_t(data) << 8;
                    data = this->readsum();
                    sensdata.ang_rate.x |= data;
                    data = this->readsum();
                    sensdata.ang_rate.y |= int16_t(data) << 8;
                    data = this->readsum();
                    sensdata.ang_rate.y |= data;
                    data = this->readsum();
                    sensdata.ang_rate.z |= int16_t(data) << 8;
                    data = this->readsum();
                    sensdata.ang_rate.z |= data;
                    break;
                  case 4: //Gps_pos Sensor (Gps_pos_t)
                    //Clearing the Gps_pos_t struct
                    sensdata.gps_pos.lat = 0;
                    sensdata.gps_pos.lon = 0;
                    sensdata.gps_pos.alt = 0;
                    sensdata.gps_pos.gspeed = 0;
                    //Getting the data
                    data = this->readsum();
                    sensdata.gps_pos.lat |= int32_t(data) << 24;
                    data = this->readsum();
                    sensdata.gps_pos.lat |= int32_t(data) << 16;
                    data = this->readsum();
                    sensdata.gps_pos.lat |= int32_t(data) << 8;
                    data = this->readsum();
                    sensdata.gps_pos.lat |= data;
                    data = this->readsum();
                    sensdata.gps_pos.lon |= int32_t(data) << 24;
                    data = this->readsum();
                    sensdata.gps_pos.lon |= int32_t(data) << 16;
                    data = this->readsum();
                    sensdata.gps_pos.lon |= int32_t(data) << 8;
                    data = this->readsum();
                    sensdata.gps_pos.lon |= data;
                    data = this->readsum();
                    sensdata.gps_pos.alt |= int32_t(data) << 24;
                    data = this->readsum();
                    sensdata.gps_pos.alt |= int32_t(data) << 16;
                    data = this->readsum();
                    sensdata.gps_pos.alt |= int32_t(data) << 8;
                    data = this->readsum();
                    sensdata.gps_pos.alt |= data;
                    data = this->readsum();
                    sensdata.gps_pos.gspeed |= int32_t(data) << 24;
                    data = this->readsum();
                    sensdata.gps_pos.gspeed |= int32_t(data) << 16;
                    data = this->readsum();
                    sensdata.gps_pos.gspeed |= int32_t(data) << 8;
                    data = this->readsum();
                    sensdata.gps_pos.gspeed |= data;
                    break;
                  case 5: //Wind Sensor (Wind_t)
                    //Clearing the Wind_t struct
                    sensdata.wind.speed = 0;
                    sensdata.wind.dir = 0;
                    //Getting the data
                    data = this->readsum();
                    sensdata.wind.speed |= int16_t(data) << 8;
                    data = this->readsum();
                    sensdata.wind.speed |= data;
                    data = this->readsum();
                    sensdata.wind.dir |= int16_t(data) << 8;
                    data = this->readsum();
                    sensdata.wind.dir |= data;
                    break;
                  case 6: //Light Sensor (Light_t)
                    //Clearing the Light_t struct
                    sensdata.light.lux = 0;
                    sensdata.light.temp = 0;
                    //Getting the data
                    data = this->readsum();
                    sensdata.light.lux |= int32_t(data) << 24;
                    data = this->readsum();
                    sensdata.light.lux |= int32_t(data) << 16;
                    data = this->readsum();
                    sensdata.light.lux |= int32_t(data) << 8;
                    data = this->readsum();
                    sensdata.light.lux |= data;
                    data = this->readsum();
                    sensdata.light.temp |= int16_t(data) << 8;
                    data = this->readsum();
                    sensdata.light.temp |= data;
                    break;
                  default:
                    break;
                }
              }
            }
            //HERE ENDS THE DYNAMIC SENSOR GRABBING

            DT_step++;
            break;

          //Checksum Block
          case 7:
            DT_ck_a = data; //Getting byte checksum A
            DT_step++;
            break;
          case 8:
            DT_ck_b = data; //Getting byte checksum B
            if((this->ck_a==DT_ck_a) && (this->ck_b==DT_ck_b)){ //Checksum is correct
              nb->sensors = sensdata; //Sending the data to the neigbour repository
              nb->alive = true; //Sets the neighbour as alive
              this->trustpack = true; //Trust the package
              keepgoing = false; //Stops the loop
            }
            else{ //Checksum is not correct
              this->lostpack++; //Increment the lost package counter
              DT_step=0; //Reset the step counter to start over
              this->ck_a = 0; //Reset the checksum A
              this->ck_b = 0; //Reset the checksum B
              if(chances-- == 0) keepgoing = false; //Stops the loop if the chances are over
            }
            break; //End of the checksum block
          }
        }
        this->_mux.disable(); //Inhibits the multiplexing
        this->clearBuffer(); //Clears the serial buffer
      }
    }
};

#endif