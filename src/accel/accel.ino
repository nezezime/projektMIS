#include <SparkFun_ADXL345.h> 

ADXL345 adxl = ADXL345();             // I2C COMMUNICATION
int interruptPin = 2;
//int interruptPin = 3;
//int interruptPin = 1;

void setup() {
  Serial.begin(9600);
  Serial.println("setup()");
  
  adxl.powerOn();
  adxl.setRangeSetting(2);           // Give the range settings
                                      // Accepted values are 2g, 4g, 8g or 16g
                                      // Higher Values = Wider Measurement Range
                                      // Lower Values = Greater Sensitivity
  
  adxl.setActivityXYZ(1,1,1);       // Set to activate movement detection in the axes "adxl.setActivityXYZ(X, Y, Z);" (1 == ON, 0 == OFF)
  adxl.setActivityThreshold(30);      // 62.5mg per increment   // Set activity   // Inactivity thresholds (0-255)

  // Setting all interupts to take place on INT1 pin
  adxl.setImportantInterruptMapping(1, 1, 1, 1, 1);     // Sets "adxl.setEveryInterruptMapping(single tap, double tap, free fall, activity, inactivity);" 
                                                        // Accepts only 1 or 2 values for pins INT1 and INT2. This chooses the pin on the ADXL345 to use for Interrupts.
                                                        // This library may have a problem using INT2 pin. Default to INT1 pin.
  adxl.setInterruptLevelBit(1); // positive logic
  
  pinMode(interruptPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), ADXL_ISR, RISING);
  
  // Turn on Interrupt (1 == ON, 0 == OFF)
  adxl.ActivityINT(1);

  Serial.println("END SETUP");
}

void loop(){
  
}

void ADXL_ISR() {
  
  // getInterruptSource clears all triggered actions after returning value
  // Do not call again until you need to recheck for triggered actions
  byte interrupts = adxl.getInterruptSource(); // mogoce bi blo bols na konc funkcije?
  
  Serial.println("*** INSIDE ADXL_ISR ***"); 
  
  // Activity
  if(adxl.triggered(interrupts, ADXL345_ACTIVITY)){
    Serial.println("*** ACTIVITY ***"); 
     //add code here to do when activity is sensed
  }
}

