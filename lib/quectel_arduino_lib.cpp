#include "quectel_arduino_lib.h"


int quectelArduinoClass::send_at_command(String command){

    command = command + "\r";

    const char * buf = command.c_str();

    //send data
    quectelSerial->listen();
    quectelSerial->write(buf);

    //wait for response
    delay(100);

    return quectelSerial->available();
}

char * quectelArduinoClass::get_at_response(){

    int nSybols = quectelSerial->available();

    //char * retval = (char *) malloc(128*sizeof(char));

    //flush the buffer
    atResponseBuffer[0] = '\0';
    
    if(nSybols){
        for(int i = 0; i<nSybols; i++){

            atResponseBuffer[i] = quectelSerial->read();
        }
    }

    return &atResponseBuffer[0];
}

char * quectelArduinoClass::crop_at_response(void){

    int i = 0;
    int tmpBuffIdx = 0;
    char tmpBuff[128];

    while(atResponseBuffer[i] != '\0'){

        char c = atResponseBuffer[i];

        if(c!=13 && c!=10 && c!=6 && c!= 3){
            tmpBuff[tmpBuffIdx] = c;
            tmpBuffIdx ++;
        }
        i++;
    }

    //Serial.print("atResponseBufferSize: ");
    //Serial.println(i);
    
    //overwrite old buffer
    strncpy(atResponseBuffer, tmpBuff, 128);

    //replace end of text char with null char
    atResponseBuffer[tmpBuffIdx] = '\0';

    /*
    i = 0;
    while(atResponseBuffer[i] != '\0'){
        int bla = (int)atResponseBuffer[i];
        Serial.print(bla);

        i++;
    }

    Serial.print("new atResponseBufferSize: ");
    Serial.println(i);
    */

    return &atResponseBuffer[0];
}

bool quectelArduinoClass::send_handshake(){

    if(send_at_command("AT") == 0){
        return false;
    }

    //if(strcmp(get_at_response().c_str(), "\nOK\n")){
    //    return true;
    //}
}


//do not use the hardware serial pins
bool quectelArduinoClass::init(int rxPin, int txPin){

    quectelSerial = new SoftwareSerial(rxPin, txPin);

    quectelSerial->begin(9600);
    quectelSerial->listen();

    //if(send_handshake() == true) return true;
    //else return false;
    return true;
}

quectelArduinoClass quectelArduino;


