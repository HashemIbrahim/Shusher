//--MQTT--------------------------------------------------------------
#include <rpcWiFi.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
//for simplicity, the internet settings are hardcoded for now but will be a part of a header file later in the git ignore.
const char* ssid = "ISAACHP";
const char* password = "isaac123";
const char* server = "192.168.137.1";
WiFiClient wioClient;
PubSubClient client(wioClient);
//---------------------------------------------------------------------
#define sampleWindow 10
#define peakToPeakAverages 10
#define DEBUG 
const int brightnesslevellights = 20;
int val;
int sample;
int loudness;
int loudnessMaxReachedCount;
int baseThreshold = 49;
 
const char* TOPIC_sub1 = "shusher/threshold";  
const char* TOPIC_sub2 = "shusher/lights/+";                             //delete
const char* TOPIC_pub_connection = "shusher";                              //delete

//---------------------------

uint32_t color = 0xFF00FF;


//------------------------------

// RGB LED Stick-------------------------------------------------------
#include <Adafruit_NeoPixel.h>
#define NUM_LEDS 10
#define DATA_PIN 0
Adafruit_NeoPixel strip(NUM_LEDS, DATA_PIN, NEO_RGB);

uint32_t ledStickColors[] = {0xFF0000, 0xFFFF00, 0x00FF00 };

//----------------------------------------------------------------------


void setup() {
  // put your setup code here, to run once:
  //--MQTT------------
 /* WiFi.begin(ssid, password);
  client.setServer(server,1883);
  client.setCallback(callback);
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected");
    return;
    
}
*/
  //-------------------
  #ifdef DEBUG
 Serial.begin(9600);
 #endif
 ledStartupTest();
 pinMode(WIO_MIC, INPUT);
 
}

void loop() {
  // put your main code here, to run repeatedly:
LoudnessSensorLoudValue();
setLedStick();
// this is a known issue where it has to connect to even display data
/*if (!client.connected()) {
    reconnect();
  }

client.loop();
*/

}

//Functions

