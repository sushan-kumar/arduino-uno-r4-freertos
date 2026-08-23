#include <Arduino_FreeRTOS.h>

static const int led_pin = LED_BUILTIN;
static SemaphoreHandle_t mutex;

void blinkLED(void *parameters) {
  // safely copy the value into our own local variable 
  // before the master task wakes up and destroys the original
  int num = *(int *)parameters;

  // unlock the mutex to tell the master task we are done copying
  xSemaphoreGive(mutex);

  Serial.print("Received: ");
  Serial.println(num);

  pinMode(led_pin, OUTPUT);

  while (1) {
    digitalWrite(led_pin, HIGH);
    vTaskDelay(num / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(num / portTICK_PERIOD_MS);
  }
}

void masterTask(void *parameters) {
  // this local variable will be destroyed when this task ends!
  long int delay_arg;

  Serial.println("Enter a number for delay (milliseconds)");
  
  while (Serial.available() <= 0) {
    vTaskDelay(10 / portTICK_PERIOD_MS); 
  }

  delay_arg = Serial.parseInt();
  Serial.print("Sending: ");
  Serial.println(delay_arg);

  // lock the mutex right before launching the other task
  xSemaphoreTake(mutex, portMAX_DELAY);

  xTaskCreate(blinkLED, "Blink LED", 256, (void *)&delay_arg, 1, NULL);

  // try to take the mutex again. since we already locked it above, 
  // this forces this task to freeze here until blinkLED unlocks it.
  xSemaphoreTake(mutex, portMAX_DELAY);
  
  // clean up the lock now that we are awake again
  xSemaphoreGive(mutex);

  Serial.println("Done! Local variable is now safely going out of scope.");
  
  // delete this task so it doesn't repeat
  vTaskDelete(NULL);
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("---FreeRTOS Mutex Challenge---");

  mutex = xSemaphoreCreateMutex();

  xTaskCreate(masterTask, "Master", 256, NULL, 1, NULL);

  vTaskStartScheduler();
}

void loop() {
  // left empty
}