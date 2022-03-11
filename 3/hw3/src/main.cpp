#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Keypad.h>
#include <string.h>

static bool passwordShowableChanged = false;
const static int SHOW_PASSWORD = 2;
static bool isPasswordShowAble = false;
//lcd
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
byte colPins[COLS] = {13, 12, 11}; //connect to the column pinouts of the kpd

Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

const static char password[5] = {'1','3','7','8',0};
const static int passwordSize = 4;

static char input[32] ;
static char star_input[32];
static int inputSize = 0;




void changePasswordShowMode();
void addChar(char userCharInput);
void removeChar();
void submitInput();
void clearInput();
void printPassword();
void readInput();

void setup() {
  
  pinMode(SHOW_PASSWORD, INPUT);

  attachInterrupt(digitalPinToInterrupt(SHOW_PASSWORD), changePasswordShowMode, RISING);

  lcd.begin(16, 2);

    
}

void loop() {
    
    readInput();
    if (passwordShowableChanged)
    {
        printPassword();
        passwordShowableChanged = false;
    }
        
    
    
    


    
}

void changePasswordShowMode(){
    isPasswordShowAble = !isPasswordShowAble;
    passwordShowableChanged = true;
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
                    submitInput();
                }else if(kpd.key[i].kchar == '#'){
                    removeChar();
                }else{
                    addChar(kpd.key[i].kchar);
                }
                printPassword();
                
               
            }
        }
    }
}

void addChar(char userCharInput){
    if(inputSize < 32){
        input[inputSize]= userCharInput;
        star_input[inputSize]='*';
        inputSize++;
    }
}

void removeChar(){
    if (inputSize > 0 )
    {
       inputSize--;
        input[inputSize]= 0;
        star_input[inputSize]=0;
    }   

}

void submitInput(){
    lcd.clear();

    if ((passwordSize == inputSize) && (strcmp(password,input) == 0))
    {
         lcd.print("right!");
    }else {
         lcd.print("wrong!");
    }
    clearInput();
    delay(1000);
}

void clearInput(){
    while (inputSize >=0)
    {
        input[inputSize] = 0;
        star_input[inputSize] = 0;
        inputSize--;
    }
    inputSize = 0;  
    
}

void printPassword(){
    lcd.clear();
    if(isPasswordShowAble){
        lcd.print(input);
    }else{
        lcd.print(star_input);
    }

}


