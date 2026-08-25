#include <Arduino_FreeRTOS.h>

// Total number of worker tasks to spawn
static const int num_tasks = 5;

// Shared data structure we'll pass into the worker tasks
typedef struct Message {
  char body[20];
  uint8_t len;
} Message;

// Semaphore to synchronize the master task and worker tasks
static SemaphoreHandle_t sem_params;

void myTask(void *parameters) {
  // Make a local copy of the payload immediately.
  // This is crucial so the master task can safely move on or modify the original.
  Message msg = *(Message *)parameters;

  // Signal to the master task that we've safely copied the data
  xSemaphoreGive(sem_params);

  Serial.print("Recieved: ");
  Serial.print(msg.body);
  Serial.print(" | len: ");
  Serial.println(msg.len);

  // Hang out for a second to simulate work, then clean up this task
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  vTaskDelete(NULL);
}

void masterTask(void *parameters) {
  char task_name[12];
  Message msg;
  char text[20] = "All your base";

  // Prepare the shared payload
  strcpy(msg.body, text);
  msg.len = strlen(text);

  // Spin up the worker tasks
  for (int i = 0; i < num_tasks; i++) {
    sprintf(task_name, "Task %i", i);
    xTaskCreate(myTask, task_name, 128, (void *)&msg, 1, NULL);
  }

  // Wait here until every single worker task has signaled back.
  // This prevents the master task from continuing until all tasks have read the payload.
  for (int i = 0; i < num_tasks; i++) {
    xSemaphoreTake(sem_params, portMAX_DELAY);
  }

  Serial.println("All Tasks created and synchronized");
  
  // Tasks in FreeRTOS must not return, so we delete it when the job is done
  vTaskDelete(NULL);
}

void setup() {
  Serial.begin(115200);

  // Give the serial monitor a moment to connect
  delay(1000);
  Serial.println();
  Serial.println("---FreeRTOS Counting Semaphore Demo---");

  // Create a counting semaphore capable of holding up to 'num_tasks' tokens, starting at 0
  sem_params = xSemaphoreCreateCounting(num_tasks, 0);

  // Spawn the orchestrator task
  xTaskCreate(masterTask, "Master", 128, NULL, 1, NULL);

  // Hand over control to FreeRTOS
  vTaskStartScheduler();
}

void loop() {
  // FreeRTOS takes over, so the standard Arduino loop remains empty
}