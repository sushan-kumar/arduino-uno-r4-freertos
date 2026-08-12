# include <Arduino_FreeRTOS.h>
#include <string.h>


// this global pointer will hold the memory address of our heap data.
// it is NULL initially so it points to nothing.
char *msg_ptr = NULL;

// Message status
volatile bool msg_ready = false;

void Listener(void *parameter){
  char temp[50];
  int index=0;

  while (1) {
    if (Serial.available()>0) {
      char c = Serial.read();
      if (c != '\n') {
        if (index < 49) {
          temp[index]=c;
          index++;
        }
      } else {
        // Add the null terminator to make it a proper string
        temp[index] = '\0';
        if (msg_ready == false) {
          // Allocate EXACTLY enough heap memory for the string (+1 for the '\0')
          msg_ptr = (char *)pvPortMalloc((index + 1) * sizeof(char));
          if (msg_ptr != NULL) {
            strcpy(msg_ptr, temp);
            msg_ready = true;
            //while (msg_ready == true) {
            //  vTaskDelay(1 / portTICK_PERIOD_MS);
            //}
          } else {
            Serial.println("Error: Not enough heap memory!");
          }
        } else {
          Serial.println("Error: Task B is too busy. Message dropped.");
        }
        index=0;  // Reset the index so we are ready for the next word you type
      }
    } else {
      vTaskDelay(20 / portTICK_PERIOD_MS);
    }
  }
}

void printer(void *parameter){
  while (1) {
    if (msg_ready == true) {  // Check if Task A has flagged that a message is ready
      Serial.print("Task B recieved : ");
      Serial.println(msg_ptr);

      // Free the memory so we don't leak memory.
      vPortFree(msg_ptr);

      //Reset the pointer and the flag so Task A is allowed to send again
      msg_ptr = NULL;
      msg_ready = false;
    } 

    vTaskDelay(20 / portTICK_PERIOD_MS );
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);

  Serial.println("--- Pass A Message Challenge ---");
  Serial.println("Type a message in the Serial Monitor and press Enter...");

  xTaskCreate(Listener, "Listener", 256, NULL, 2, NULL);
  xTaskCreate(printer, "Printer", 256, NULL, 1, NULL);

  vTaskStartScheduler();

}

void loop() {
  // put your main code here, to run repeatedly:

}
