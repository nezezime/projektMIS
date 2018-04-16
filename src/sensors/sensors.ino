#include <HCSR04.h> // Distance sensor lib
#include <SparkFun_ADXL345.h> // Acceleration sensor lib

#define TrigPin 11 // HCSR04 sensor trig pin
#define EchoPin 12 // HCSR04 sensor echo pin
#define accelRange 2 // Set accel sensor range, accepted values are 2g, 4g, 8g or 16g

UltraSonicDistanceSensor distanceSensor(TrigPin, EchoPin);  // Initialize distance sensor
ADXL345 adxl = ADXL345(); //I2C communication with accel sensor

void setup () {
    Serial.begin(9600);
    adxl.powerOn(); // Power on the accel sensor
    adxl.setRangeSetting(accelRange); // Accel range settings
}

void loop () {
    int x,y,z;   
    adxl.readAccel(&x, &y, &z); // Read the accelerometer values and store them
    Serial.print(x);
    Serial.print(", ");
    Serial.print(y);
    Serial.print(", ");
    Serial.println(z);
    double dist = distanceSensor.measureDistanceCm();
    Serial.println(dist); // Print the distance in centimeters
    Serial.println();

    delay(1000);
}
