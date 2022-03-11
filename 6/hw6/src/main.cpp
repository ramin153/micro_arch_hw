#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Wire.h>

byte BCD_DECIMAL(byte bcd);
void readTimeFromDS();
void timeDisplay();
void byteToAssci(byte number,char location[],int index);
void setTime();
void setCalander();
void printTime();
void writeOnDs();
void timeToBCD();
void setNewDataOnDS();
void sendRegisterAddress();
char time[] = "time:  :  AM";;
char calander[] = "date:  /  /20  ";
static bool setTimeFirstTime = true;
const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
byte second = 0,minute =36,hour = 9,day = 6,month = 3,year = 21;
byte second_bcd,minute_bcd,hour_bcd,day_bcd,month_bcd,year_bcd;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  Wire.begin();


}

void loop() {
  if(setTimeFirstTime){
    setNewDataOnDS();
    setTimeFirstTime = false;
  }

  sendRegisterAddress();              
  Wire.requestFrom(0x68, 7);
  while (Wire.available()>0)
  {
    timeDisplay();

  }
  


  delay(500);
}

byte BCD_DECIMAL(byte bcd){
  return (bcd >> 4) * 10 + (bcd & 0x0F);
}


byte DECIMAL_BCD(byte decimal){
  return  ((decimal / 10) << 4) + (decimal % 10);
}

void readTimeFromDS(){
  second = BCD_DECIMAL(Wire.read());
  minute = BCD_DECIMAL(Wire.read());
  hour = BCD_DECIMAL(Wire.read());

  Wire.read();

  day = BCD_DECIMAL(Wire.read());
  month = BCD_DECIMAL(Wire.read());
  year = BCD_DECIMAL(Wire.read());
}

void sendRegisterAddress(){
    Wire.beginTransmission(0x68);                 
    Wire.write(0);                                
    Wire.endTransmission(false);  
}
void timeDisplay(){


  readTimeFromDS();


  setTime();
  setCalander();

  printTime();
       
}

void byteToAssci(byte number,char location[],int index){
  location[index] = number % 10 + 48;
  location[index-1] = number / 10 + 48;
}

void setTime(){
  //byteToAssci(second,time,12);
  byteToAssci(minute,time,9);
  byteToAssci(hour,time,6);
  if (hour >= 12)
  {
    hour -= 12;
    time[10] = 'P';
  }else{
    time[10] = 'A';
  }
  
}


void setCalander(){
    byteToAssci(year,calander,14);
    byteToAssci(month,calander,9);
    byteToAssci(day,calander,6);
}


void   printTime(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(calander);                               
  lcd.setCursor(0, 1);
  lcd.print(time); 
}

void setNewDataOnDS(){
  timeToBCD();
  

  writeOnDs();

}

void timeToBCD(){
  second_bcd = DECIMAL_BCD(second);
  minute_bcd = DECIMAL_BCD(minute);
  hour_bcd = DECIMAL_BCD(hour);
  day_bcd = DECIMAL_BCD(day);
  month_bcd = DECIMAL_BCD(month);
  year_bcd = DECIMAL_BCD(year);
}

void writeOnDs(){
  Wire.beginTransmission(0x68); 

  Wire.write(0);                              
  Wire.write(second_bcd);                              
  Wire.write(minute_bcd);                         
  Wire.write(hour_bcd);                           
  Wire.write(1);                             
  Wire.write(day_bcd);                           
  Wire.write(month_bcd);                          
  Wire.write(year_bcd); 

  Wire.endTransmission();
    
}