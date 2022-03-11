#include <Arduino.h>
#include <stdlib.h>

void binaryGuess();
void firstGuess();

static int changeValue = 25;
static int guess = 50;

static bool isEnd = false;
static bool isItThefirstGuess = true;


static const int MORE = 2, LESS = 0,RIGHT = 1;


void setup() {
    	Serial.begin(9600);
}

void loop() {
    if (!isEnd)
    {
      firstGuess();
      binaryGuess();
    }
    
    

}

void firstGuess(){
  if(isItThefirstGuess ) {
    delay(100);
    Serial.println(guess);
    isItThefirstGuess = false;

  }

  
  
}

void binaryGuess(){
  if (Serial.available() > 0)
  {
    int resultOfGuess =  Serial.parseInt();


    if (resultOfGuess == RIGHT)
    {
      isEnd = true;
      return;
    }else if(resultOfGuess == MORE){
      guess += changeValue + 1;
    }else{
      guess -= changeValue + 1;
    }

    changeValue = changeValue/2;

    delay(300);
    Serial.println(guess);
  }
}