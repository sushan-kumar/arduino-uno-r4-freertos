// Uno R4 ISR Critical Section Demo
// Increments a global variable in a hardware timer ISR and prints it in a task.

#include <Arduino_FreeRTOS.h>
#include "FspTimer.h" 

const TickType_t task_delay = 2000 / portTICK_PERIOD_MS;
volatile int isr_counter = 0;
FspTimer hardware_timer;

const int led_pin = LED_BUILTIN; 
volatile bool led_state = false;

// Hardware timer interrupt callback
void my_timer_callback(timer_callback_args_t __attribute((unused)) 
  
  // Pause interrupts to safely modify our shared counter
  UBaseType_t saved_int_status = taskENTER_CRITICAL_FROM_ISR();
  isr_counter++;
  taskEXIT_CRITICAL_FROM_ISR(saved_int_status);

  // Toggle the LED so we can physically see the ISR working
  led_state = !led_state;
  digitalWrite(led_pin, led_state);
}

// Task to print and lower the counter values
void printValues(void *parameters) {
  while (1) {
    while (isr_counter > 0) {
      Serial.println(isr_counter);
  
      // Pause context switches to safely decrement the counter
      taskENTER_CRITICAL();
      isr_counter--;
      taskEXIT_CRITICAL();
    }
  
    // Wait 2 seconds before checking again 
    vTaskDelay(task_delay);
  }
}

void setup() {
  pinMode(led_pin, OUTPUT); 

  Serial.begin(115200);
  while (!Serial) { ; }
  delay(1000); 

  Serial.println("\n--- FreeRTOS ISR Critical Section Demo (Uno R4) ---");

  // Create our printing task
  xTaskCreate(printValues, "Print values", 256, NULL, 1, NULL);

  // Dynamically allocate an available GPT hardware timer
  uint8_t timer_type = GPT_TIMER; 
  int8_t tindex = FspTimer::get_available_timer(timer_type); 
  
  // Fallback: force allocation if standard timers are reserved for PWM
  if (tindex < 0) { 
    tindex = FspTimer::get_available_timer(timer_type, true); 
    FspTimer::force_use_of_pwm_reserved_timer(); 
  }

  if (tindex >= 0) { 
    hardware_timer.begin(TIMER_MODE_PERIODIC, timer_type, tindex, 10.0f, 0.0f, my_timer_callback);
    hardware_timer.setup_overflow_irq();
    hardware_timer.open();
    hardware_timer.start();
  }

  // Explicitly start the RTOS scheduler for the Uno R4 architecture
  vTaskStartScheduler();
}

void loop() {
  // Main execution handled by FreeRTOS tasks and hardware interrupts
}