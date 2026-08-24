#include <Arduino_FreeRTOS.h>

static const int led_pin = LED_BUILTIN;
static SemaphoreHandle_t bin_sem;

// Blinking Task
void blinkLED(void *parameters) {
  int num = *(int *)parameters;

  // unlock the master task by giving the binary semaphore
  xSemaphoreGive(bin_sem);

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

// Main Master Task
void masterTask(void *parameters) {
  long int delay_arg;

  Serial.println("Enter a number for delay in milliseconds: ");

  // wait for user input
  while (Serial.available() <= 0) {
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
  
  delay_arg = Serial.parseInt();
  Serial.print("Sending: ");
  Serial.println(delay_arg);

  // launch the blink task
  xTaskCreate(blinkLED, "Blink LED", 256, (void *)&delay_arg, 1, NULL);

  // wait for the binary semaphore to be given by blinkLED.
  // because binary semaphores start empty, we will block here immediately!
  xSemaphoreTake(bin_sem, portMAX_DELAY);

  Serial.println("Done! The local variable is now safe to be destroyed.");
  
  // delete the master task since its job is done
  vTaskDelete(NULL);
}

void setup() {
  Serial.begin(115200);
  delay(1000); 

  Serial.println();
  Serial.println("--- FreeRTOS Semaphore Demo ---");

  // create the binary semaphore (it starts with 0 keys available)
  bin_sem = xSemaphoreCreateBinary();

  // launch the master task
  xTaskCreate(masterTask, "Master Task", 256, NULL, 1, NULL);

  vTaskStartScheduler();
}

void loop() {
}