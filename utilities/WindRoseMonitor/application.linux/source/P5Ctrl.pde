void customize(Button bt, boolean st) {
  bt.setLabel("Start");
  if(!st){
    bt.lock();
  }
  else bt.unlock();
  bt.setId(1);
  bt.setPosition(1020,10);
  bt.setWidth(50);
  bt.setHeight(15);
  bt.setColorBackground(color(60));
  bt.setColorActive(color(0,200, 200));
}

void customize(DropdownList ddl) {
  ddl.setId(0);
  ddl.setWidth(180);
  ddl.setOpen(false);
  ddl.setBackgroundColor(color(190));
  ddl.setItemHeight(20);
  ddl.setBarHeight(15);
  ddl.setColorBackground(color(60));
  ddl.setColorActive(color(0,200, 200));
}

boolean openSerial(){
  try{
    _stmRef = new Serial(this,portNames[serial_port],BAUDRATE);
    _stmRef.buffer(64);
    delay(100);
    ready = true;
    return true;
  }catch(Exception e){
    System.err.println("Error opening serial port " + portNames[serial_port]);
    e.printStackTrace();
    ready = false;
    return false;
  }
}

void controlEvent(ControlEvent theEvent){
  if (theEvent.isController()) {
    drawBG();
    drawGrid();
    println("event from controller --> "+theEvent.getController().getValue()+" from "+theEvent.getController());
    
    switch(theEvent.getController().getId()){
      case 0:
        serial_port = int(theEvent.getController().getValue());
        customize(bt_StartStop, true);
        if(_stmRef != null){
          _stmRef.stop();
          _stmRef = null;
          ready = false;
        }
        break;
      case 1:
        if(ready){
          if(_stmRef != null){
            ready = false;
            _stmRef.stop();
            _stmRef = null;
            theEvent.getController().setLabel("Start");
          }
        }
        else{
            if(openSerial())
            theEvent.getController().setLabel("Stop");            
        }
        break;
    }
  }
}
