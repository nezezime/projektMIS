//includes
#include "quectel_arduino_lib.h"

//macros
#define QUECTEL_SERIAL_RX 8
#define QUECTEL_SERIAL_TX 7

extern quectelArduinoClass quectelArduino;

void setup() {
  
  //init serial communication
  Serial.begin(9600);
  
  if(quectelArduino.init(QUECTEL_SERIAL_RX, QUECTEL_SERIAL_TX, "8", "v-iot", "29341") == false){
    Serial.println("Failed quectel module initialization");
  }
  else Serial.println("Quectel module serial communication successful");
}

void loop() {
  
  delay(3000);
  quectelArduino.at_ping("83.212.127.86", 1000);
  delay(1000);
  quectelArduino.create_socket("test");
  
}
