#include "Ultrasonic.h"
#include "TFT_eSPI.h"
#define LOOP_PERIOD 100

//Class Initializers
TFT_eSPI tft;
Ultrasonic ultrasonic(0);
TFT_eSprite spr = TFT_eSprite(&tft);

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
 Serial.print(RangeInCentimeters);//0~400cm
 Serial.println(" cm");

 tft.setCursor(0, 0);
 tft.setTextSize(2);

 spr.fillSprite(TFT_WHITE);
    spr.createSprite(100, 40);
    spr.fillSprite(TFT_WHITE);
    spr.setTextColor(TFT_BLACK, TFT_WHITE);
    spr.setFreeFont(&FreeSansBoldOblique12pt7b);
    spr.drawNumber(RangeInCentimeters, 0, 0);
    spr.drawString(" cm", 50, 0);
    spr.pushSprite(0, 0); 
    spr.deleteSprite();
 //Short delay to ensure quick refreshes of data measurements
 delay(100);



}