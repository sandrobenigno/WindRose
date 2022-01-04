/**
 * @file Sensing.h
 * @brief This file contains the Sensing types, the sensors enumeration, and defines the sensing holder SensingData.
 * @author Sandro Benigno
 * @version 1.0
 * @date 2021-12-31
 */

#ifndef Sensing_h
#define Sensing_h

/**
 * @brief Definition of all sensor bytes summed up in a single packet
 */
#define MAX_SENSING_BYTES_PER_PACKET 50 // Max number of bytes per packet, summing all sensing bytes for all sensors

/**
 * @brief Generic Coordinate System sensor type (ID/ASBM Bit = 0)
 * @param x The x-axis coordinate
 * @param y The y-axis coordinate
 * @param z The z-axis coordinate
 */
typedef struct
{
  int16_t x;
  int16_t y;
  int16_t z;
} Coord_t;

/**
 * @brief IMU/AHRS sensor type (ID/ASBM Bit = 1)
 * @param pitch The pitch angle
 * @param roll The roll angle
 * @param yaw The yaw angle
 */
typedef struct
{
  int16_t pitch; //float * 100
  int16_t roll; //float * 100
  int16_t yaw; //float * 100
} Angle_t;

/**
 * @brief Accelerometer sensor type (ID/ASBM Bit = 2)
 * @param x The x-axis acceleration
 * @param y The y-axis acceleration
 * @param z The z-axis acceleration
 */
typedef struct
{
  int16_t x;
  int16_t y;
  int16_t z;
} Accel_t;

/**
 * @brief Gyroscope sensor type (ID/ASBM Bit = 3)
 * @param x The x-axis angular velocity
 * @param y The y-axis angular velocity
 * @param z The z-axis angular velocity
 */
typedef struct
{
  int16_t x;
  int16_t y;
  int16_t z;
} Ang_rate_t;


/**
 * @brief GPS sensor type (ID/ASBM Bit = 4)
 * @param lat The latitude
 * @param lon The longitude
 * @param alt The altitude
 * @param gspeed The ground speed
 */
typedef struct
{
  int32_t lat;
  int32_t lon;
  int32_t alt;
  int32_t gspeed;
} Gps_pos_t; //Repensar a questão do mux

/**
 * @brief Wind sensor type (ID/ASBM Bit = 5)
 * @param speed The wind speed
 * @param dir The wind direction
 */
typedef struct
{
  int16_t speed;
  int16_t dir;
} Wind_t;

/**
 * @brief Light sensor type (ID/ASBM Bit = 6)
 * @param lux The light intensity
 * @param temp The light temperature in Kelvin
 */
typedef struct
{
  uint32_t lux; //x1000
  uint16_t temp;
} Light_t;

/**
 * @brief Sensor types enumeration
 * @details This enumeration defines all possible sensor types. \n
 * The sequence here defines the ID of the sensor \n
 * and it does match the bit position of the ASBM \n
 * from its least significant bit to the most significant bit
 */
enum sensor_types {
    coord    = 0,
    angle    = 1,
    accel    = 2,
    ang_rate = 3,
    gps_pos  = 4,
    wind     = 5,
    light    = 6
};

/**
 * @brief Sensing holder structure
 * with all the sensor types, ordered by the ASBM bits
 * from its least significant bit to its most significant bit
 */
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