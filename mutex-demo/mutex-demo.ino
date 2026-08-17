#include <Arduino_FreeRTOS.h>

static int shared_val = 0;
static SemaphoreHandle_t mutex;

void incTask(void *parameter){
  int local_var;

  while (1) {
    if (xSemaphoreTake(mutex, 0) == pdTRUE) {
        local_var = shared_val;
        local_var++;
        vTaskDelay(random(100,500) / portTICK_PERIOD_MS);
        shared_val = local_var;

        xSemaphoreGive(mutex);

        Serial.println(shared_val);
    } else {
      // any code
    }
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void setup() {
  // put your setup code here, to run once:
  randomSeed(analogRead(0));

  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("---FreeRTOS Mutex Demo----");

  mutex = xSemaphoreCreateMutex();

  xTaskCreate(incTask, "Task-1", 256, NULL, 1, NULL);

  xTaskCreate(incTask, "Task-2", 256, NULL, 1, NULL);

  vTaskStartScheduler();
}

void loop() {
  // put your main code here, to run repeatedly:

}
