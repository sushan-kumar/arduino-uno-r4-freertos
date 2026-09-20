#include <Arduino_FreeRTOS.h>

TickType_t cs_wait = 250;
TickType_t  med_wait = 5000;

static SemaphoreHandle_t lock;


// Low priority Task

void dotaskL(void *parameters){
  TickType_t timestamp;

  while (1) {

    // Take lock
    Serial.println("Task L is taking the lock !");
    timestamp = xTaskGetTickCount() * portTICK_PERIOD_MS;
    xSemaphoreTake(lock, portMAX_DELAY);

    //How much time we spend waiting for the lock
    Serial.print("Task L got the lock. Spent ");
    Serial.print((xTaskGetTickCount()*portTICK_PERIOD_MS)-timestamp);
    Serial.println(" ms waiting for the lock. Doing some work...");

    // Hog the processor
    timestamp = xTaskGetTickCount() * portTICK_PERIOD_MS;
    while ((xTaskGetTickCount()*portTICK_PERIOD_MS)-timestamp < cs_wait);


    // Release the lock
    Serial.println("Task L releasing the lock.");
    xSemaphoreGive(lock);

    // Sleep
    vTaskDelay(500/portTICK_PERIOD_MS);
  }
}

// Medium priority Tasl
void dotaskM(void *parameters){
  TickType_t timestamp;

  while (1) {

    //Hog the processor
    Serial.println("Task M is doing some work..");
    timestamp = xTaskGetTickCount() * portTICK_PERIOD_MS;
    while ((xTaskGetTickCount()*portTICK_PERIOD_MS)-timestamp < med_wait);

    // Sleep
    Serial.println("Task M done !");
    vTaskDelay(500/portTICK_PERIOD_MS);
  }
}

// High priority Task
void dotaskH(void *parameters){
  TickType_t timestamp;

  while (1) {
    // Take lock
    Serial.println("Task H is trying to take the lock");
    timestamp = xTaskGetTickCount() * portTICK_PERIOD_MS;
    xSemaphoreTake(lock, portMAX_DELAY);

    // How long we spen here ?
    Serial.print("Task H got the lock. Spent ");
    Serial.print((xTaskGetTickCount()*portTICK_PERIOD_MS)-timestamp);
    Serial.println(" ms waiting for lock. Doing some work");

    // Hog the processor
    timestamp = xTaskGetTickCount() * portTICK_PERIOD_MS;
    while ((xTaskGetTickCount()*portTICK_PERIOD_MS)-timestamp <  cs_wait);

    // Release the lock
    Serial.println("Task H releasing the lock");
    xSemaphoreGive(lock);

    // Sleep
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void masterTask(void *parameters){
  lock = xSemaphoreCreateMutex();

  xTaskCreate(dotaskL,"low", 128, NULL, 1, NULL);

  vTaskDelay(20/portTICK_PERIOD_MS);

  xTaskCreate(dotaskH, "high", 128, NULL, 3, NULL);
  xTaskCreate(dotaskM, "medium", 128, NULL, 2, NULL);
  
  vTaskDelete(NULL);

}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  delay(1000);

  Serial.println();
  Serial.println("---FreeRTOS priority Inversion Demo---");

  xTaskCreate(masterTask, "master", 128, NULL, 4, NULL);

  vTaskStartScheduler();
}

void loop() {
  // put your main code here, to run repeatedly:

}
