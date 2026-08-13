#include <Arduino_FreeRTOS.h>

static const uint8_t msg_queue_len = 5;

// Global handle for our queue
static QueueHandle_t msg_queue;

// Task 1: The Printer
void printMessages(void *parameters){
  int item;

  while (1) {
    // Read from the queue. '0' means don't wait if it's empty.
    if (xQueueReceive(msg_queue, (void *)&item, 0) == pdTRUE) {
      Serial.println(item);
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

// Task 2: The Sender 
void sendMessages(void *parameters) {
  // 'static' keeps the value of 'num' saved between loops
  static int num = 0;

  while (1) {
    // Send the number to the queue. Wait up to 10 ticks if full.
    if (xQueueSend(msg_queue, (void *)&num, 10) != pdTRUE) {
      Serial.println("Queue Full");
    } 
    
    num++; 

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  // Standard delay before FreeRTOS starts
  delay(1000); 
  
  Serial.println();
  Serial.println("--- FreeRTOS Queue Demo ---");

  // Create a queue for 5 integers
  msg_queue = xQueueCreate(msg_queue_len, sizeof(int));

  xTaskCreate(printMessages, "Printer", 256, NULL, 1, NULL);
  xTaskCreate(sendMessages, "Sender", 256, NULL, 1, NULL);

  vTaskStartScheduler();
}

void loop() {
  // put your main code here, to run repeatedly:
  // Leave empty since FreeRTOS is handling the tasks
}