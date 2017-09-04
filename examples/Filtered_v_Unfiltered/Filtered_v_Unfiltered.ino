#include "APDS9200.h"
// #include <C:\Users\Trevor Meyer\Documents\GitHub\APDS-9200\APDS9200.h>

#include "Wire.h"

APDS9200 light;

void setup() {
  Serial.begin(9600);
  Wire.begin();
}

void loop() {
  // put your main code here, to run repeatedly:

  delay(500);
  Serial.print("UV Light (Filter vs Unfilter:\t");
  light.enableUV();
  delay(100);
  Serial.print( light.getUV() );
  Serial.print("\t");
  Serial.println(light.getUVUnfiltered() );

  Serial.print("Light (Filter vs Unfilter):  ");
  light.enableLight();
  delay(100);
  Serial.print( light.getLight());
  Serial.print("\t");
  Serial.println( light.getLightUnfiltered() );
  Serial.println();
}
