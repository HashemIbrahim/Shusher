#include <TFT_eSPI.h> 
TFT_eSPI tft;
TFT_eSprite spr = TFT_eSprite(&tft);

int val;
void setup() {
  tft.begin(); 
  tft.setRotation(1); 
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(3);
}

void loop() {
  readAnalog();
  displayValue();
  delay(100);
}

// Function to read the values from the loudness sensor
void readAnalog() {
  analogRead(0);
  delay(10);
  val = analogRead(0);
}

// Function to display the values
void displayValue() {
  tft.setCursor(1,0);

  spr.fillSprite(TFT_WHITE);
  spr.createSprite(320,240);
  spr.fillSprite(TFT_WHITE);
  spr.setTextColor(TFT_BLACK,TFT_WHITE);
  spr.setFreeFont(&FreeSansBoldOblique12pt7b);
  spr.drawString("Value: ",0,0);
  spr.drawNumber(val,70,0);
  spr.pushSprite(0,0);
  spr.deleteSprite();
}