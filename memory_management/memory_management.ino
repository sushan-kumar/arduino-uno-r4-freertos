#include <Arduino_FreeRTOS.h>

// Random Task
void testTask(void *parameter){
  while (1) {
    int a=1;
    int b[100];

    for (int i=0; i<100; i++) {
      b[i]=a+1;
    }
    Serial.println(b[0]);

    Serial.print("High Water mark (word's): ");         // Stack memory in word's
    Serial.println(uxTaskGetStackHighWaterMark(NULL));  // To use this in arduino uno r4 go to this location C:\Users\User_name\AppData\Local\Arduino15\packages\arduino\hardware\renesas_uno\1.0.1\libraries\Arduino_FreeRTOS
                                                        // Open it in text editor. Search this #define INCLUDE_uxTaskGetStackHighWaterMark 0 and make the 0 to 1
    Serial.print("Heap before malloc (byte's): ");      // Heap memory in byte's before malloc
    Serial.println(xPortGetFreeHeapSize());

    int *ptr =  (int*)pvPortMalloc(256 * sizeof(int));  // malloc

    if(ptr == NULL){
      Serial.println("Not enough heap.");
    } else {
      for(int i=0; i<256 ; i++){
      ptr[i]=3;
      }
    }
    Serial.print("Heap after malloc (byte's): ");      // Heap memory in byte's after malloc
    Serial.println(xPortGetFreeHeapSize());

    vPortFree(ptr);                                     // freeeing the memory

    vTaskDelay(100 / portTICK_PERIOD_MS);

  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  delay(1000);

  Serial.println();
  Serial.println("--- Stack Memory Demo ---");
  xTaskCreate(testTask, "Memory-Demo", 256, NULL, 1, NULL);

  vTaskStartScheduler();

}

void loop() {
  // put your main code here, to run repeatedly:

}
