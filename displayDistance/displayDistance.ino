#include "Ultrasonic.h"
#include"TFT_eSPI.h"

Ultrasonic ultrasonic(0);
TFT_eSPI tft = TFT_eSPI();
void setup()
{
 Serial.begin(9600);
 //Color and Text Settings before run
 tft.setTextColor(TFT_WHITE);
 tft.setTextSize(2);
}
void loop()
{
 long RangeInCentimeters;
 //Ultrasonic method that measures distance in centimeters
 RangeInCentimeters = ultrasonic.MeasureInCentimeters();
 //Centers text in screen
 tft.setCursor((320 - tft.textWidth(RangeInCentimeters + " cm")) / 2, 120);
 tft.println(RangeInCentimeters + " cm");
 delay(200);
}