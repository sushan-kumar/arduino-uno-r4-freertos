#include <Arduino_FreeRTOS.h>
#include "FspTimer.h"

static const int led_pin = LED_BUILTIN;
FspTimer myTimer;

// Hardware timer interrupt callback
void timer_callback(timer_callback_args_t __attribute((unused)) *p_args) {
  digitalWrite(led_pin, !digitalRead(led_pin));
}

void setup() {
  pinMode(led_pin, OUTPUT);

  // Dynamically allocate an available GPT hardware timer
  uint8_t timer_type = GPT_TIMER;
  int8_t tindex = FspTimer::get_available_timer(timer_type);
  
  // Fallback: force allocation if standard timers are reserved for PWM
  if (tindex < 0) { 
    tindex = FspTimer::get_available_timer(timer_type, true);
    FspTimer::force_use_of_pwm_reserved_timer();
  }

  // Initialize and start the timer
  if (tindex >= 0) {
    // Parameters: mode, type, index, frequency (1 Hz), duty cycle, callback
    myTimer.begin(TIMER_MODE_PERIODIC, timer_type, tindex, 1.0f, 0.0f, timer_callback);
    myTimer.setup_overflow_irq();
    myTimer.open();
    myTimer.start();
  }

  // Explicitly start the RTOS scheduler for the Uno R4 architecture
  vTaskStartScheduler();
}

void loop() {
  // Main execution handled by FreeRTOS tasks and hardware interrupts
}