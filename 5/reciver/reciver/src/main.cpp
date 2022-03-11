#include <Arduino.h>
#include <LiquidCrystal.h>
#include <stdlib.h>

void setNoise();
void checkTheGuess();
void printGuessAnditsState(int state);
void createMessageForLcd(int state);
#define NOISE_PORT A1


static int noise ;
static int guess;
static bool isNoiseGot = false;
static const int MORE = 2, LESS = 0,RIGHT = 1;
static  int guideTheGuess = 4;

static char lcd_message[12];
static char help_lcd_message[7];



const int rs = 12, en = 11, d4 = 7, d5 = 6, d6 = 5, d7 = 4;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


void setup() {
  	Serial.begin(9600);
    pinMode(NOISE_PORT, INPUT);
    lcd.begin(16, 2);

}

void loop() {

  if(guideTheGuess != RIGHT){
    setNoise();
    checkTheGuess();
  }
  
}

void checkTheGuess(){
  if (Serial.available() > 0)
  {
    
    guess =  Serial.parseInt();

    if (guess == noise)
      guideTheGuess = RIGHT;
    else if (guess < noise)
      guideTheGuess = MORE;
    else
      guideTheGuess = LESS;

    printGuessAnditsState(guideTheGuess);
    Serial.println(guideTheGuess);
  }
}  

void printGuessAnditsState(int state){
  createMessageForLcd(state);
  lcd.clear();
  lcd.print(lcd_message);
}

void createMessageForLcd(int state){
  sprintf(lcd_message,"%03d",guess);
  lcd_message[3] = ' ';
  lcd_message[4] = 0;
  if(state == RIGHT){
    strcpy(help_lcd_message ,"right");
  }else if (state == MORE){
    strcpy(help_lcd_message ,"more");
  }else{
    strcpy(help_lcd_message , "less");
  }
  strcat(lcd_message,help_lcd_message);
}

void setNoise(){
    if(!isNoiseGot)
  {
      noise = map(analogRead(NOISE_PORT),0,1023,0,100);
      isNoiseGot = true;

  }
}



