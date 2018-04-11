
#ifndef quectel_arduino_lib_h
#define quectel_arduino_lib_h

//includes
#include <SoftwareSerial.h>
#include <string.h>
#include <Arduino.h>

//declarations
class quectelArduinoClass{
    private:
        SoftwareSerial * quectelSerial;
        char atResponseBuffer[128];

    public:
        int send_at_command(String command);
        char * get_at_response(void);
        bool send_handshake(void);
        bool init(int rxPin, int txPin, String nBand, String APN, String forceOperator);
        char * crop_at_response(void); 
        void sendCommandAndPrintResp(String command, unsigned int timeout);

};

//extern quectelArduinoClass quectelArduino; 

#endif
