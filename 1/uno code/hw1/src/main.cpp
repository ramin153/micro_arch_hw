#include <Arduino.h>

#define INPUT_LINE A0
#define LED_1 A1
#define LED_2 A2
#define LED_3 A3
#define LED_4 A4
bool led1,led2,led3,led4;

void selectLed(int voltagePercentage);

void setup() {
  pinMode(INPUT_LINE, INPUT);
  Serial.begin(9600);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(LED_4, OUTPUT);

}

void loop() {
    
    int voltagePercentage = map(analogRead(INPUT_LINE),0,1023,0,100);// between 0 to 100 the value will be returend

    selectLed(voltagePercentage);
    
    
    digitalWrite(LED_1, led1);
    digitalWrite(LED_2, led2);
    digitalWrite(LED_3, led3);
    digitalWrite(LED_4, led4);

    


}

void selectLed(int voltagePercentage){
  if (voltagePercentage <= 25)
    {
      led1 = false;
      led2 = false;
      led3 = false;
      led4 = false;
    }else if (voltagePercentage <= 50)
    {
      led1 = true;
      led2 = false;
      led3 = false;
      led4 = false;
    }else if (voltagePercentage <= 75)
    {
      led1 = true;
      led2 = true;
      led3 = false;
      led4 = false;
    }else if (voltagePercentage <= 90)
    {
      led1 = true;
      led2 = true;
      led3 = true;
      led4 = false;
    }else{
      led1 = true;
      led2 = true;
      led3 = true;
      led4 = true;
    }
}