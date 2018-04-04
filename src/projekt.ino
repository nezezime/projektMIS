#include <SoftwareSerial.h>

int led_pin = 13;

SoftwareSerial quectel_serial(8,7);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("session started");
  quectel_serial.begin(9600);

  pinMode(led_pin, OUTPUT);
  digitalWrite(led_pin, HIGH);
}

byte ry_byte = 0;
char buff[100];

void loop() {
    quectel_serial.listen();
    quectel_serial.write("AT\r");

    String a;
    

    delay(1000);

    int i = quectel_serial.available();
    Serial.print("bytes available: ");
    Serial.println(i);
    
    int cnt;
    
    for(cnt = 0; cnt <i; cnt++){
      char c = quectel_serial.read();
      Serial.print((int)c);
    }
    Serial.println("");
    
    
    delay(1000);
}
