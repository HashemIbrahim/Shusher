int val;
const int sampleWindow = 50;
int sample;
int loudness;
int loudnessMaxReachedCount;
int constexpr baseThreshold = 49;
int constexpr Thresholds[] = {baseThreshold,baseThreshold+2,baseThreshold+4,baseThreshold+5,baseThreshold+6,baseThreshold+8,baseThreshold+10,baseThreshold+12,baseThreshold+18,baseThreshold+26};


// RGB LED Stick-------------------------------------------------------
#include <Adafruit_NeoPixel.h>
#define NUM_LEDS 10
#define DATA_PIN 0
Adafruit_NeoPixel strip(NUM_LEDS, DATA_PIN, NEO_RGB);
//----------------------------------------------------------------------


void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600);
 ledStartupTest();
 pinMode(WIO_MIC, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
LoudnessSensorLoudValue();
setLedStick();



}
//Functions

//Loudness Sensor Thresholds

  void LoudnessSensorLoudValue() {
    //this portion of code was taken from https://how2electronics.com/iot-decibelmeter-sound-sensor-esp8266/ it creates whats known as an 'envelope' to encompass the sound. This is necessary due to the way the analog value is recorded and how sound is a wave.
    unsigned long startMillis= millis();                   // Start of sample window
   float peakToPeak = 0;                                  // peak-to-peak level
 
   unsigned int signalMax = 0;                            //minimum value
   unsigned int signalMin = 1024;                         //maximum value
 
                                                          // collect data for 50 mS
   while (millis() - startMillis < sampleWindow)
   {
      sample = analogRead(WIO_MIC);                    //get reading from microphone
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
 
   peakToPeak = signalMax - signalMin;                    // max - min = peak-peak amplitude
   int val = map(peakToPeak,20,900,49.5,90);              // maps the value to a decibel
	/////



// based on the decibel a loudness value is assigned
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
   loudness = 10;
   loudnessMaxReachedCount++; // adds one to a count of how many times the max threshold was reached
   
  }
 
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
   /* else if(i== 10){                      //the whole Stick flashes red when the last LED is reached.
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
  */

}
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


//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------





//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------