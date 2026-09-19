#include <Arduino_FreeRTOS.h>

TickType_t mutex_timeout = 1000/portTICK_PERIOD_MS;

static SemaphoreHandle_t mutex_1;
static SemaphoreHandle_t mutex_2;

// Tasks

// Task A (high priority)
void doTaskA(void *parameters) {

  // Loop forever
  while (1) {

    // Take mutex 1 (introduce wait to force deadlock)
    xSemaphoreTake(mutex_1, mutex_timeout);
    Serial.println("Task A took mutex 1");
    vTaskDelay(1 / portTICK_PERIOD_MS);

    // Take mutex 2
    xSemaphoreTake(mutex_2, mutex_timeout);
    Serial.println("Task A took mutex 2");

    // Critical section protected by 2 mutexes
    Serial.println("Task A doing some work");
    vTaskDelay(500 / portTICK_PERIOD_MS);

    // Give back mutexes
    xSemaphoreGive(mutex_2);
    xSemaphoreGive(mutex_1);

    // Wait to let the other task execute
    Serial.println("Task A going to sleep");
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

// Task B (low priority)
void doTaskB(void *parameters) {

  // Loop forever
  while (1) {

    // Take mutex 2 (introduce wait to force deadlock)
    xSemaphoreTake(mutex_2, mutex_timeout);
    Serial.println("Task B took mutex 2");
    vTaskDelay(1 / portTICK_PERIOD_MS);

    // Take mutex 1
    xSemaphoreTake(mutex_1, mutex_timeout);
    Serial.println("Task B took mutex 1");

    // Critical section protected by 2 mutexes
    Serial.println("Task B doing some work");
    vTaskDelay(500 / portTICK_PERIOD_MS);

    // Give back mutexes
    xSemaphoreGive(mutex_1);
    xSemaphoreGive(mutex_2);

    // Wait to let the other task execute
    Serial.println("Task A going to sleep");
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

// Master Task 

void masterTask(void *parameters){
  // Create mutexes before starting tasks
  mutex_1 = xSemaphoreCreateMutex();
  mutex_2 = xSemaphoreCreateMutex();

  // Start Task A (high priority)
  xTaskCreate(doTaskA,"Task A",256,NULL,3,NULL);

  // Start Task B (low priority)
  xTaskCreate(doTaskB,"Task B",256,NULL,2,NULL);

  // Delete "Master-Task" task
  vTaskDelete(NULL);
}


void setup() {
  
  // Configure Serial
  Serial.begin(115200);

  // Wait a moment to start (so we don't miss Serial output)
  delay(1000);
  Serial.println();
  Serial.println("---FreeRTOS Deadlock Demo---");

  xTaskCreate(masterTask,"Master-Task",256,NULL,1,NULL);

  // Hand over control to FreeRTOS
  vTaskStartScheduler();

}

void loop() {
}