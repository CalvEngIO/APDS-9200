#include <APDS9200.h>

// #include "APDS9200.h"
#include "Wire.h"

APDS9200 light;

void setup() {
  Serial.begin(9600);
  Wire.begin();
}

void loop() {
  // put your main code here, to run repeatedly:

  delay(500);
  Serial.print("UV Light:\t");
  light.enableUV();
  delay(100);
  Serial.print( light.getUV() );
  Serial.print("\t\t");

  Serial.print("Light:  ");
  light.enableLight();
  delay(100);
  Serial.println( light.getLight());
  Serial.println("a whole new world!");

}
