
#include "quectel_arduino_lib.h"


int quectelArduinoClass::send_at_command(String command){

    command = command + "\r";

    const char * buf = command.c_str();

    //send data
    quectelSerial->listen()
    quectelSerial->write(buf);

    //wait for response
    delay(100);

    return quectelSerial->available();
}

char * quectelArduinoClass::get_at_response(){

    while(!quectelSerial->listen()){
        delay(500);
        Serial.println("Waiting for serial response");
    }

    int nSybols = quectelSerial->available();

    //char * retval = (char *) malloc(128*sizeof(char));

    //naredi s timeoutom

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
bool quectelArduinoClass::init(int rxPin, int txPin, String nBand, String APN, String forceOperator){

    quectelSerial = new SoftwareSerial(rxPin, txPin);

    quectelSerial->begin(9600);
    quectelSerial->listen();


    //send handshake
    send_at_command("AT");
    char resp = get_at_response();

    delay(1000);

    Serial.print("AT command response: ");
    Serial.println(resp);
    
    //set frequency band
    String command = "AT+NBAND=" + nBand;
    Serial.println(command);
    send_at_command(command);
    resp = get_at_response();
    Serial.println(resp);

    //set apn
    delay(1000);
    command = "AT+CGDCONT=1,\"IP\",\"" + APN + "\"";
    Serial.println(command);

    //connect to iot core

    //set indications to active
    //command = "AT+NSMI=1";
    //Serial.println(command);

    //command = "AT+NNMI=1";
    //Serial.println(command);

    //set radio active
    command = "AT+CFUN=1";
    Serial.println(command);

    //get radio signal quality
    command = "ST+CSQ";
    Serial.println(command);

    //force the module to connect to network
    command = "AT+COPS=1,2,\""+ forceOperator +"\"";
    Serial.println(command);

    //NCONFIG?

    //preveri povezavo
    command = "AT+CGATT";
    Serial.println(command);

    //za debug
    command = "AT+CPSMS?";
    Serial.println(command);

    
    return true;
}

quectelArduinoClass quectelArduino;


