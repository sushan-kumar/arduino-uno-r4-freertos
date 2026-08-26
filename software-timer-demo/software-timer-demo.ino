#include <Arduino_FreeRTOS.h>

static TimerHandle_t one_shot_timer = NULL;
static TimerHandle_t auto_reload_timer = NULL;

// Called when one of the timer expires
void myTimerCallback(TimerHandle_t xTimer){
  //Print message if timer 0 expired
  if ((uint32_t)pvTimerGetTimerID(xTimer) == 0) {
    Serial.println("One-Shot Timer expired");
  }

  //Print message if timer 1 expired
  if ((uint32_t)pvTimerGetTimerID(xTimer) == 1) {
    Serial.println("Auto-Reload Timer expired");
  }
}

void masterTask(void *parameters){
  one_shot_timer = xTimerCreate(
                      "One-Shot Timer",        //Name of timer
                      2000/portTICK_PERIOD_MS, //Period of timer in ticks
                      pdFALSE,                 //Auto relod
                       (void *)0,              //Timer ID
                      myTimerCallback);        //Callback function

  auto_reload_timer = xTimerCreate(
                      "Auto-Reload Timer",        //Name of timer
                      1000/portTICK_PERIOD_MS, //Period of timer in ticks
                      pdTRUE,                 //Auto relod
                       (void *)1,              //Timer ID
                      myTimerCallback);        //Callback function

  // To make sure the timer's where created
  if (one_shot_timer == NULL || auto_reload_timer == NULL) {
    Serial.println("Could not create one of the timer's");
  } else {

    // Wait and then print the message
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    Serial.println("Starting Timers....");

    //Start the timer's(max block time if command queue is full)
    xTimerStart(one_shot_timer, portMAX_DELAY); 
    xTimerStart(auto_reload_timer, portMAX_DELAY);
  }
  // Timer's will work with no user task's
  vTaskDelete(NULL);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("---FreeRTOS Timer Demo---");

  xTaskCreate(masterTask, "Master Task", 256, NULL, 1, NULL);

  vTaskStartScheduler();
}

void loop() {
  // put your main code here, to run repeatedly:

}
