#include <Arduino_FreeRTOS.h>

TickType_t mutex_timeout = 1000/portTICK_PERIOD_MS;

static SemaphoreHandle_t mutex_1;
static SemaphoreHandle_t mutex_2;

// Tasks

// Task A (high priority)
void doTaskA(void *parameters) {

  // Loop forever
  while (1) {
    //Declare the tracking variables at the top of the loop scope
    bool gotMutex1 = false;
    bool gotMutex2 = false;

    // Take mutex 1 (introduce wait to force deadlock)
    if(xSemaphoreTake(mutex_1, mutex_timeout) == pdTRUE){
      gotMutex1 = true;
      Serial.println("Task A took mutex 1");
      vTaskDelay(1 / portTICK_PERIOD_MS);

    // Take mutex 2
      if(xSemaphoreTake(mutex_2, mutex_timeout) == pdTRUE){
        gotMutex2 = true;
        Serial.println("Task A took mutex 2");

        // Critical section protected by 2 mutexes
        Serial.println("Task A doing some work");
        vTaskDelay(500 / portTICK_PERIOD_MS);
      } else {
        Serial.println("Task A timedout waiting for mutex 2");
      }
    } else {
      Serial.println("Task A timedout waiting for mutex 1");
    }

    // Give back mutexes
    if (gotMutex2) {
      xSemaphoreGive(mutex_2);
    }
    if (gotMutex1) {
      xSemaphoreGive(mutex_1);
    }

    // Wait to let the other task execute
    Serial.println("Task A going to sleep");
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

// Task B (low priority)
void doTaskB(void *parameters) {

  // Loop forever
  while (1) {
    //Declare the tracking variables at the top of the loop scope
    bool gotMutex1 = false;
    bool gotMutex2 = false;

    // Take mutex 2 (introduce wait to force deadlock)
    if(xSemaphoreTake(mutex_2, mutex_timeout) == pdTRUE){
      gotMutex2 = true;
      Serial.println("Task B took mutex 2");
      vTaskDelay(1 / portTICK_PERIOD_MS);

    // Take mutex 1
      if(xSemaphoreTake(mutex_1, mutex_timeout) == pdTRUE){
        gotMutex1 = true;
        Serial.println("Task B took mutex 1");

        // Critical section protected by 2 mutexes
        Serial.println("Task B doing some work");
        vTaskDelay(500 / portTICK_PERIOD_MS);
      } else {
        Serial.println("Task B timedout waiting for mutex 1");
      }
    } else {
      Serial.println("Task B  timedout waiting for mutex 2");
    }
    // Give back mutexes
    if (gotMutex2) {
      xSemaphoreGive(mutex_2);
    }
    if (gotMutex1) {
      xSemaphoreGive(mutex_1);
    }

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