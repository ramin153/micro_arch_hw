#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <queue.h>
#include <LiquidCrystal.h>
#include <stdio.h>
#include <stdlib.h>
#include <EEPROM.h>
#include <semphr.h>

#define ERROR_CODE_SHUT_DOWN 1
#define SPEED_CHANGE 2
#define OVER_TEMPERATURE 3
#define NO_LOG -1
#define PRESET_TEMPERATURE 80
#define CRITICAL_TEMPERATURE 100


#define POT_HG A0
#define MOTOR_SPEED 9
#define LM35 A1

int eeprom_address = 0;
int last_log = NO_LOG;

int motor_speed = 0;
int temperature = 0;

QueueHandle_t queue = NULL;

const int rs = 13, en = 12, d4 = 7, d5 = 6, d6 = 5, d7 = 4;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


SemaphoreHandle_t mutex_address;
SemaphoreHandle_t mutex_last_log;

SemaphoreHandle_t mutex_read_temperature;
SemaphoreHandle_t mutex_read_speed;

bool isSystemtemperatrueHigh = false;

void getDataFromPotansumeMeter(void *params){
    for(;;){
        int value = map(analogRead(POT_HG),0,1023,0,255) ;
        xQueueSend(queue, (void *)&value, 0);
        vTaskDelay(250 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}


void setMotorSpeed(void *params){
    int old_val = -1;
    for(;!isSystemtemperatrueHigh;){
        int value  ;
        if(uxQueueMessagesWaiting(queue) > 0){
            xQueueReceive(queue, &value, 0);
            if(old_val != value){
                xSemaphoreTake(mutex_address, portMAX_DELAY);
                EEPROM.update(eeprom_address,SPEED_CHANGE);
                eeprom_address++;
                old_val = value;
                xSemaphoreGive(mutex_address);
            }

            xSemaphoreTake(mutex_read_speed, portMAX_DELAY);
            motor_speed = value;
            xSemaphoreGive(mutex_read_speed);


            analogWrite(MOTOR_SPEED,value);}
            
            vTaskDelay(250 / portTICK_PERIOD_MS);
    }
    xSemaphoreTake(mutex_read_speed, portMAX_DELAY);
    motor_speed = 0;
    xSemaphoreGive(mutex_read_speed);
    analogWrite(MOTOR_SPEED,0);
    vTaskDelete(NULL);
}

void readTemperature(void *params){
    for(;;){
        int value = (analogRead(LM35)* (5000 / 1024.0))/10 ;
        
        xSemaphoreTake(mutex_read_temperature, portMAX_DELAY);
        temperature = value;
        xSemaphoreGive(mutex_read_temperature);

          if(!isSystemtemperatrueHigh){
            xSemaphoreTake(mutex_last_log, portMAX_DELAY);
            
             if (value >= CRITICAL_TEMPERATURE  )
                last_log = ERROR_CODE_SHUT_DOWN;
            else if(value >= PRESET_TEMPERATURE )
                last_log = OVER_TEMPERATURE;

            xSemaphoreGive(mutex_last_log);
        }


        if (value >= CRITICAL_TEMPERATURE  )
        {
            xSemaphoreTake(mutex_address, portMAX_DELAY);
            EEPROM.update(eeprom_address,ERROR_CODE_SHUT_DOWN);
            
            eeprom_address++;
            isSystemtemperatrueHigh = true;
            xSemaphoreGive(mutex_address);
            
        }else if(value >= PRESET_TEMPERATURE ){
            xSemaphoreTake(mutex_address, portMAX_DELAY);
            EEPROM.update(eeprom_address,OVER_TEMPERATURE);
            eeprom_address++;
            xSemaphoreGive(mutex_address);
        }
      

        
        vTaskDelay(250 / portTICK_PERIOD_MS);

    }
    vTaskDelete(NULL);
}



void printOnLCD(void *params){
    char * temperature_speed = (char*)malloc((sizeof(char)*60));
    char * log = (char*)malloc((sizeof(char)*30));
    int help_speed;
    int help_temerature;
    for(;;){
        xSemaphoreTake(mutex_read_speed, portMAX_DELAY);
        help_speed = motor_speed;
        xSemaphoreGive(mutex_read_speed);
        

        xSemaphoreTake(mutex_read_temperature, portMAX_DELAY);
        help_temerature = temperature;
        xSemaphoreGive(mutex_read_temperature);


        sprintf(temperature_speed,"tem:%03d|spe:%03d",help_temerature,help_speed);
        sprintf(log,"log code:%01d",last_log);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(temperature_speed);                               
        lcd.setCursor(0, 1);
        if(last_log != NO_LOG){
            lcd.print(log);
        }
        if (last_log != ERROR_CODE_SHUT_DOWN)
        {
            xSemaphoreTake(mutex_last_log, portMAX_DELAY);
            last_log = NO_LOG;
            xSemaphoreGive(mutex_last_log);

        }
        
         
        vTaskDelay(250 / portTICK_PERIOD_MS);

    }
    free(temperature_speed);
    free(log);
    vTaskDelete(NULL);
}

void setup() {
    Serial.begin(9600);

    mutex_address = xSemaphoreCreateMutex();
    mutex_last_log = xSemaphoreCreateMutex();
    mutex_read_speed = xSemaphoreCreateMutex();
    mutex_read_temperature = xSemaphoreCreateMutex();

    queue = xQueueCreate(5, sizeof(int));


    pinMode(POT_HG, INPUT);
    pinMode(MOTOR_SPEED, OUTPUT);
    pinMode(LM35, INPUT);

    lcd.begin(16, 2);



    xTaskCreate(readTemperature, "readTemperature", 128, NULL, tskIDLE_PRIORITY + 4, NULL);
    xTaskCreate(getDataFromPotansumeMeter, "getDataFromPotansumeMeter", 128, NULL, tskIDLE_PRIORITY + 3, NULL);
    xTaskCreate(setMotorSpeed, "setMotorSpeed", 128, NULL, tskIDLE_PRIORITY + 2, NULL);
    xTaskCreate(printOnLCD, "printOnLCD", 128, NULL, tskIDLE_PRIORITY + 2, NULL);

}

void loop() {
  // put your main code here, to run repeatedly:
}