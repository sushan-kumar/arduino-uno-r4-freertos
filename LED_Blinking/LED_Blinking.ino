#include "Arduino_FreeRTOS.h"

static const int led_pin = LED_BUILTIN; // On board led 

void toggleLED(void *parameter){
  while (1) {
    digitalWrite(led_pin,HIGH);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(led_pin, OUTPUT);

  xTaskCreate(toggleLED,    //Function to be called
            "Toggle LED",   //Name of the task
             1024,          //Stack size or stack depth
             NULL,          //Parameter to pass to function
             1,             //Task priority
             NULL);         //Task handle
  
  vTaskStartScheduler();

}

void loop() {
  // put your main code here, to run repeatedly:

}