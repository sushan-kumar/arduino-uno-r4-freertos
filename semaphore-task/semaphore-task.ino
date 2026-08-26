#include <Arduino_FreeRTOS.h>

// --- Configuration ---
enum {BUF_SIZE = 5};
static const int num_prod_tasks = 5;
static const int num_cons_tasks = 2;
static const int num_writes = 3;

// --- Shared Resources ---
static int buf[BUF_SIZE];
static int head = 0;
static int tail = 0;

// --- Synchronization Tools ---
static SemaphoreHandle_t empty_slots;  // Tracks available space
static SemaphoreHandle_t filled_slots; // Tracks unread data
static SemaphoreHandle_t buffer_mutex; // Protects the buffer array from collisions
static SemaphoreHandle_t bin_sem;      // Synchronizes task creation in setup

void producer(void *parameters) {
  // 1. Safely copy the parameter, then unlock the master task
  int num = *(int *)parameters;
  xSemaphoreGive(bin_sem); 

  // 2. Write to the buffer multiple times
  for (int i = 0; i < num_writes; i++) {
    xSemaphoreTake(empty_slots, portMAX_DELAY);  // Wait for an empty slot
    xSemaphoreTake(buffer_mutex, portMAX_DELAY); // Lock the buffer array

    // 3. Critical Section: Write data and advance the head index
    buf[head] = num;
    head = (head + 1) % BUF_SIZE;

    xSemaphoreGive(buffer_mutex);  // Unlock the buffer array
    xSemaphoreGive(filled_slots);  // Signal to consumers that new data is ready
  }
  
  // Task is finished
  vTaskDelete(NULL);
}

void consumer(void *parameters) {
  int val;
  
  while (1) {
    xSemaphoreTake(filled_slots, portMAX_DELAY); // Wait until data is available
    xSemaphoreTake(buffer_mutex, portMAX_DELAY); // Lock the buffer array

    // 1. Critical Section: Read data and advance the tail index
    val = buf[tail];
    tail = (tail + 1) % BUF_SIZE;

    xSemaphoreGive(buffer_mutex); // Unlock the buffer array
    xSemaphoreGive(empty_slots);  // Signal to producers that a slot is now empty

    // 2. Print outside the mutex so we don't hold up other tasks!
    Serial.println(val);
  }
}

void masterTask(void *parameters) {
  // Launch producers and wait for each one to safely copy its ID
  for (int i = 0; i < num_prod_tasks; i++) {
    xTaskCreate(producer, "Prod", 128, (void *)&i, 1, NULL);
    xSemaphoreTake(bin_sem, portMAX_DELAY); 
  }

  // Launch consumers
  for (int i = 0; i < num_cons_tasks; i++) {
    xTaskCreate(consumer, "Cons", 128, NULL, 1, NULL);
  }

  Serial.println("All tasks created");
  
  // Master task's job is done, safely delete it
  vTaskDelete(NULL);
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("---FreeRTOS Semaphore Solution---");

  // Initialize all semaphores and mutexes
  bin_sem = xSemaphoreCreateBinary();
  buffer_mutex = xSemaphoreCreateMutex();
  
  // empty_slots starts completely full (5), filled_slots starts completely empty (0)
  empty_slots = xSemaphoreCreateCounting(BUF_SIZE, BUF_SIZE); 
  filled_slots = xSemaphoreCreateCounting(BUF_SIZE, 0);       

  // Launch the master task to safely handle the task creation process
  xTaskCreate(masterTask, "Master", 256, NULL, 1, NULL);

  // Turn on the FreeRTOS scheduler
  vTaskStartScheduler();
}

void loop() {
  // Left entirely empty because FreeRTOS is handling the execution
}
