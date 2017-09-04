#include "APDS9200.h"
#include "Wire.h"

APDS9200 light;
const int interruptPin = 6;
const int LED = 5;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  pinMode(interruptPin, INPUT);
  pinMode(LED, OUTPUT);

  //For both and upper and lower threshold:
  light.setInterruptThresh(500, 1000);

  //Just the upper light threshold:
  //light.setInterruptThreshUpper(1000);

  //Just the lower light threshold:
  //light.setInterruptThreshLower(500);

  
  light.enableLightInterrupt();
}

void loop() {
  // put your main code here, to run repeatedly:
  
  light.enableLight();
  
  // Sensor is most reliable with at minimum 104ms between readings
  delay(104);

  if(digitalRead(interruptPin) == HIGH)
    digitalWrite(LED, HIGH);
  else
    digitalWrite(LED, LOW);
    
}
