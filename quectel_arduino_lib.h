#ifndef quectel_arduino_lib_h
#define quectel_arduino_lib_h

//includes
#include <SoftwareSerial.h>
#include <string.h>

//declarations
class quectelArduinoClass(){
    private:
        
    public:
        SoftwareSerial quectelSerial(8,7); //naredi init funkcijo
        int send_at_command(String command);
        String get_at_response(void);
        bool send_handshake(void);
        bool init(int rxPin, int txPin);


}

extern quectelArduinoClass quectelArduino; 

#endif
