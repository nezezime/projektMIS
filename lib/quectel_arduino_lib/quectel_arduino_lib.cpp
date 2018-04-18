
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

bool quectelArduinoClass::at_ping(String ip_addr, int timeout){

    String command = "AT+NPING="+ ip_addr + ",12,5000\r";
    Serial.println(command);
    send_at_command(command);
    delay(timeout);
    char * resp = get_at_response();
    Serial.println(crop_at_response());

    return true;
}

bool quectelArduinoClass::create_socket(String port){
    /*  
    //TCP
    //create TCP socket
    sendCommandAndPrintResp("AT+NSOCR=STREAM,6,1234,2", 3000); //ustvari socket, ALI TCP DELUJE?

    //connect to TCP server
    //String command = ;
    sendCommandAndPrintResp("AT+NSOCO=0,83.212.127.86,1024", 3000);

    //send tcp
    //sendCommandAndPrintResp();

    //close session
    sendCommandAndPrintResp("AT+NSOCR=0",1000);
    */
    

    //UDP
    sendCommandAndPrintResp("AT+NSOCR=DGRAM,17,1235,1", 3000); //open UDP socket
    sendCommandAndPrintResp("AT+NSOST=0,83.212.127.86,80,20,5175656374656C204E422D496F542044656D6F0D,1", 3000); //send some data
    sendCommandAndPrintResp("AT+NSOST=0,83.212.127.86,80,2,AB30,2", 3000);
    sendCommandAndPrintResp("AT+NSOCL=0", 1000); //close socket
    
    return true;
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

void quectelArduinoClass::sendCommandAndPrintResp(String command, unsigned int timeout){

    Serial.println(command);
    send_at_command(command);
    delay(timeout);
    char * resp = get_at_response();
    Serial.println(crop_at_response());
}



//do not use the hardware serial pins
bool quectelArduinoClass::init(int rxPin, int txPin, String nBand, String APN, String forceOperator){

    quectelSerial = new SoftwareSerial(rxPin, txPin);
    quectelSerial->begin(9600);
    quectelSerial->listen();

    String command;

    //send handshake
    /*
    send_at_command("AT");
    char *resp = get_at_response();

    delay(1000);

    Serial.print("AT command response: ");
    Serial.println(crop_at_response());
    */

    //test the serial conection
    //PrintResp("AT+NRB",10000);
    sendCommandAndPrintResp("AT", 300);

    //get the existing configuration
    sendCommandAndPrintResp("AT+NCONFIG?",300);
    sendCommandAndPrintResp("AT+CGDCONT?",300);

    //////////////////////////////initial configuration////////////////////////////////////////////
    sendCommandAndPrintResp("AT+CFUN=0", 5000); //disable radio
    sendCommandAndPrintResp("AT+NCONFIG=\"AUTOCONNECT\",\"FALSE\"", 300);
    sendCommandAndPrintResp("AT+NCONFIG=\"CR_0354_0338_SCRAMBLING\",\"TRUE\"", 5000);
    sendCommandAndPrintResp("AT+NCONFIG=\"CR_0859_SI_AVOID\",\"TRUE\"", 300);
    sendCommandAndPrintResp("AT+CEREG=2", 10000); //connect to iot core


    /////////////////////////////connect to network///////////////////////////////////////////////
    command = "AT+CGDCONT=1,\"IP\",\"" + APN + "\"";
    sendCommandAndPrintResp(command, 10000);
    sendCommandAndPrintResp("AT+CFUN=1", 25000); //enable radio

    command = "AT+COPS=1,2,\""+ forceOperator +"\"";
    sendCommandAndPrintResp(command, 10000);

    sendCommandAndPrintResp("AT+CSQ", 5000); //get radio signal quality
    sendCommandAndPrintResp("AT+NBAND?", 500);//get the radio channel used
    sendCommandAndPrintResp("AT+CGPADDR", 500);//show the device ip

    /////////////////////////////try the connection///////////////////////////////////////////////
    //sendCommandAndPrintResp("AT+CSCON=1", 1000); //omogoca izpisovanje connection statusa ob vsakem radijskem dogodku
    //sendCommandAndPrintResp("AT+NPING=8.8.8.8,12,1000", 3000); // ping google dns
    //sendCommandAndPrintResp("AT+NPING=83.212.127.86,12,1000", 3000);//ping our server

    at_ping("8.8.8.8", 1000);
    at_ping("83.212.127.86", 1000);
  
    //set frequency band - NOT REQUIRED
    /*
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
    */
    /*
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
    command = "AT+CPSMS?";
    Serial.println(command);
    send_at_command(command);
    resp = get_at_response();
    Serial.println(crop_at_response());

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
    */
    return true;
}

quectelArduinoClass quectelArduino;


