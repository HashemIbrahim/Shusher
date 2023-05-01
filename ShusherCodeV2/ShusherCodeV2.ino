// Ultrasonic Ranger Initialization------------------------------------
#include "Ultrasonic.h"
#include "TFT_eSPI.h"
#define LOOP_PERIOD 100
TFT_eSPI tft;
Ultrasonic ultrasonic(0);
TFT_eSprite spr = TFT_eSprite(&tft);

// RGB LED Stick Initialization-----------------------------------------
#include <Adafruit_NeoPixel.h>
#define NUM_LEDS 10
#define DATA_PIN 0
Adafruit_NeoPixel strip(NUM_LEDS, DATA_PIN, NEO_RGB);
//----------------------------------------------------------------------

void setup()
{
 //displayDistance =============================================================================================================================
  //tft setup
 tft.begin();
 tft.fillScreen(TFT_BLACK);
 tft.setRotation(3);

 Serial.begin(9600);
 ledStartupTest();
 //=============================================================================================================================================
}
void loop()
{
rangeFinder();
loudnessSensor();
LoudnessSensorLoudValue();
Messagecalculator();
Serial.println(message);
setLedStick();
delay(100);
}

//Functions

//Ultrasonic Range Finder Sensor
void rangeFinder(){
 long RangeInCentimeters;
 //Measues distance in CM with short delay to ensure fast and accurate measurements
 RangeInCentimeters = (double)ultrasonic.MeasureInCentimeters();
 Serial.print(RangeInCentimeters);//0~400cm
 Serial.println(" cm");

 //Visual Displaying of Data
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
}

//Loudness Sensor
void loudnessSensor() {
  loudnessVal = analogRead(1);

  //Visual Displaying of Data
  tft.setCursor(1,0);
  spr.fillSprite(TFT_WHITE);
  spr.createSprite(320,240);
  spr.fillSprite(TFT_WHITE);
  spr.setTextColor(TFT_BLACK,TFT_WHITE);
  spr.setFreeFont(&FreeSansBoldOblique12pt7b);
  spr.drawString("Volume: ",0,0);
  spr.drawNumber(loudnessVal,70,0);
  spr.pushSprite(0,0);
  spr.deleteSprite();
}

//Loudness Sensor Thresholds
String Messagecalculator() {
if (loudness >= 1 && loudness <= 2) {
message = "Quiet";
}   else if (loudness > 2 && loudness <8) {
message = "Reasonable";  
} else { message = "Loud";}
return message;
}  

  int LoudnessSensorLoudValue() {
   analogRead(0);
	delay(10);

	val = analogRead(0);
  if (val <= Thresholds[0]) {
  (loudness = 1);
  }
  else if (val > Thresholds[0] && val <=  Thresholds[1]) {
   (loudness = 2);
  }
   else if (val >  Thresholds[1] && val <=  Thresholds[2]) {
   (loudness = 3);
  }
   else if (val >  Thresholds[2] && val <=  Thresholds[3]) {
   (loudness = 4);
  }
   else if (val >  Thresholds[3] && val <=  Thresholds[4]) {
   (loudness = 5);
   }
 else if (val >  Thresholds[4] && val <=  Thresholds[5]) {
   (loudness = 6);
  }   
   else if (val >  Thresholds[5] && val <=  Thresholds[6]) {
   (loudness = 7);
  }
   else if (val >  Thresholds[6] && val <=  Thresholds[7]) {
   (loudness = 8);
  }
 else if (val >  Thresholds[7] && val <=  Thresholds[8]) {
   (loudness = 9);
  }  
   else if (val >  Thresholds[8] ) {
   (loudness = 10);
  }
return loudness;  
  }
// RGB LED Stick Functions
void ledStartupTest(){    // Testing that all LEDs work(LightShow ;) )
  strip.begin();
  strip.setBrightness(150);
  strip.clear(); 
  for(int i= 0; i<=NUM_LEDS;i++){
    delay(200);
    if(i<3){
    strip.setPixelColor(i, 255,0,0);       //setting the color of led number 1-3 to green
    
  }
    else if(i<7){
    strip.setPixelColor(i,255,255,0);    //setting the color of led number 4-7 yellow
  }

    else if(i<10) {
    strip.setPixelColor(i,0,255,0);       //Setting the color of LED number 8-9 red
  }
    else if(i== 10){                      //the whole Stick flashes red when the last LED is reached.
       for(int j = 0; j < 5; j++){
        for(int k = 0; k < NUM_LEDS; k++){
          strip.setPixelColor(k, 0, 255, 0);
        }
        strip.show();
        delay(200);
        strip.clear();
        strip.show();
        delay(200);
        }
    }
    delay(150);
    strip.show();
  }
  strip.clear();

}


void setLedStick(){                 //Activating the LEDs dependent on the loudness which is determined by the thresholds set at the top.
  
  if(loudness >= 1){
    strip.setPixelColor(0, 255,0,0);
  }
  if(loudness >= 2){
    strip.setPixelColor(1, 255,0,0);
  }
  if(loudness >= 3){
    strip.setPixelColor(2, 255,0,0);
  }
  if(loudness >= 4){
    strip.setPixelColor(3, 255,255,0);
  }
  if(loudness >= 5){
    strip.setPixelColor(4, 255,255,0);
  }
  if(loudness >= 6){
    strip.setPixelColor(5, 255,255,0);
  }
  if(loudness >= 7){
    strip.setPixelColor(6, 255,255,0);
  }
  if(loudness >= 8){
    strip.setPixelColor(7, 0,255,0);
  }
  if(loudness >= 9){
    strip.setPixelColor(8, 0,255,0);
  }
  
   
    while(loudness >= 10){                 //when the stick is at maximum(meaning led number 10) the whole stick flashes red
     for(int j = 0; j < 5; j++){
        for(int k = 0; k < NUM_LEDS; k++){
          strip.setPixelColor(k, 0, 255, 0);
        }
        strip.show();
        delay(200);
        strip.clear();
        strip.show();
        delay(200);
        }
    }
  
  strip.show();
  } 