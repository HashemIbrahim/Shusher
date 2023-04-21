#include "Ultrasonic.h"
#include "TFT_eSPI.h"

//Class Initializers
TFT_eSPI tft;
Ultrasonic ultrasonic(0);

void setup()
{
 //displayDistance =============================================================================================================================
 tft.begin();
 tft.fillScreen(TFT_BLACK);
 tft.setRotation(3);
 tft.setTextSize(5);
 tft.setCursor((320 - tft.textWidth("Shusher")) / 2, 110);
 tft.print("Shusher");
 delay(1000);
 tft.fillScreen(TFT_BLACK);

 Serial.begin(9600);
 //=============================================================================================================================================
}
void loop()
{
rangeFinder();
}

//Functions

//Ultrasonic Range Finder Sensor
void rangeFinder(){
 long RangeInCentimeters;
 //Measues distance in CM with short delay to ensure fast and accurate measurements
 RangeInCentimeters = (double)ultrasonic.MeasureInCentimeters();
 Serial.print(Rangecd SEMREPInCentimeters);//0~400cm
 Serial.println(" cm");

 tft.setCursor(0, 0);
 tft.setTextSize(2);
 tft.drawNumber(RangeInCentimeters, 0, 0);
 tft.drawString("cm", 38, 0);
 //Short delay to ensure quick refreshes of data measurements
 delay(100);

}