int val;
int loudness;
int Thresholds[] = {20,40,60,80,100,120,140,160,180,200};
// 
#include <Adafruit_NeoPixel.h>
#define NUM_LEDS 10
#define DATA_PIN 0

String message;

void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
LoudnessSensorLoudValue();
Messagecalculator();
Serial.println(message);

	delay(200);

}
//Functions

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
void ledStartupTest(){
  strip.begin();
  strip.clear();
  for(int i= 0; i<NUM_LEDS;i++){
    delay(200);
    if(i<3){
    strip.setPixelColor(i, 255,0,0);
  }
    else if(i<7){
    strip.setPixelColor(i,255,255,0);
  }

    else if(i<10) {
    strip.setPixelColor(i,0,255,0);
  }
    else if(i==(NUM_LEDS -1)){
      for(int i = 0; < 3; i++){
        delay(200);
        for(int j = 0; j<NUM_LEDS; i++ ){
        strip.setPixelColor(i,0,255,0) 
        strip.show();     
      }
      delay(300);
      strip.clear();
      strip.show();
    }
  }
  
  
  
  strip.show();  
}
for (int i = 0; i<NUM_LEDS; i++) {
  delay(300);
  strip.setPixelColor(i, 0,0,0);
}
}
void setLedStick(){
  
  if(loudness >= 1)
    strip.setPixelColor(0, 255,0,0);
  if(loudness >=2)
    strip.setPixelColor(1,255,0,0);
  if(loudness >=3)
    strip.setPixelColor(2,255,0,0);
  if (loudness >=4)
    strip.setPixelColor(3,255,255,0);
  if (loudness >=5)
    strip.setPixelColor(4,255,255,0); 
  if (loudness >=6)
    strip.setPixelColor(5,255,255,0);
  if (loudness >=7)
    strip.setPixelColor(6,255,255,0);
  if (loudness >=8)
    strip.setPixelColor(7,0,255,0);
  if (loudness >=9)
    strip.setPixelColor(8,0,255,0);
   
   
    while(loudness >= 10){
      strip.clear();
      delay(150);
      for(int i = 0; i<NUM_LEDS; i++){
        strip.setPixelColor(i,0,255,0);       
      }
  }
  } 


//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------





//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------