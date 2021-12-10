#ifndef Sensing_h
#define Sensing_h

#define MAX_SENSING_BYTES_PER_PACKET 50 // Max number of bytes per packet, summing all sensing bytes for all sensors

typedef struct
{
  int16_t x;
  int16_t y;
  int16_t z;
} Accel_t;

typedef struct
{
  int16_t pitch; //float * 100
  int16_t roll; //float * 100
  int16_t yaw; //float * 100
} Angle_t;

typedef struct
{
  int16_t x;
  int16_t y;
  int16_t z;
} Ang_rate_t;

typedef struct
{
  int16_t x;
  int16_t y;
  int16_t z;
} Coord_t;

typedef struct
{
  int32_t lat;
  int32_t lon;
  int32_t alt;
  int32_t gspeed;
} Gps_pos_t; //Repensar a questão do mux

typedef struct
{
  int16_t speed;
  int16_t dir;
} Wind_t;

typedef struct
{
  uint32_t lux; //x1000
  uint16_t temp;
} Light_t;

/* Structure of Sensing Data (Matches the ASBM bit sequence)*/

enum sensor_types {
    coord,angle,accel,ang_rate,gps_pos,
    wind,light
};

typedef struct
{
  //[Type] [atrib] [bit_pos in ASBM by sensor_types]
  Coord_t coord; //0
  Angle_t angle; //1
  Accel_t accel; //2
  Ang_rate_t ang_rate; //3
  Gps_pos_t gps_pos; //4
  Wind_t wind; //5
  Light_t light; //6
} SensingData;

#endif