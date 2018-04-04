//includes
#include "quectel_arduino_lib.h"

//macros
#define QUECTEL_SERIAL_RX 8
#define QUECTEL_SERIAL_TX 7

extern quectelArduinoClass quectelArduino;

void setup() {

  //init serial communication
  Serial.begin(9600);
  
  if(quectelArduino.init(QUECTEL_SERIAL_RX, QUECTEL_SERIAL_TX) == false){
    Serial.println("Failed quectel module initialization");
  }
  else Serial.println("Quectel module serial communication successful");
}

void loop() {
  quectelArduino.send_at_command("AT");
  char * resp = quectelArduino.get_at_response();
  //Serial.print("a");
  //Serial.print(resp);
  //Serial.print("b");
  delay(1000);

  char bla;

  Serial.println(quectelArduino.crop_at_response());

}
