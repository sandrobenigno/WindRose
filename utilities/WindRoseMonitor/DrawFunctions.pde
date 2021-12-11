int oft = 24;

void drawBG(PImage _img){
  //Drawing the background
  background(0);
  image(_img, 1280 - 460, 0);
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
      rect(0,stp-oft,180,20,10,10,10,0);
      fill(200);
      textSize(12);
      text(i,5,stp-10);
    }
  popMatrix();
}

String MsConversion(int MS) {
  int totalSec= (MS / 1000);
  int seconds = (MS / 1000) % 60;
  int minutes = (MS / (1000*60)) % 60;
  int hours = ((MS/(1000*60*60)) % 24);                      

  return nf(hours,2)+": " +nf(minutes,2)+ ": "+ nf(seconds,2);
}

void drawLPPS(){
  fill(255, 0, 0);
  textSize(16);
  text("LPPM: "+lostrate, 1200, 780);
}

void drawLifetime(){
  fill(180, 180, 0);
  textSize(16);
  text("Lifetime: "+MsConversion(lifetime), 1000, 780);
}

void drawSensorName(String name, int pos){
  int offset = oft * pos;
  fill(127,250,150);
  textSize(16);
  text(name, 30, 13+offset);
}

void drawSensorData(String data, int pos){
  int offset = oft * pos;
  fill(127,250,150);
  textSize(16);
  text(data, 220, 16+offset);
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
