//--MQTT--------------------------------------------------------------
#include <rpcWiFi.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "Ultrasonic.h"
//for simplicity, the internet settings are hardcoded for now but will be a part of a header file later in the git ignore.
const char* ssid = "ISAACHP";
const char* password = "isaac123";
const char* server = "192.168.137.1";
WiFiClient wioClient;
PubSubClient client(wioClient);
//---------------------------------------------------------------------
//--Setting constants--------------------------------------------------------------
#define sampleWindow 5
#define peakToPeakAverages 7
#define brightnesslevellights 20
#define DEBUGSERIAL
#define DEBUGPRINTING 
#define DEBUGWIFI
//#define DEBUGMQTT
int const ranges []= {15,40,60,75,100};
float const sensvalues [] = {1,1.75,2,2.35,2.8,3.3};
//---------------------------------------------------------------------------------
//--Setting the Ranger--------------------------------------------------------------
Ultrasonic ultrasonic(2);
//---------------------------------------------------------------------------------
//--Setting the screen--------------------------------------------------------------
#include "Ultrasonic.h"
#include "TFT_eSPI.h"
#define LOOP_PERIOD 1
TFT_eSPI tft;
TFT_eSprite spr = TFT_eSprite(&tft);
//--Setting variables--------------------------------------------------------------
int loudness;
int loudnessMaxReachedCount;
float Sens = 3;
float baseThreshold = 49;
long RangeInCentimeters;
float Thresholds[10];
int currentrange;
float decibels;

// RGB LED Stick-------------------------------------------------------
#include <Adafruit_NeoPixel.h>
#define NUM_LEDS 10
#define DATA_PIN 0
Adafruit_NeoPixel strip(NUM_LEDS, DATA_PIN, NEO_RGB);
//----------------------------------------------------------------------


void setup() {
 setupWIFI();
 setupSerial();
 setupScreen();
 ledStartupTest();
 setupMic();
}

void loop() {
 ThresholdCalculator();
 LoudnessSensorLoudValue();
 //if (isDataChanged()) {
 displayDataSPR();
 
 setLedStick();
 #ifdef DEBUGMQTT
 if (!client.connected()) {
    reconnect();
  }
 #endif

}
//Functions
void setupScreen() {
 tft.begin();
 tft.init();
 tft.setRotation(3);
 tft.fillScreen(TFT_BLACK);
}
void setupWIFI() { //connects to the wifi
  #ifdef DEBUGWIFI
  WiFi.begin(ssid, password);
  client.setServer(server,1883);
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected");
  }
  #endif
}

void setupSerial() { //starts the serial for input
  #ifdef DEBUGSERIAL
    Serial.begin(115200);
  #endif
}

void setupMic() { //sets what to use as the mic
  pinMode(WIO_MIC, INPUT);
}
void ThresholdCalculator() {
  currentrange = rangeFinder(); //gets the range from the ranger right now
  //calulates what the sensitivity should be based on that range
  if (currentrange < ranges[0] ) {Sens=sensvalues[5];}
  else if (currentrange >= ranges[0] && currentrange < ranges[1]) {Sens=sensvalues[4];}
  else if ((currentrange >= ranges[1] && currentrange < ranges[2]) ) {Sens=sensvalues[3];}
  else if (currentrange >= ranges[2] && currentrange < ranges[3] ) {Sens=sensvalues[2];}
  else if (currentrange >= ranges[3] && currentrange < ranges[4]) {Sens=sensvalues[1];}
  else if (currentrange >= ranges[4]) {Sens=sensvalues[0];}
  
  // printing for debugging
  #ifdef DEBUGPRINTING
  for (int i = 0; i < 10; i++) {
    Serial.print(Thresholds[i]); // print the value of the current element
    Serial.print(" "); // add a space to separate values
  }
  Serial.println();
  #endif

 //sets the threshold values based on this sensitivity
 for (int i = 0; i < 10; i++) {
   Thresholds[i] =  baseThreshold + (i * Sens);
}
 
}
//Loudness Sensor Thresholds
void LoudnessSensorLoudValue() {

  //this portion of code was taken from https://how2electronics.com/iot-decibelmeter-sound-sensor-esp8266/ it creates whats known as an 'envelope' to encompass the sound. This is necessary due to the way the analog value is recorded and how sound is a wave.
  float peakToPeak = 0;                                  // peak-to-peak level
  unsigned int signalMax = 0;                            //minimum value
  unsigned int signalMin = 1024;                         //maximum value because its 10 bit
  unsigned long startMillis = 0; 
  int sample;                        // Start of sample window
  for(unsigned int i = 0; i <peakToPeakAverages ; i++){
    startMillis = millis(); 
    while (millis() - startMillis < sampleWindow)        // reads value for 10ms
    {
        sample = analogRead(WIO_MIC);                       //get reading from microphone
        if (sample < 1024)                                  // toss out spurious readings
        {
          if (sample > signalMax)
          {
              signalMax = sample;                           // save just the max levels
          }
          else if (sample < signalMin)
          {
              signalMin = sample;                           // save just the min levels
          }
        }
    }
    peakToPeak += signalMax - signalMin;                    // max - min = peak-peak amplitude
   }
  peakToPeak /= peakToPeakAverages;
  decibels = map(peakToPeak,20,900,49.5,90);              // maps the value to a "decibel" (this value is not entirely accurate and is influenced by the microphone used and its relative sensitivity)
  // printing for debugging
  #ifdef DEBUGPRINTING
  Serial.println(decibels);
  #endif

// based on the decibel a loudness value is assigned and published to mqtt

  if (decibels <= Thresholds[0]) {
  (loudness = 1);
  client.publish("shusher/loudness", "1");
  }
  else if (decibels > Thresholds[0] && decibels <=  Thresholds[1]) {
   (loudness = 2);
   client.publish("shusher/loudness", "2");
  }
   else if (decibels >  Thresholds[1] && decibels <=  Thresholds[2]) {
   (loudness = 3);
   client.publish("shusher/loudness", "3");
  }
   else if (decibels >  Thresholds[2] && decibels <=  Thresholds[3]) {
   (loudness = 4);
   client.publish("shusher/loudness", "4");
  }
   else if (decibels >  Thresholds[3] && decibels <=  Thresholds[4]) {
   (loudness = 5);
   client.publish("shusher/loudness", "5");
   }
  else if (decibels >  Thresholds[4] && decibels <=  Thresholds[5]) {
   (loudness = 6);
   client.publish("shusher/loudness", "6");
  }   
  else if (decibels >  Thresholds[5] && decibels <=  Thresholds[6]) {
   (loudness = 7);
   client.publish("shusher/loudness", "7");
  }
  else if (decibels >  Thresholds[6] && decibels <=  Thresholds[7]) {
   (loudness = 8);
   client.publish("shusher/loudness", "8");
  }
  else if (decibels >  Thresholds[7] && decibels <=  Thresholds[8]) {
   (loudness = 9);
   client.publish("shusher/loudness", "9");
  }  
  else if (decibels >  Thresholds[8] ) {

   (loudness = 10);
   client.publish("shusher/loudness", "10");

   loudness = 10;
   loudnessMaxReachedCount++; // adds one to a count of how many times the max threshold was reached
  
  }
 
}

