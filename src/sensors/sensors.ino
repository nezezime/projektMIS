#include <HCSR04.h> // Distance sensor lib
#include <SparkFun_ADXL345.h> // Acceleration sensor lib

#define TIMER1 1000 //namesto delay(1000);
#define TrigPin 11 // HCSR04 sensor trig pin
#define EchoPin 12 // HCSR04 sensor echo pin
#define accelRange 2 // Set accel sensor range, accepted values are 2g, 4g, 8g or 16g

//-----INITIALIZATION-----
UltraSonicDistanceSensor distanceSensor(TrigPin, EchoPin);  // Initialize distance sensor
ADXL345 adxl = ADXL345(); //I2C communication with accel sensor

//-----GLOBAL VARIABLES-----
double dist = 0;
int accelX, accelY, accelZ = 0;
bool measurementError1 = 0; //distance sensor returned "-1" 100x
bool measurementError2 = 0; //accelerometer returned <200 in y axes therefore lid is open
bool interruptFromAccel, interruptFromTimer = 0;

//-----SETUP-----
void setup () {
    Serial.begin(9600);
    adxl.powerOn(); // Power on the accel sensor
    adxl.setRangeSetting(accelRange); // Accel range settings
    measurement(); //izvede prvo meritev
}

//-----LOOP-----
void loop () {
  /*
    int xx,yy,zz;   
    adxl.readAccel(&xx, &yy, &zz); // Read the accelerometer values and store them
    Serial.print(xx);
    Serial.print(", ");
    Serial.print(yy);
    Serial.print(", ");
    Serial.println(zz);
    double distance = distanceSensor.measureDistanceCm();
    Serial.println(distance); // Print the distance in centimeters
    Serial.println();
*/
    measurement();
    Serial.print("Distance [cm] = ");
    Serial.println(dist);
    Serial.print("accelY = ");
    Serial.println(accelY);
    Serial.print("measurementError1 = ");
    Serial.println(measurementError1);
    Serial.print("measurementError2 = ");
    Serial.println(measurementError2);
    Serial.println();
    delay(TIMER1);

    if (interruptFromAccel) {
      delay(60 * TIMER1);
      measurement();
      if (measurementError2) {
        //pokrov odprt
        for (int i = 0; i < 10; i++) {
          delay(60 * TIMER1); //pocakaj 1 min
          if (!measurementError2) {
            break;
          }
          if (i == 9) {
            //na server poslji measurementError2=1 + interruptFromAccel=1
          }
        }
      }
      if (!measurementError2) {
        if (measurementError1) {
          //pokrov zaprt + senzor razdalje ne dela
          //na server poslji accelY + measurementError1=1 + interruptFromAccel=1
        }
        else {
          //pokrov zaprt + senzor razdalje OK
          //na server poslji dist in accelY + interruptFromAccel=1
          
        }
      }
      interruptFromAccel = 0;
    }
}

//-----FUNCTIONS-----
void measurement() {
  measurementError1 = 0; //distance sensor returned "-1" 100x
  measurementError2 = 0; //accelerometer returned <200 in y axes therefore lid is open
  dist = -1;
  
  updateAccel(); //prebere pospesek
  if (accelY < 200) { //preveri ali je v Y smeri odprt pokrov
    measurementError2 = 1; //1, ce je pokrov odprt
  }
  else {
    updateDist(); //prebere razdaljo v [cm]
  }
}

//MEASURE ACCEL
void updateAccel() {
  adxl.readAccel(&accelX, &accelY, &accelZ);
}

//MEASURE DISTANCE
void updateDist() {
  int i, tmp, cnt1 = 0;
  
  while (cnt1 != 20) {
    tmp = distanceSensor.measureDistanceCm();
    if (tmp > 0) { //ce vrne -1 zavrzi meritev
      dist = dist + tmp;
      cnt1++;
    }
    i++;
    if (i == 100) break;
    delay(20);
  }
  if (dist == 0) {
    measurementError1 = 1;
  }
  else {
    dist = round(dist / cnt1);
  }
}

