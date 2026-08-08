#include "Arduino_FreeRTOS.h"

static const int led_pin = LED_BUILTIN; // On board led 

static const int rate_1 = 500;
static const int rate_2 = 323;

void toggleLED1(void *parameter){
  while (1) {
    digitalWrite(led_pin,HIGH);
    vTaskDelay(rate_1 / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(rate_1 / portTICK_PERIOD_MS);
  }
}

void toggleLED2(void *parameter){
  while (1) {
    digitalWrite(led_pin,HIGH);
    vTaskDelay(rate_2 / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(rate_2 / portTICK_PERIOD_MS);
  }
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(led_pin, OUTPUT);

  xTaskCreate(toggleLED1,    //Function to be called
            "Toggle_LED-1",   //Name of the task
             128,          //Stack size or stack depth
             NULL,          //Parameter to pass to function
             1,             //Task priority
             NULL);         //Task handle
  
  xTaskCreate(toggleLED2,    //Function to be called
            "Toggle_LED-2",   //Name of the task
             128,          //Stack size or stack depth
             NULL,          //Parameter to pass to function
             1,             //Task priority
             NULL);
  
  vTaskStartScheduler();

}

void loop() {
  // put your main code here, to run repeatedly:

}