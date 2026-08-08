#include <Arduino_FreeRTOS.h>

static TaskHandle_t input = NULL; // Input task handler
static TaskHandle_t led_blinking = NULL; // LED handler

static const int led_pin = LED_BUILTIN; // Pin number
volatile int led_delay = 500; // Initial delay


// Task-1 Reading from serial moniter
void readinput(void *parameter){
  while (1) {
    if (Serial.available() > 0) { // checking if there is data in the serial moniter input
      int temp = led_delay; // assigning the current delay to temp variable
      led_delay=Serial.parseInt(); // loading the value in the serial buffer to delay variable

      if(led_delay < 0){
          Serial.println("Invalid value entered!.Enter a number greater than zero."); // if the new delay is zero or less than zero assign the old value to it so it doesn't cause any error
          led_delay= temp;
      }else {
          Serial.print("Updated delay to: ");
          Serial.print(led_delay);
          Serial.println("ms");
      }
    }
    vTaskDelay(1 / portTICK_PERIOD_MS);
  }
}

// Task-2 Blinking the led
void blinkled(void *parameter){
  while (1) {
    digitalWrite(led_pin, HIGH);
    vTaskDelay(led_delay / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(led_delay / portTICK_PERIOD_MS);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  while (!Serial) {
    ;                    // Waiting for the serial moniter to open
  }

  pinMode(led_pin, OUTPUT);
  Serial.println("--- Adjustable LED Delay ---");
  Serial.println("Enter the delay(in ms) to control the blinking of LED: ");

  xTaskCreate(blinkled, "LED Blinking", 256, NULL, 2, &led_blinking); // HIGH priority task
  xTaskCreate(readinput, "Reading Data", 256, NULL, 1, &input); // Low priority task

  vTaskStartScheduler();
}

void loop() {
  // put your main code here, to run repeatedly:

}
