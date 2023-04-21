int val;
int loudness;
int Thresholds[] = {20,40,60,80,100,120,140,160,180,200};

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
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------





//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------