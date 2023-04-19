#include "Ultrasonic.h"
#include "TFT_eSPI.h"
TFT_eSPI tft;

Ultrasonic ultrasonic(0);
void setup()
{
 Serial.begin(9600);
}
void loop()
{
 long RangeInCentimeters;
//Measues distance in CM with short delay to ensure fast and accurate measurements
 RangeInCentimeters = ultrasonic.MeasureInCentimeters();
 Serial.print(RangeInCentimeters);//0~400cm
 Serial.println(" cm");
 delay(100);
}