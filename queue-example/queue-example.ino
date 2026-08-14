#include <Arduino_FreeRTOS.h>
#include <string.h>
#include <stdio.h>

// custom structure to package the feedback message and blink count for queue 2
struct BlinkMessage {
  char text[10];
  int count;
};

// global handles for our two communication channels
static QueueHandle_t queue1;
static QueueHandle_t queue2;

void TaskA(void *parameters) {
  char serial_buf[32];
  uint8_t idx = 0;
  BlinkMessage incoming_msg;

  while (1) {
    // check if task b sent any milestone feedback via queue 2
    if (xQueueReceive(queue2, &incoming_msg, 0) == pdTRUE) {
      Serial.print("From Task B -> ");
      Serial.print(incoming_msg.text);
      Serial.print(": ");
      Serial.println(incoming_msg.count);
    }

    // read serial input character by character from the user
    while (Serial.available() > 0) {
      char c = Serial.read();

      if (c == '\n' || c == '\r') {
        if (idx > 0) {
          serial_buf[idx] = '\0'; // end the string properly
          
          Serial.print("Echo: ");
          Serial.println(serial_buf);

          int new_delay;
          // parse input if user typed "delay [number]"
          if (sscanf(serial_buf, "delay %d", &new_delay) == 1) {
            // send the parsed number to task b through queue 1
            if (xQueueSend(queue1, &new_delay, 0) == pdTRUE) {
              Serial.print("sent new delay: ");
              Serial.println(new_delay);
            }
          }
          idx = 0; // reset buffer for next command
        }
      } else {
        // keep collecting characters if there's room in the buffer
        if (idx < sizeof(serial_buf) - 1) {
          serial_buf[idx++] = c;
        }
      }
    }

    // yield cpu briefly so other tasks stay responsive
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}

void TaskB(void *parameters) {
  const int led_pin = LED_BUILTIN;
  pinMode(led_pin, OUTPUT);

  int current_delay = 500; // default blink speed
  int blink_counter = 0;
  
  BlinkMessage out_msg;
  strcpy(out_msg.text, "Blinked");

  while (1) {
    // check queue 1 to see if task a requested a new delay rate
    int received_delay;
    if (xQueueReceive(queue1, &received_delay, 0) == pdTRUE) {
      if (received_delay > 0) {
        current_delay = received_delay;
      }
    }

    // toggle the built-in led using our current delay setting
    digitalWrite(led_pin, HIGH);
    vTaskDelay(current_delay / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(current_delay / portTICK_PERIOD_MS);

    // increment counter and report back to task a every 100 blinks
    blink_counter++;
    if (blink_counter >= 100) {
      out_msg.count = blink_counter;
      xQueueSend(queue2, &out_msg, 0);
      blink_counter = 0; // reset counter for the next batch
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000); // standard stabilization delay for uno r4

  // create the two queues for bidirectional messaging
  queue1 = xQueueCreate(5, sizeof(int));
  queue2 = xQueueCreate(5, sizeof(BlinkMessage));

  // create both real-time tasks
  xTaskCreate(TaskA, "TaskA", 256, NULL, 1, NULL);
  xTaskCreate(TaskB, "TaskB", 256, NULL, 1, NULL);

  // start the freertos scheduler
  vTaskStartScheduler();
}

void loop() {
  // left completely empty because rtos manages task execution
}
