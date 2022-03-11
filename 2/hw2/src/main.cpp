#include <Arduino.h>
#define INPUT_LINE A0
#define OUTPUT_LINE 9
int value = 0;
void timeOn(int percentTimeOn);
void setup() {
  pinMode(INPUT_LINE, INPUT);
  Serial.begin(9600);
  pinMode(OUTPUT_LINE, OUTPUT);

}

void loop() {

  value = map(analogRead(INPUT_LINE),0,1023,0,255) ;
  analogWrite(OUTPUT_LINE,value);

  
  // int voltagePercentage = map(analogRead(INPUT_LINE),0,1023,0,100);
  // timeOn(voltagePercentage);

}

void timeOn(int percentTimeOn){

  digitalWrite(OUTPUT_LINE, true);
  delay(percentTimeOn);
  digitalWrite(OUTPUT_LINE, false);
  delay(100-percentTimeOn);
}