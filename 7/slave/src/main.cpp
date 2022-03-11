#include <Arduino.h>
#include<SPI.h>
#include <EEPROM.h>

#define SEND_DATA_FLAG 1
#define GET_DATA_FLAG 2

void getFirstInput();
void clearAllValue();
void readData();
void readAddress();

static bool  isfirstInput = true,
            isPartOneAddresGet = false,
            isAdressGet = false,
            isDataReady = false;;
int address = 0 ;
int dateMode = 0;
byte value = 0;
byte myEeprom[256];
byte returnValue;


void setup() {
  Serial.begin(9600);
  
                
  pinMode(MISO,OUTPUT);                  
  


  SPCR |= _BV(SPE);
  SPI.attachInterrupt();
}

void loop() {

}

ISR (SPI_STC_vect) 
{ 
  if(isfirstInput){
    getFirstInput();
    
    return;
  }
  
  if(dateMode == GET_DATA_FLAG){
    readAddress();
    if(isDataReady){
      SPDR = returnValue;
      clearAllValue();
    }
    else if(isAdressGet){
    returnValue =EEPROM.read(address);
    SPDR = returnValue;   
    
    isDataReady = true;
    }
    
    return;
  }

  if(isAdressGet){
    readData();
    clearAllValue();
  }else{
    readAddress();
  }
  
  
}

void getFirstInput(){
  dateMode = SPDR;
  isfirstInput = false;
}

void readAddress(){

  if(isPartOneAddresGet){
    address = (SPDR << 8) + address;
    isAdressGet = true;
    
    isPartOneAddresGet = false;
  }else{
    address = SPDR;
    isPartOneAddresGet = true;
  }
}

void readData(){
  value = SPDR;
  
  EEPROM.update(address,value);
}

void   clearAllValue(){
  isfirstInput = true,
  isPartOneAddresGet = false,
  isAdressGet = false;
  address = 0 ;
  dateMode = 0;
  value = 0;
  isDataReady = false;
}