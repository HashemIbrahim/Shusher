//--MQTT--------------------------------------------------------------
#include <rpcWiFi.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "Ultrasonic.h"
#include "WifiSetup.h"
WiFiClient wioClient;
PubSubClient client(wioClient);
//---------------------------------------------------------------------
//--Setting constants--------------------------------------------------------------
#define sampleWindow 10
#define peakToPeakAverages 10
#define brightnesslevellights 20
int const ranges[] = { 20, 40, 80, 120, 160 };
float const sensvalues[] = { 1, 2, 2.5, 3, 3.5, 4};
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
uint32_t thematicColors[] = {TFT_DARKGREEN, TFT_BLACK, TFT_PURPLE};
//--Setting variables--------------------------------------------------------------
int loudness;
int loudnessMaxReachedCount;
float Sens = 3;
float baseThreshold = 49;
long RangeInCentimeters;
float Thresholds[10];
int currentRange;
float decibels;

// RGB LED Stick-------------------------------------------------------
#include <Adafruit_NeoPixel.h>
#define NUM_LEDS 10
#define DATA_PIN 0
Adafruit_NeoPixel strip(NUM_LEDS, DATA_PIN, NEO_RGB);

uint32_t ledStickColors[] = { 0xFF0000, 0xFFFF00, 0x00FF00 };

//----------------------------------------------------------------------
const char* TOPIC_sub1 = "shusher/threshold";
const char* TOPIC_sub2 = "shusher/lights/+";
const char* TOPIC_pub_connection = "shusher";

void setup() {
  setupWIFI();
  setupSerial();
  ledStartupTest();
  setupMic();
  setupScreen();
}

void loop() {
  ThresholdCalculator();
  LoudnessSensorLoudValue();
  displayDataSPRMika();
  setLedStick();
  client.loop();
  if (!client.connected()) {
    reconnect();
  }
}
//Functions
void setupScreen() {
  tft.begin();
  tft.setRotation(3);
  resetScreen();
}
void setupWIFI() {  //connects to the wifi
  WiFi.begin(ssid, password);
  client.setServer(server, 1883);
  client.setCallback(callback);
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected");
  }
}

void setupSerial() {  //starts the serial for input
  Serial.begin(115200);
}

void setupMic() {  //sets what to use as the mic
  pinMode(WIO_MIC, INPUT);
}
void ThresholdCalculator() {
  currentRange = rangeFinder();  //gets the range from the ranger right now
  //---MQTT-------------------------------------------------------------------------------------------------------
  const char* currentDistance = String(currentRange).c_str();  // converts the integer into a const char*, this is necessary for the publish method to work
  client.publish("shusher/distance", currentDistance);         //published the currentDistance via MQTT with the topic "shusher/distance"
  //-----------------------------------------------------------------------------------------------------------
  //calulates what the sensitivity should be based on that range
  if (currentRange < ranges[0]) {
    Sens = sensvalues[5];
  } else if (currentRange >= ranges[0] && currentRange < ranges[1]) {
    Sens = sensvalues[4];
  } else if ((currentRange >= ranges[1] && currentRange < ranges[2])) {
    Sens = sensvalues[3];
  } else if (currentRange >= ranges[2] && currentRange < ranges[3]) {
    Sens = sensvalues[2];
  } else if (currentRange >= ranges[3] && currentRange < ranges[4]) {
    Sens = sensvalues[1];
  } else if (currentRange >= ranges[4]) {
    Sens = sensvalues[0];
  }

  //sets the threshold values based on this sensitivity
  for (int i = 0; i < 10; i++) {
    Thresholds[i] = baseThreshold + (i * Sens);
  }
}
//Loudness Sensor Thresholds
void LoudnessSensorLoudValue() {

  //this portion of code was taken from https://how2electronics.com/iot-decibelmeter-sound-sensor-esp8266/ it creates whats known as an 'envelope' to encompass the sound. This is necessary due to the way the analog value is recorded and how sound is a wave.
  float peakToPeak = 0;           // peak-to-peak level
  unsigned int signalMax = 0;     //minimum value
  unsigned int signalMin = 1024;  //maximum value because its 10 bit
  unsigned long startMillis = 0;
  int sample;  // Start of sample window
  for (unsigned int i = 0; i < peakToPeakAverages; i++) {
    startMillis = millis();
    while (millis() - startMillis < sampleWindow)  // reads value for 10ms
    {
      sample = analogRead(WIO_MIC);  //get reading from microphone
      if (sample < 1024)             // toss out spurious readings
      {
        if (sample > signalMax) {
          signalMax = sample;  // save just the max levels
        } else if (sample < signalMin) {
          signalMin = sample;  // save just the min levels
        }
      }
    }
    peakToPeak += signalMax - signalMin;  // max - min = peak-peak amplitude
  }
  peakToPeak /= peakToPeakAverages;
  decibels = map(peakToPeak, 20, 900, 49.5, 90);  // maps the value to a "decibel" (this value is not entirely accurate and is influenced by the microphone used and its relative sensitivity)


  // based on the decibel a loudness value is assigned and published to mqtt
  if (decibels <= Thresholds[0]) {
    (loudness = 1);
    client.publish("shusher/loudness", "1");
  } else if (decibels > Thresholds[0] && decibels <= Thresholds[1]) {
    (loudness = 2);
    client.publish("shusher/loudness", "2");
  } else if (decibels > Thresholds[1] && decibels <= Thresholds[2]) {
    (loudness = 3);
    client.publish("shusher/loudness", "3");
  } else if (decibels > Thresholds[2] && decibels <= Thresholds[3]) {
    (loudness = 4);
    client.publish("shusher/loudness", "4");
  } else if (decibels > Thresholds[3] && decibels <= Thresholds[4]) {
    (loudness = 5);
    client.publish("shusher/loudness", "5");
  } else if (decibels > Thresholds[4] && decibels <= Thresholds[5]) {
    (loudness = 6);
    client.publish("shusher/loudness", "6");
  } else if (decibels > Thresholds[5] && decibels <= Thresholds[6]) {
    (loudness = 7);
    client.publish("shusher/loudness", "7");
  } else if (decibels > Thresholds[6] && decibels <= Thresholds[7]) {
    (loudness = 8);
    client.publish("shusher/loudness", "8");
  } else if (decibels > Thresholds[7] && decibels <= Thresholds[8]) {
    (loudness = 9);
    client.publish("shusher/loudness", "9");
  } else if (decibels > Thresholds[8]) {

    (loudness = 10);
    client.publish("shusher/loudness", "10");

    loudness = 10;
    loudnessMaxReachedCount++;  // adds one to a count of how many times the max threshold was reached
    const char* counter = String(loudnessMaxReachedCount).c_str();
    client.publish("shusher/loudness/counter", counter);
  }
}

