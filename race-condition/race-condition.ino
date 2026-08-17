#include <Arduino_FreeRTOS.h>

static int shared_val = 0;

void incTask(void *parameter){
  int local_var;

  while (1) {

    local_var = shared_val;
    local_var++;
    vTaskDelay(random(100,500) / portTICK_PERIOD_MS);
    shared_val = local_var;

    Serial.println(shared_val);
  }
}

void setup() {
  // put your setup code here, to run once:
  randomSeed(analogRead(0));

  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("---FreeRTOS Mutex Demo----");

  xTaskCreate(incTask, "Task-1", 256, NULL, 1, NULL);

  xTaskCreate(incTask, "Task-2", 256, NULL, 1, NULL);

  vTaskStartScheduler();
}

void loop() {
  // put your main code here, to run repeatedly:

}
