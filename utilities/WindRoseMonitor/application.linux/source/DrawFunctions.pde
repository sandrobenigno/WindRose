int oft = 21;
PImage graf, dots, sens, bg; 

void drawBG(){
  //Drawing the background
  background(0);
  image(bg, 1280 - 460, 5);
  image(sens, 0,0);
}

void drawGrid(){
  pushMatrix();
    translate(panelX,panelY+22);
    int stp = 0;
    for(int i=0; i < 32; i++){
      stp = oft * i;
      strokeWeight(2);
      stroke(60);
      line(200,stp,800,stp);
      line(160,stp-oft,200,stp);
      strokeWeight(1);
      stroke(80);
      fill(30);
      rect(0,stp-oft,180,18,10,10,10,0);
      fill(200);
      textSize(12);
      text(i,4,stp-8);
    }
  popMatrix();
}

String MsConversion(int MS) {
  //int totalSec= (MS / 1000);
  int seconds = (MS / 1000) % 60;
  int minutes = (MS / (1000*60)) % 60;
  int hours = ((MS/(1000*60*60)) % 24);                      

  return nf(hours,2)+": " +nf(minutes,2)+ ": "+ nf(seconds,2);
}

void initImages(){
  bg = loadImage("bg2.png"); //Background
  graf = loadImage("graf.png"); //Clean NGM Tracks
  dots = loadImage("dots.png"); //Pattern for NGM Plot
  sens = loadImage("sens.png"); //Sensor Mapping Title
}

void shiftNGN_Graf(){
  PImage graf_t;
  graf_t = loadImage("graf.png");
  graf_t.copy(graf,3,0,297,80,0,0,297,80);
  graf.copy(graf_t,0,0,300,80,0,0,300,80);
}

void drawNGM(){
  shiftNGN_Graf();
  pushMatrix();
    translate(845,580);
    for(int i=0;i<8;i++){
      fill(127,250,150);
      if((NGM>>i & 0b01) == 1){
        switch(i){
          case 0: //N
            fill(255,50,50,157);
            stroke(0);
            rect(29,0,24,12,5);
            graf.copy(dots,0,0,3,14,297,0,3,14);
            break;
          case 1: //E
            fill(0,255,0,157);
            rect(69,29,12,24,5);
            graf.copy(dots,0,22,3,14,297,22,3,14);
            break;
          case 2: //S
            fill(50,127,255,157);
            rect(29,69,24,12,5);
            graf.copy(dots,0,44,3,14,297,44,3,14);
            break;
          case 3: //W
            fill(255,255,50,157);
            rect(0,29,12,24,5);
            graf.copy(dots,0,66,3,14,297,66,3,14);
            break;
          case 4: //NE
            fill(127,250,150,127);
            triangle(60, 0, 80, 0, 80, 20);
            break;
          case 5: //SE
            fill(127,250,150,127);
            triangle(80, 60, 80, 80, 60, 80);
            break;
          case 6: //SW
            fill(127,250,150,127);
            triangle(20, 80, 0, 80, 0, 60);
            break;
          case 7: //NW
            fill(127,250,150,127);
            triangle(0, 0, 20, 0, 0, 20);
            break;
          default:
            break;
        }
      }
      image(graf,126,-15,300,70);
    }
  popMatrix();
}

void drawLPPS(){
  fill(255, 0, 0);
  textSize(16);
  text("LPPM: "+nf(lostrate,3), 1200, 674);
}

void drawLifetime(){
  fill(180, 180, 0);
  textSize(16);
  text("Lifetime: "+MsConversion(lifetime), 975, 674);
}

void drawSensorName(String name, int pos){
  int offset = oft * pos;
  fill(127,250,150);
  textSize(16);
  text(name, 30, 15+offset);
}

void drawSensorData(String data, int pos){
  int offset = oft * pos;
  fill(127,250,150);
  textSize(16);
  text(data, 220, 17+offset);
}

void drawSensors(){
  pushMatrix();
    translate(panelX,panelY);
    for(int i=0;i<32;i++){
      if((ASBM>>i & 0x01) == 1){
        switch(i){
          case 0:
            drawSensorName(sensor_types.coord.label, i);
            drawSensorData("X: [ "+sensdata.coord.x+" ] Y: [ "+ sensdata.coord.y +" ] Z: ["+ sensdata.coord.z +"]", i);
            break;
          case 1:
            drawSensorName(sensor_types.angle.label, i);
            drawSensorData("Pitch: [ "+sensdata.angle.pitch+" ] Roll: [ "+ sensdata.angle.roll +" ] Yaw: ["+ sensdata.angle.yaw +"]", i);
          case 2:
            drawSensorName(sensor_types.accel.label, i);
            drawSensorData("X: [ "+sensdata.accel.x+" ] Y: [ "+ sensdata.accel.y +" ] Z: ["+ sensdata.accel.z +"]", i);
            break;
          case 3:
            drawSensorName(sensor_types.ang_rate.label, i);
            drawSensorData("X: [ "+sensdata.ang_rate.x+" ] Y: [ "+ sensdata.ang_rate.y +" ] Z: ["+ sensdata.ang_rate.z +"]", i);
            break;
          case 4:
            drawSensorName(sensor_types.gps_pos.label, i);
            drawSensorData("Lat: [ "+sensdata.gps_pos.lat+" ] Lon: [ "+ sensdata.gps_pos.lon +" ] Alt: ["+ sensdata.gps_pos.alt +"] Ground Speed: ["+ sensdata.gps_pos.gspeed +"]", i);
            break;
          case 5:
            drawSensorName(sensor_types.wind.label, i);
            drawSensorData("Speed: [ "+sensdata.wind.speed+" ] Direction: [ "+ sensdata.wind.dir +" ]", i);
            break;
          case 6:
            drawSensorName(sensor_types.light.label, i);
            drawSensorData("lux: [ "+sensdata.light.lux+" ] temp: [ "+sensdata.light.temp +"K ]", i);
            break;
          default:
            break;
        }
      }
    }
  popMatrix();
}