// Ultrasonic ranger functionality
int rangeFinder(){
 //Measues distance in CM with short delay to ensure fast and accurate measurements
 return RangeInCentimeters = (double)ultrasonic.MeasureInCentimeters();
}

bool isDataChanged() {
  int previousRange = 0;
  int previousDecibels = 0;
  bool previousLoudness = false;
  // Check if the data has changed
  if (currentrange != previousRange || decibels != previousDecibels || loudness != previousLoudness) {
    // Update the previous data with the current values
    previousRange = currentrange;
    previousDecibels = decibels;
    previousLoudness = loudness;
    return true;
  }

  return false;
}

void displayDataSPR() {
  spr.createSprite(250, 40);
  if (loudness >= 4) {
    spr.fillSprite(TFT_DARKGREY);
    spr.setTextColor(TFT_GREEN, TFT_DARKGREY);
    spr.setFreeFont(&FreeSansBoldOblique12pt7b);
    spr.setTextSize(1);
    spr.drawString("SHHHH...", 100, 30);
  } else {
    spr.fillSprite(TFT_DARKGREY);
    spr.setTextColor(TFT_GREEN, TFT_BLACK);
    spr.setFreeFont(&FreeSansBoldOblique12pt7b);
    spr.setTextSize(1);
    spr.drawString("SHUSHER", 100, 30);
    spr.setTextSize(1);
    spr.setTextColor(TFT_PURPLE, TFT_BLACK);
    spr.drawNumber(currentrange, 0, 0);
    spr.setTextColor(TFT_GREEN, TFT_BLACK);
    spr.drawString(" cm", 30, 0);
    spr.setTextColor(TFT_PURPLE, TFT_BLACK);
    spr.drawNumber(decibels, 120, 0);
    spr.setTextColor(TFT_GREEN, TFT_BLACK);
    spr.drawString(" dB", 210, 0);
  }
  
  
  spr.pushSprite(30, 190);
  spr.deleteSprite();
}
void displayDataTFT() {
    tft.fillScreen(TFT_BLACK);
    if (loudness >= 8) {
      tft.setTextColor(TFT_WHITE, TFT_DARKGREY);
      tft.setFreeFont(&FreeSansBoldOblique12pt7b);
      tft.setTextSize(2);
      tft.setCursor(0, 80); // Adjust the coordinates as needed
      tft.println("SHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH..."); // Display "SHHHH..." in the middle
  } else {
      tft.setTextColor(TFT_ORANGE, TFT_BLACK);
      tft.setFreeFont(&FreeSansBoldOblique12pt7b);
      tft.setTextSize(1);

    // Display "SHUSHER" in the middle
      tft.setCursor(100, 100); // Adjust the coordinates as needed
      tft.println("SHUSHER");

    // Display range in the bottom left
      tft.setTextColor(TFT_WHITE, TFT_BLACK);
      tft.setCursor(20, 200); // Adjust the coordinates as needed
      tft.print(currentrange);
      tft.setTextColor(TFT_GREEN, TFT_BLACK);
      tft.println(" cm");

    // Display decibels in the bottom right
      tft.setTextColor(TFT_WHITE, TFT_BLACK);
      tft.setCursor(200, 200); // Adjust the coordinates as needed
      tft.print(decibels);
      tft.setTextColor(TFT_RED, TFT_BLACK);
      tft.println(" dB");
    }
}




// RGB LED Stick Functions
void ledStartupTest(){    // Testing that all LEDs work(LightShow ;) )
  strip.begin();
  strip.setBrightness(brightnesslevellights);
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
  
   if(loudness >= 10){                 //when the stick is at maximum(meaning led number 10) the whole stick flashes red
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
  strip.clear();
  } 

void reconnect() {                                                  // method is taken fron the MQTT workshop
// Loop until we're reconnected
 while (!client.connected()) {
   Serial.println("Attempting MQTT connection...");
   // Create a random client ID
   String clientId = "WioTerminal";
   // Attempt to connect
   if (client.connect(clientId.c_str())) {
     Serial.println("connected");
     // Once connected, publish an announcement...
    } else {
     Serial.print("failed, rc=");
     Serial.print(client.state());
     Serial.println(" try again in 5 seconds");
     // Wait 5 seconds before retrying
     delay(5000);
    }
  }
}