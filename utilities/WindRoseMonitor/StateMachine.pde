//State Machine Control Variables
byte ck_a = 0;
byte ck_b = 0; //Local CHK
byte seed_a = 0;
byte seed_b = 0; //CHK seeds from Serial data
int lifetime = 0;
long ASBM = 0;
long lastget = 0;
long lastlost = 0;
int lostpack = 0;
int lostrate = 0;
boolean pack_ok = false;

void checksum(int dt){
  ck_a += byte(dt);
  ck_b += ck_a; 
}

int readsum(){
  int _data = _stmRef.read();
  checksum(_data);
  return _data;
}

void getData()
{
  ck_a = 0;
  ck_b = 0;
  int tries = 4;
  boolean keepgoing=true;
  pack_ok = false;
  int data;
  int DT_step=0;
  byte DT_ck_a = 0;
  byte DT_ck_b = 0; //CHK result bytes from serial data

  int numc = _stmRef.available();
  if (numc >= 0){
    //for(int i=0;i<numc;i++){
    while(_stmRef.available() > 0 && keepgoing){
      //println(numc);
      data = _stmRef.read();
      switch(DT_step)  //We start from zero. Here starts the State Machine
      {
        case 0:  
          if(data==char('W'))
          { // DATA sync char 1
            DT_step++;   //Hmmm... first data packet is correct, lets jump to the next step
          }
          break; 
        case 1:  
          if(data==char('R'))  // DATA sync char 2
          {
            DT_step++;   //Oooh... second data packet is correct, lets jump to the step 2
          }
          else 
            DT_step=0;   //Nop, not this time... so restart to step zero and try again.     
          break;
        case 2:  
          if(data==char('o')){  // DATA sync char 3
            DT_step++;   //Yayyy! third data packet is correct, lets jump to the step 3
          }
          else{ 
            DT_step=0;   //Nop, so close, but not this time... so restart from step zero and try again.
          }
          break;
        case 3:  
          if(data==char('S')){  // DATA sync char 4
            DT_step++;   //YEESSSS!!! The fourth data packet is correct, jump to the step 4
          }
          else{
            DT_step=0;   //Nop, is not correct, so restart from step zero and try again.
          }
          break;
        case 4:
          ck_a = byte(data);//Getting the first chk sum seed
          DT_step++;
          break;
        case 5:
          ck_b = byte(data);//Getting the second chk sum seed
          DT_step++;
          break;
        case 6:
          lifetime = 0;
          checksum(data);
          lifetime |= (data)<<24; //Getting the Lifetime (Byte 4)
          data = readsum();
          lifetime |= (data)<<16; //Getting the Lifetime (Byte 3)
          data = readsum();
          lifetime |= (data)<<8; //Getting the Lifetime (Byte 2)
          data = readsum();
          lifetime |= (data); //Getting the Lifetime (Byte 1)
        
          ASBM = 0x00;
          data = readsum();
          ASBM |= (data)<<24; //Getting the Active Sensor Map (Byte 4)
          data = readsum();
          ASBM |= (data)<<16;  //Getting the Active Sensor Map (Byte 3)
          data = readsum();
          ASBM |= (data)<<8;  //Getting the Active Sensor Map (Byte 2)
          data = readsum();
          ASBM |= data;  //Getting the Active Sensor Map (Byte 1)

          //HERE STARTS THE DYNAMIC SENSOR GRABBING
          for(int j=0;j<32;j++){ //Looping through the 32 bits of the Active Sensor Map
            if((ASBM>>j & 0x01) == 1){ //Checking if the bit is set (active sensor)
              switch(j){ //Checking the sensor type
                case 0: //Coordinates Sensor (Coord_t)
                  //Clearing the Coord_t structure
                  sensdata.coord.x = 0;
                  sensdata.coord.y = 0;
                  sensdata.coord.z = 0;
                  //Getting the data
                  data = readsum();
                  sensdata.coord.x |= (data) << 8;
                  data = readsum();
                  sensdata.coord.x |= data;
                  data = readsum();
                  sensdata.coord.y |= (data) << 8;
                  data = readsum();
                  sensdata.coord.y |= data;
                  data = readsum();
                  sensdata.coord.z |= (data) << 8;
                  data = readsum();
                  sensdata.coord.z |= data;
                  break;
                case 1: //Angular Sensor (Angle_t)
                  //Clearing the Angle_t structure
                  sensdata.angle.pitch = 0;
                  sensdata.angle.roll = 0;
                  sensdata.angle.yaw = 0;
                  //Getting the data
                  data = readsum();
                  sensdata.angle.pitch |= (data) << 8;
                  data = readsum();
                  sensdata.angle.pitch |= data;
                  data = readsum();
                  sensdata.angle.roll |= (data) << 8;
                  data = readsum();
                  sensdata.angle.roll |= data;
                  data = readsum();
                  sensdata.angle.yaw |= (data) << 8;
                  data = readsum();
                  sensdata.angle.yaw |= data;
                  break;
                case 2: //Accelerometer Sensor (Accel_t)
                  //Clearing the Accel_t structure
                  sensdata.accel.x = 0;
                  sensdata.accel.y = 0;
                  sensdata.accel.z = 0;
                  //Getting the data
                  data = readsum();
                  sensdata.accel.x |= (data) << 8;
                  data = readsum();
                  sensdata.accel.x |= data;
                  data = readsum();
                  sensdata.accel.y |= (data) << 8;
                  data = readsum();
                  sensdata.accel.y |= data;
                  data = readsum();
                  sensdata.accel.z |= (data) << 8;
                  data = readsum();
                  sensdata.accel.z |= data;
                  break;
                case 3: //Angular Rate Sensor (Ang_rate_t)
                  //Clearing the Ang_rate_t structure
                  sensdata.ang_rate.x = 0;
                  sensdata.ang_rate.y = 0;
                  sensdata.ang_rate.z = 0;
                  //Getting the data
                  data = readsum();
                  sensdata.ang_rate.x |= (data) << 8;
                  data = readsum();
                  sensdata.ang_rate.x |= data;
                  data = readsum();
                  sensdata.ang_rate.y |= (data) << 8;
                  data = readsum();
                  sensdata.ang_rate.y |= data;
                  data = readsum();
                  sensdata.ang_rate.z |= (data) << 8;
                  data = readsum();
                  sensdata.ang_rate.z |= data;
                  break;
                case 4: //Gps Position Sensor (Gps_pos_t)
                  //Clearing the Gps_pos_t structure
                  sensdata.gps_pos.lat = 0;
                  sensdata.gps_pos.lon = 0;
                  sensdata.gps_pos.alt = 0;
                  sensdata.gps_pos.gspeed = 0;
                  //Getting the data
                  data = readsum();
                  sensdata.gps_pos.lat |= (data) << 24;
                  data = readsum();
                  sensdata.gps_pos.lat |= (data) << 16;
                  data = readsum();
                  sensdata.gps_pos.lat |= (data) << 8;
                  data = readsum();
                  sensdata.gps_pos.lat |= data;
                  data = readsum();
                  sensdata.gps_pos.lon |= (data) << 24;
                  data = readsum();
                  sensdata.gps_pos.lon |= (data) << 16;
                  data = readsum();
                  sensdata.gps_pos.lon |= (data) << 8;
                  data = readsum();
                  sensdata.gps_pos.lon |= data;
                  data = readsum();
                  sensdata.gps_pos.alt |= (data) << 24;
                  data = readsum();
                  sensdata.gps_pos.alt |= (data) << 16;
                  data = readsum();
                  sensdata.gps_pos.alt |= (data) << 8;
                  data = readsum();
                  sensdata.gps_pos.alt |= data;
                  data = readsum();
                  sensdata.gps_pos.gspeed |= (data) << 24;
                  data = readsum();
                  sensdata.gps_pos.gspeed |= (data) << 16;
                  data = readsum();
                  sensdata.gps_pos.gspeed |= (data) << 8;
                  data = readsum();
                  sensdata.gps_pos.gspeed |= data;
                  break;
                case 5: //Wind Sensor (Wind_t)
                  //Clearing the Wind_t structure
                  sensdata.wind.speed = 0;
                  sensdata.wind.dir = 0;
                  //Getting the data
                  data = readsum();
                  sensdata.wind.speed |= (data) << 8;
                  data = readsum();
                  sensdata.wind.speed |= data;
                  data = readsum();
                  sensdata.wind.dir |= (data) << 8;
                  data = readsum();
                  sensdata.wind.dir |= data;
                  break;
                case 6: //Light Sensor (Light_t)
                  //Clearing the Light_t struct
                  sensdata.light.lux = 0;
                  sensdata.light.temp = 0;
                  //Getting the data
                  data = readsum();
                  sensdata.light.lux |= (data) << 24;
                  data = readsum();
                  sensdata.light.lux |= (data) << 16;
                  data = readsum();
                  sensdata.light.lux |= (data) << 8;
                  data = readsum();
                  sensdata.light.lux |= data;
                  data = readsum();
                  sensdata.light.temp |= (data) << 8;
                  data = readsum();
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
          DT_ck_a = byte(data); //Getting byte checksum A
          DT_step++;
          break;
        case 8:
          DT_ck_b = byte(data); //Getting byte checksum B
          //println("Dat."+hex(DT_ck_a)+":"+hex(DT_ck_b));
          //println("Res."+hex(ck_a)+":"+hex(ck_b));
          
          if((ck_a==DT_ck_a) && (ck_b==DT_ck_b)){ //Chacksum is correct
            //nb->sensors = sensdata;
            pack_ok = true;
            keepgoing = false;
            //_mux.set(-1); //Inhibits the multiplexing
          }
          else{ //Checksum is not correct
            if(tries-- == 0) keepgoing = false;
            pack_ok = false;
            lostpack++;
            DT_step=0;
            ck_a = 0;
            ck_b = 0;
          }
          DT_step++;
          break;
        default:
          break;
      }
    }
    while (_stmRef.available() > 0) _stmRef.read(); //Clearing the Serial buffer
  }
}
