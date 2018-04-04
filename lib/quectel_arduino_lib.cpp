#include "quectel_arduino_lib.h"

quectelArduinoClass quectelArduino;

int quectelArduinoClass::send_at_command(String command){

    command.append("\n");

    //send data
    quectelSerial.write();

    //wait for response
    delay(100);

    return quectelSerial.available();
}

String quectelArduinoClass::get_at_reponse(){

    int nSybols = quectelSerial.available();
    String retval;

    if(nSybols){
        for(int i = 0; i<nSybols; i++){

            retval += quectel_serial.read();
        }
    }

    return retval;
}

bool quectelArduinoClass::send_handshake(){


}

bool quectelArduinoClass::init(int rxPin, int txPin){

    quectelSerial.begin(9600);

}