//Loudness Sensor Thresholds
void LoudnessSensorLoudValue() {
  
    //this portion of code was taken from https://how2electronics.com/iot-decibelmeter-sound-sensor-esp8266/ it creates whats known as an 'envelope' to encompass the sound. This is necessary due to the way the analog value is recorded and how sound is a wave.
  
   float peakToPeak = 0;                                  // peak-to-peak level
   int Thresholds[] = {baseThreshold,baseThreshold+3,baseThreshold+6,baseThreshold+9,baseThreshold+12,baseThreshold+15,baseThreshold+18,baseThreshold+20,baseThreshold+22,baseThreshold+26};

   unsigned int signalMax = 0;                            //minimum value
   unsigned int signalMin = 1024;                         //maximum value because its 10 bit
 
                                                          // collect data for 50 mS
  unsigned long startMillis = 0;                   // Start of sample window
  for(unsigned int i = 0; i <peakToPeakAverages ; i++){
    startMillis = millis(); 
    while (millis() - startMillis < sampleWindow)
    {
        sample = analogRead(WIO_MIC);                    //get reading from microphone
        //Serial.print("0, ");                             // debugging printing
        //Serial.println(sample);
        delay(1);
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
   val = map(peakToPeak,20,900,49.5,90);              // maps the value to a "decibel" (this value is not entirely accurate and is influenced by the microphone used and its relative sensitivity)
   //Serial.println(peakToPeak);
	/////

 //Serial.println(val);

// based on the decibel a loudness value is assigned

  if (val <= Thresholds[0]) {
  (loudness = 1);
  client.publish("shusher/loudness", "1");
  }
  else if (val > Thresholds[0] && val <=  Thresholds[1]) {
   (loudness = 2);
   client.publish("shusher/loudness", "2");
  }
   else if (val >  Thresholds[1] && val <=  Thresholds[2]) {
   (loudness = 3);
   client.publish("shusher/loudness", "3");
  }
   else if (val >  Thresholds[2] && val <=  Thresholds[3]) {
   (loudness = 4);
   client.publish("shusher/loudness", "4");
  }
   else if (val >  Thresholds[3] && val <=  Thresholds[4]) {
   (loudness = 5);
   client.publish("shusher/loudness", "5");
   }
 else if (val >  Thresholds[4] && val <=  Thresholds[5]) {
   (loudness = 6);
   client.publish("shusher/loudness", "6");
  }   
   else if (val >  Thresholds[5] && val <=  Thresholds[6]) {
   (loudness = 7);
   client.publish("shusher/loudness", "7");
  }
   else if (val >  Thresholds[6] && val <=  Thresholds[7]) {
   (loudness = 8);
   client.publish("shusher/loudness", "8");
  }
 else if (val >  Thresholds[7] && val <=  Thresholds[8]) {
   (loudness = 9);
   client.publish("shusher/loudness", "9");
  }  
   else if (val >  Thresholds[8] ) {

   (loudness = 10);
   client.publish("shusher/loudness", "10");

   loudness = 10;
   loudnessMaxReachedCount++; // adds one to a count of how many times the max threshold was reached
  
  }
 
  }
// RGB LED Stick Functions
void ledStartupTest(){    // Testing that all LEDs work(LightShow ;) )
  strip.begin();
  strip.setBrightness(brightnesslevellights);
  strip.clear(); 
  for(int i= 0; i<=NUM_LEDS;i++){
    delay(100);
    if(i<3){
    strip.setPixelColor(i, ledStickColors[0]);       //setting the color of led number 1-3 to green
    
  }
    else if(i<7){
    strip.setPixelColor(i,ledStickColors[1]);    //setting the color of led number 4-7 yellow
  }

    else if(i<10) {
    strip.setPixelColor(i,ledStickColors[2]);       //Setting the color of LED number 8-9 red
  }
    else if(i== 10){                      //the whole Stick flashes red when the last LED is reached.
       for(int j = 0; j < 5; j++){
        for(int k = 0; k < NUM_LEDS; k++){
          strip.setPixelColor(k, ledStickColors[2]);
        }
        strip.show();
        delay(150);
        strip.clear();
        strip.show();
        delay(150);
        }
    }
    delay(150);
    strip.show();
  }
  strip.clear();
  

}



void setLedStick(){                 //Activating the LEDs dependent on the loudness which is determined by the thresholds set at the top. 
  for(int i = 0; i < loudness; i++){
      if(i <= 2){
      strip.setPixelColor(i, ledStickColors[0]);
      }
      if(i > 2 && i <7){
      strip.setPixelColor(i,ledStickColors[1]);
      }
      if(i>=7){
      strip.setPixelColor(i, ledStickColors[2]);
      }
      if (i >= 9){ 
      for(int j = 0; j < 3; j++){
        for(int k = 0; k < NUM_LEDS; k++){
          strip.setPixelColor(k, ledStickColors[2]);
        }
        strip.show();
        delay(150);
        strip.clear();
        strip.show();
        delay(150);
        }
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
      // ... and resubscribe
      client.subscribe(TOPIC_sub2);
      client.subscribe(TOPIC_sub1);
      Serial.print("Subcribed to: ");
      Serial.println(TOPIC_sub1);
      Serial.println(TOPIC_sub2);
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

//--MQTT--------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
  //This function is called when a message over mqtt is recieved 
  void callback(char* topic, byte* payload, unsigned int length){

    Serial.print("Message recieved on topic ");
    Serial.println(topic);

    String message;
    for(int i = 0; i<length; i++){   //iterates through the payload converting it from byte to a string
    message += (char)payload[i];
    }
    Serial.print("Message payload: ");
    Serial.println(message);

  if(strcmp(topic, "shusher/threshold") == 0){ //checks if the topic is "shusher/threshold"
    changeThreshold(message);               // if it is, call the changeThreshold cunction
    Serial.print(message);
  }
  else if (strstr(topic, "shusher/lights/") != NULL){  // check if the topic contains "shusher/lights/". strstr checks if the given string is a substring of the topic.
    char* section = topic + strlen("shusher/colors/"); // extracts the section from the topic string by moving the pointer to the first letter of "section"
    changeLightsTheme(message,section);       //call the changeLightsTheme with the message payload and section as arguments
  }
}

//this function changes the theme of the LED lights based on the message payload and section
void changeLightsTheme(String message, char* section){
    char* endptr;                                             // lines 317,318, 310,309 is recomended by chatgpt, i think strtoul is a good function to use to convert a hexadecimal string becase of the error handling,
                                                              // strlen was a smart way to extract the section from the topic because the whole topic is not needed in the changeLightsTheme
    uint32_t hexValue = strtoul(message.c_str(), &endptr, 16);// converts the message payload from a hexadecimal string to uint32_t to be able to set the lights with message  
    Serial.print("Converted value: ");
    Serial.println(hexValue);
    if(strcmp(section, "section1") == 0){ //checks if section is "section1"
      ledStickColors[0] = hexValue; // if it is, set the color of section 1 to the converted value 
    }
    else if(strcmp(section, "section2") == 0){
      ledStickColors[1] = hexValue;
    }
    else if(strcmp(section, "section3") == 0){
      ledStickColors[2] = hexValue;
    }
    
}

//this function hcanges the baseThreshold based on the message payload.
void changeThreshold(String message){
  if (message.equals("High")){        // checks if the message payload equals "High" 
      Serial.println("baseThreshold is now set to 63"); 
      baseThreshold = 63;               // if it does, change the baseThreshold to 63.
    }
    else if (message.equals("Medium")){ //checks if the message payload equals "Medium"
      Serial.println("baseThreshold is now set to 56");
      baseThreshold = 56;             // if it does, change the baseThreshold to 56.
    }
    else if(message.equals("Low")){     //checks if the message payload equals "Low"
      Serial.println("baseThreshold is now set to 49");
      baseThreshold = 49;             //if it does, change the baseThreshold to 56.

    }
    Serial.println(message);
}




//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------