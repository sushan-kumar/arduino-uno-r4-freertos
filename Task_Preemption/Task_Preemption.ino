#include <Arduino_FreeRTOS.h>

const char message[] = "Electric blue turtles ate three flying pancakes under the glowing neon moon."; 

// Task handles
static TaskHandle_t task1 = NULL;
static TaskHandle_t task2 = NULL;
static TaskHandle_t task3 = NULL;

// Task-1 to print with a lower priority
void executeTask1(void *parameter){
  int message_length = strlen(message);

  while (1) {
      Serial.println();
      for(int i = 0; i < message_length; i++){
        Serial.print(message[i]);
        
        // Add a small delay so we can visually see Task-2 interrupting!
        // (Because Native USB ignores the 300 baud rate)
        vTaskDelay(20 / portTICK_PERIOD_MS); 
      }
      Serial.println();
      vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

// Task-2 to print with a higher priority
void executeTask2(void *parameter){
  while (1) {
      Serial.print("*");
      vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

// Task-3 to control the other tasks (Replaces the standard Arduino loop)
void controlTask(void *parameter){
  // Suspend the higher priority task for some interval
  for (int i=0; i<4; i++) {
    vTaskSuspend(task2);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    vTaskResume(task2);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }

  // Delete the lower priority task
  if(task1 != NULL){
    vTaskDelete(task1);
    task1 = NULL;
  }

  // This control task is finished, so it can delete itself
  vTaskDelete(NULL);
}

void setup() {
  Serial.begin(9600); 

  // Wait for you to open the Serial Monitor before starting
  while (!Serial) {
    ; 
  }

  Serial.println("\n--- FreeRTOS Task Demo (Uno R4) ---");

  // Create tasks. A stack size of 512 words (2048 bytes) is safe and plenty for the R4
  xTaskCreate(executeTask1, "Task-1", 512, NULL, 1, &task1);
  xTaskCreate(executeTask2, "Task-2", 512, NULL, 2, &task2);
  
  // Create our 3rd task to handle the suspend/resume logic. Priority 3 (highest)
  xTaskCreate(controlTask, "Controller", 512, NULL, 3, &task3);

  // On the Uno R4, you MUST explicitly start the scheduler!
  vTaskStartScheduler();
}

void loop() {
  // This will never run because vTaskStartScheduler() takes over the processor forever.
}