// Ultrasonic ranger functionality
int rangeFinder() {
  //Measues distance in CM with short delay to ensure fast and accurate measurements
  return RangeInCentimeters = (double)ultrasonic.MeasureInCentimeters();
}

void displayDataSPRMika() {
  //Data Display of Ultrasonic Ranger
  spr.createSprite(165, 40);
  spr.fillSprite(thematicColors[0]);
  spr.setTextColor(TFT_WHITE, thematicColors[0]);
  spr.setFreeFont(&FreeSansBold9pt7b);
  spr.drawString("cm", 120, 20);
  spr.setFreeFont(&FreeSansBold18pt7b);
  spr.drawNumber(currentRange, 50, 10);
  spr.pushSprite(30, 30);

  //Data Display of Loudness
  spr.fillSprite(thematicColors[1]);
  spr.setTextColor(TFT_WHITE, thematicColors[1]);
  spr.setFreeFont(&FreeSansBold9pt7b);
  spr.drawString("Vol", 120, 20);
  spr.setFreeFont(&FreeSansBold18pt7b);
  spr.drawNumber(loudness, 50, 10);
  spr.pushSprite(125, 170);

  //Data display of boundary breaks
  spr.fillSprite(thematicColors[2]);
  spr.setTextColor(TFT_WHITE, thematicColors[2]);
  spr.drawNumber(loudnessMaxReachedCount, 110, 0);
  spr.pushSprite(-100, 200);
}

void resetScreen() {
  tft.fillTriangle(0, 0, 0, 240, 320, 0, thematicColors[1]);
  tft.fillTriangle(320, 0, 0, 240, 320, 240, thematicColors[0]);
  tft.fillCircle(0, 240, 100, thematicColors[2]);

  //Data Table Setting
  tft.fillRect(25, 25, 175, 50, thematicColors[2]);
  tft.fillRect(120, 165, 175, 50, thematicColors[2]);

  //Max Boundary Breaks Setting
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.setTextSize(0.4);
  tft.setTextColor(TFT_WHITE, thematicColors[2]);
  tft.drawString("Breaks:", 5, 170);

  //Logo display
  tft.setFreeFont(&FreeSansBold12pt7b);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.drawString("Shusher", 65, 100);
}

//Function to flash a SHHHH across the screen
void flashSHHH() {
  tft.fillScreen(TFT_RED);
  tft.setTextColor(TFT_BLACK, TFT_RED);
  tft.setTextSize(3);
  tft.drawString("SHHHHHHHHHHHHH", 0, 100);
  delay(50);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.drawString("SHHHHHHHHHHHHH", 0, 100);
  delay(50);
}

