void customize(DropdownList ddl) {
  ddl.setWidth(180);
  ddl.setOpen(false);
  ddl.setBackgroundColor(color(190));
  ddl.setItemHeight(20);
  ddl.setBarHeight(15);
  ddl.setColorBackground(color(60));
  ddl.setColorActive(color(0,200, 200));
}

void controlEvent(ControlEvent theEvent){
  if (theEvent.isController()) {
    drawBG();
    drawGrid();
    println("event from controller --> "+theEvent.getController().getValue()+" from "+theEvent.getController());
    int port = int(theEvent.getController().getValue());
    if(_stmRef != null){
      _stmRef.stop();
      _stmRef = null;
    }
    try{
      _stmRef = new Serial(this,portNames[port],BAUDRATE);
      _stmRef.buffer(64);
      delay(100);
      ready = true;
    }catch(Exception e){
      System.err.println("Error opening serial port " + portNames[port]);
      e.printStackTrace();
      ready = false;
    }
  }
}
