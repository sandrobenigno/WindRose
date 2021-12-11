//WindRose Sensor Classes

class Accel_t
{
  public int x;
  public int y;
  public int z;
};

class Angle_t
{
  public int pitch; //float * 100
  public int roll; //float * 100
  public int yaw; //float * 100
};

class Ang_rate_t
{
  public int x;
  public int y;
  public int z;
};

class Coord_t
{
  public int x;
  public int y;
  public int z;
};

class  Gps_pos_t
{
  public long lat;
  public long lon;
  public long alt;
  public long gspeed;
}; //Repensar a questão do mux

class Wind_t
{
  public int speed;
  public int dir;
};


class Light_t
{
  public long lux; //x1000
  public int temp;
};

/* Structure of Sensing Data (Matches the ASBM bit sequence)*/

public enum sensor_types {
    coord("Coordinates"),
    angle("Angle"),
    accel("Acceleration"),
    ang_rate("Angular Rate"),
    gps_pos("GPS Position"),
    wind("Wind"),
    light("Light");
    
    public final String label;
    
    private sensor_types(String label) {
        this.label = label;
    }
};

class SensingData_t
{
  //[Type] [atrib] [bit_pos in ASBM by sensor_types]
  public Coord_t coord = new Coord_t();//0
  public Angle_t angle = new Angle_t();//1
  public Accel_t accel = new Accel_t();//2
  public Ang_rate_t ang_rate = new Ang_rate_t();//3
  public Gps_pos_t gps_pos = new Gps_pos_t();//4
  public Wind_t wind = new Wind_t();//5
  public Light_t light = new Light_t();//6
};