// RGB LED Stick Functions
void ledStartupTest() {  // Testing that all LEDs work(LightShow ;) )
  strip.begin();
  strip.setBrightness(brightnesslevellights);
  strip.clear();
  for (int i = 0; i <= NUM_LEDS; i++) {
    delay(100);
    if (i < 3) {
      strip.setPixelColor(i, ledStickColors[0]);  //setting the color of led number 1-3 to green

    } else if (i < 7) {
      strip.setPixelColor(i, ledStickColors[1]);  //setting the color of led number 4-7 yellow
    }

    else if (i < 10) {
      strip.setPixelColor(i, ledStickColors[2]);  //Setting the color of LED number 8-9 red
    } else if (i == 10) {                         //the whole Stick flashes red when the last LED is reached.
      for (int j = 0; j < 5; j++) {
        for (int k = 0; k < NUM_LEDS; k++) {
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

void setLedStick() {  //Activating the LEDs dependent on the loudness which is determined by the thresholds set at the top.
  for (int i = 0; i < loudness; i++) {
    if (i <= 2) {
      strip.setPixelColor(i, ledStickColors[0]);
    }
    if (i > 2 && i < 7) {
      strip.setPixelColor(i, ledStickColors[1]);
    }
    if (i >= 7) {
      strip.setPixelColor(i, ledStickColors[2]);
    }
    if (i >= 9) {
      for (int j = 0; j < 3; j++) {
        for (int k = 0; k < NUM_LEDS; k++) {
          strip.setPixelColor(k, ledStickColors[2]);
        }
        strip.show();
        flashSHHH();
        delay(100);
        strip.clear();
        strip.show();
        delay(100);
      }
      resetScreen();
    }
  }
  strip.show();
  strip.clear();
}

void reconnect() {  // method is taken fron the MQTT workshop
  // Loop until we're reconnected
  setupWIFI();
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
void callback(char* topic, byte* payload, unsigned int length) {

  String message;
  for (int i = 0; i < length; i++) {  //iterates through the payload converting it from byte to a string
    message += (char)payload[i];
  }

  if (strcmp(topic, "shusher/threshold") == 0) {  //checks if the topic is "shusher/threshold"
    changeThreshold(message);                     // if it is, call the changeThreshold cunction
  } else if (strstr(topic, "shusher/lights/") != NULL) {  // check if the topic contains "shusher/lights/". strstr checks if the given string is a substring of the topic.
    char* section = topic + strlen("shusher/colors/");    // extracts the section from the topic string by moving the pointer to the first letter of "section"
    changeLightsTheme(message, section);                  //call the changeLightsTheme with the message payload and section as arguments
  }
}

//this function changes the theme of the LED lights based on the message payload and section
void changeLightsTheme(String message, char* section) {
  char* endptr;                                               // lines 317,318, 310,309 is recomended by chatgpt, i think strtoul is a good function to use to convert a hexadecimal string becase of the error handling,
                                                              // strlen was a smart way to extract the section from the topic because the whole topic is not needed in the changeLightsTheme
  uint32_t hexValue = strtoul(message.c_str(), &endptr, 16);  // converts the message payload from a hexadecimal string to uint32_t to be able to set the lights with message
  if (strcmp(section, "section1") == 0) {  //checks if section is "section1"
    ledStickColors[0] = hexValue;          // if it is, set the color of section 1 to the converted value
  } else if (strcmp(section, "section2") == 0) {
    ledStickColors[1] = hexValue;
  } else if (strcmp(section, "section3") == 0) {
    ledStickColors[2] = hexValue;
  }
  
  //Checks what theme it is set to
  if(strcmp(section, "theme") == 0){
    if(message == "defaultTheme"){
      thematicColors[0] = TFT_DARKGREEN;
      thematicColors[1] = TFT_BLACK;
      thematicColors[2] = TFT_PURPLE;
    }else if(message == "darkTheme"){
      thematicColors[0] = TFT_BLUE;
      thematicColors[1] = TFT_RED;
      thematicColors[2] = TFT_PURPLE;
    }else if(message == "partyTheme"){
      thematicColors[0] = TFT_CYAN;
      thematicColors[1] = TFT_PINK;
      thematicColors[2] = TFT_YELLOW;
    }resetScreen();
  }
}

//this function hcanges the baseThreshold based on the message payload.
void changeThreshold(String message) {
  if (message.equals("High")) {  // checks if the message payload equals "High"
    Serial.println("baseThreshold is now set to 63");
    baseThreshold = 63;                   // if it does, change the baseThreshold to 63.
  } else if (message.equals("Medium")) {  //checks if the message payload equals "Medium"
    Serial.println("baseThreshold is now set to 56");
    baseThreshold = 56;                // if it does, change the baseThreshold to 56.
  } else if (message.equals("Low")) {  //checks if the message payload equals "Low"
    Serial.println("baseThreshold is now set to 49");
    baseThreshold = 49;  //if it does, change the baseThreshold to 56.
  }
}
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------