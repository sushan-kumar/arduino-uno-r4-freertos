#include <Arduino_FreeRTOS.h>

// Define the timer handle
static TimerHandle_t auto_dim_timer = NULL;

static const int led_pin = LED_BUILTIN; // LED Pin

// How long you want to keep the led on after we stop typing on the serial moniter
static const int led_delay = 5000/portTICK_PERIOD_MS;

// Called when the timer expires
void autoDimCallback(TimerHandle_t xTimer) {
  // Turn off the LED after 5 seconds of inactivity
  digitalWrite(led_pin, LOW);
}

// Task to handle Serial input
void serialTask(void *parameters) {
  while (1) {
    if (Serial.available() > 0) {
      char c = Serial.read();
      
      // Echo character back to the terminal
      Serial.print(c); 
      
      // Turn on the onboard LED
      digitalWrite(led_pin, HIGH); 
      
      // Start or restart the timer's count (max block time if command queue is full)
      xTimerStart(auto_dim_timer, portMAX_DELAY); 
    }
    
    // Yield a short time to prevent task hogging
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void setup() {
  // Put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);
  
  pinMode(led_pin, OUTPUT);
  digitalWrite(led_pin, LOW); // Ensure LED starts off

  Serial.println();
  Serial.println("---FreeRTOS Auto-Dim ---");

  // Create the one-shot timer
  auto_dim_timer = xTimerCreate(
                      "Auto-Dim Timer",          // Name of timer
                      led_delay, // Period of timer in ticks (5 seconds)
                      pdFALSE,                   // Auto reload (pdFALSE = One-Shot)
                      (void *)0,                 // Timer ID
                      autoDimCallback);          // Callback function

  // To make sure the timer was created
  if (auto_dim_timer == NULL) {
    Serial.println("Could not create the timer.");
  } else {
    // Create the task to monitor Serial input
    xTaskCreate(serialTask, "Serial Task", 128, NULL, 1, NULL);
  }

  // Start the FreeRTOS scheduler
  vTaskStartScheduler();
}

void loop() {
  // Put your main code here, to run repeatedly:
}