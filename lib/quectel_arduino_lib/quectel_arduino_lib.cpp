
#include "quectel_arduino_lib.h"


int quectelArduinoClass::send_at_command(String command){

    //flush the response buffer
    int i = 0;
    while(atResponseBuffer[i] != '\0'){
        atResponseBuffer[i] = '\0';
        i++;
    }

    command = command + "\r";

    const char * buf = command.c_str();

    //send data
    quectelSerial->listen();
    quectelSerial->write(buf);

    //wait for response
    delay(300);

    return quectelSerial->available();
}

char * quectelArduinoClass::get_at_response(){

    int nSybols = quectelSerial->available();
    
    if(nSybols){
        for(int i = 0; i<nSybols; i++){

            atResponseBuffer[i] = quectelSerial->read();
        }
    }

    return &atResponseBuffer[0];
}

char * quectelArduinoClass::crop_at_response(void){

    int i = 0;
    int a;
    int tmpBuffIdx = 0;

    while(atResponseBuffer[i] != '\0'){

        char c = atResponseBuffer[i];
        
        if(c!=13 && c!=10 && c!=6 && c!= 3){
            atResponseBuffer[tmpBuffIdx] = c;

            tmpBuffIdx ++;
        }
        i ++;
    }

    //flush the rest
    for(a=tmpBuffIdx; a<i; a++){
        atResponseBuffer[a] = '\0';
    }

    //Serial.print("atResponseBufferSize: ");
    //Serial.println(atResponseBuffer);

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
    char *resp = get_at_response();

    delay(1000);

    Serial.print("AT command response: ");
    Serial.println(crop_at_response());
    
    //set frequency band
    String command = "AT+NBAND=" + nBand;
    Serial.println(command);
    send_at_command(command);
    resp = get_at_response();
    Serial.println(crop_at_response());

    //show frequency band
    command = "AT+NBAND?";
    Serial.println(command);
    send_at_command(command);
    resp = get_at_response();
    Serial.println(crop_at_response());

    //set apn
    delay(1000);
    command = "AT+CGDCONT=1,\"IP\",\"" + APN + "\""; //cid= caller id, protocol, apn
    Serial.println(command);
    send_at_command(command);
    resp = get_at_response();
    Serial.println(crop_at_response());

    //connect to iot core ????
    command = "AT+CEREG=2";
    Serial.println(command);
    send_at_command(command);
    resp = get_at_response();
    Serial.println(crop_at_response());

    command = "AT+CEREG?";
    Serial.println(command);
    send_at_command(command);
    resp = get_at_response();
    Serial.println(crop_at_response());

    //set indications to active
    //command = "AT+NSMI=1";
    //Serial.println(command);
    //send_at_command(command);
    //resp = get_at_response();
    //Serial.println(crop_at_response());

    //command = "AT+NNMI=1";
    //Serial.println(command);
    //Serial.println(crop_at_response());

    //set radio active
    command = "AT+CFUN=1"; //set this to 0 when not using radio?
    Serial.println(command);
    send_at_command(command);
    delay(7000);
    resp = get_at_response();
    Serial.println(crop_at_response());

    //get radio signal quality
    command = "AT+CSQ";
    Serial.println(command);
    send_at_command(command);
    resp = get_at_response();
    Serial.println(crop_at_response());

    //force the module to connect to network
    command = "AT+COPS=1,2,\""+ forceOperator +"\"";
    Serial.println(command);
    send_at_command(command);
    resp = get_at_response();
    Serial.println(crop_at_response());

    //NCONFIG?

    //returns 1 if the device is attached to the packet domain service
    command = "AT+CGATT?";
    Serial.println(command);
    send_at_command(command);
    delay(3000);
    resp = get_at_response();
    Serial.println(crop_at_response());

    //omogoca nastavljanje PSM (power saving mode)
    /*command = "AT+CPSMS?";
    Serial.println(command);
    send_at_command(command);
    resp = get_at_response();
    Serial.println(crop_at_response());*/

    //show the ip address of the device
    command = "AT+CGPADDR";
    Serial.println(command);
    send_at_command(command);
    resp = get_at_response();
    Serial.println(crop_at_response());


    delay(30000);
    command = "AT+CEREG?";
    Serial.println(command);
    send_at_command(command);
    resp = get_at_response();
    Serial.println(crop_at_response());

    delay(180000);
    command = "AT+CEREG?";
    Serial.println(command);
    send_at_command(command);
    resp = get_at_response();
    Serial.println(crop_at_response());

    return true;
}

quectelArduinoClass quectelArduino;


