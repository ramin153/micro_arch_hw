#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Keypad.h>
#include<SPI.h>                        
#define SEND_DATA_FLAG 1
#define GET_DATA_FLAG 2

void addNumer(char userCharInput);
void readInput();
void getDataFromSlave();
void setDataInSalve();
void printMessage(char *text);
void sendData();
void getData();
int  getDataSPI(int address);
void putDataSPI(int address,byte value);
byte sendAdress(int address);
const int rs = 9, en = 8, d4 = 7, d5 = 6, d6 = 5, d7 = 4;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
//keypad
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
{'1','2','3'},
{'4','5','6'},
{'7','8','9'},
{'*','0','#'}
};
byte rowPins[ROWS] = {A3, A2, A1, A0}; //connect to the row pinouts of the kpd
byte colPins[COLS] = {A4, A5, 3}; //connect to the column pinouts of the kpd

Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


static int input_data = 0;
static int input_number = 0 ;
static char input_number_text[32];
static bool isDataRead = false;

void setup() {
    Serial.begin(9600);
    lcd.begin(16, 2);

    SPI.begin();
    digitalWrite(SS,HIGH);                  
    

}

void loop() {
  readInput();
}


void readInput(){
    if (kpd.getKeys())
    {
        for (int i=0; i<LIST_MAX; i++)   
        {
            if ( kpd.key[i].stateChanged && (PRESSED == kpd.key[i].kstate) )   
            {
                if (kpd.key[i].kchar == '*')
                {
                    setDataInSalve();
                }else if(kpd.key[i].kchar == '#'){
                    getDataFromSlave();
                }else{
                    addNumer(kpd.key[i].kchar);
                    sprintf(input_number_text, "%d", input_number);
                    printMessage(input_number_text);
                }
                
               
            }
        }
    }
}

void addNumer(char userCharInput){
    input_number = input_number*10 + (userCharInput - '0');
}

void getDataFromSlave(){
  if(input_number >= 0 && input_number <= 512){
    input_number =  getDataSPI(input_number);
    sprintf(input_number_text, "%d", input_number);
    printMessage(input_number_text);
  }else{
    printMessage("not in range!!");
  }
  isDataRead = false;
  input_number = 0;
  input_data = 0;
}

int  getDataSPI(int address){
  digitalWrite(SS, LOW); 
  
  SPI.transfer((byte) GET_DATA_FLAG); 
  delay(25);
  byte value = sendAdress(address);

  delay(25);
  value = SPI.transfer(-1);
 
  digitalWrite(SS, HIGH); 
  return (int)value;
}  


void setDataInSalve(){
  if(isDataRead){
    sendData();

  }else{
    getData();

  }
}

   


void sendData(){
  if( input_number >= 0 && input_number <= 255  ){
      putDataSPI(input_data,input_number);
      printMessage("0");
    }else {
      printMessage("not in range!!");
      }
    isDataRead = false;
    input_number = 0;
    input_data = 0;

}


void getData(){
  if( input_number >= 0 && input_number <= 512  ){
      isDataRead = true;
      input_data = input_number;
      printMessage("0");

    }else{
      printMessage("not in range!!");
    }
    input_number = 0;

}

void putDataSPI(int address,byte value){

  digitalWrite(SS, LOW); 
  SPI.transfer(SEND_DATA_FLAG);
  delay(25);
  sendAdress(address); 
  delay(25);
  SPI.transfer(value); 
  delay(25);
  digitalWrite(SS, HIGH); 

}     




void printMessage(char *text){
    lcd.clear();

    lcd.print(text);    

}

byte sendAdress(int address){
  byte firstByte = (byte)address,
       secondByte = (byte)(address >> 8);
  
  SPI.transfer(firstByte);
  delay(25);
  byte val = SPI.transfer(secondByte); 
  delay(25);
  return val;
}
