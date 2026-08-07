# FreeRTOS Practice Programs — Arduino Uno R4

A collection of FreeRTOS practice programs for the Arduino Uno R4 board, written as part of a structured daily learning routine.

## What is FreeRTOS?

FreeRTOS is a free, open-source real-time operating system kernel that enables multitasking on microcontrollers. It allows multiple tasks to run concurrently, scheduled by priority and timing, which is essential for applications that handle multiple inputs/outputs, meet strict timing requirements, or need structured task communication via queues, semaphores, and mutexes.

## Hardware Requirements

| Component | Description |
|-----------|-------------|
| Arduino Uno R4 Minima | Renesas RA4M1, 48 MHz, 32 KB SRAM |
| Arduino Uno R4 WiFi | Renesas RA4M1 + ESP32-S3, with WiFi/Bluetooth |
| USB-C cable | Required for both Uno R4 boards |

> **Note:** Code in this repository targets the Uno R4 (Renesas architecture). The classic Uno R3 (ATmega328P) is also supported, though pin mappings may differ.

## Required Library

The programs use the **Arduino_FreeRTOS** library — a maintained port of FreeRTOS for Arduino boards.

- **FreeRTOS kernel:** Richard Barry
- **Arduino_FreeRTOS port:** Phillip Stevens (feilipu)
- **Recommended version:** 3.1.0 or newer (includes Uno R4 / Renesas support)
- **Source:** [github.com/feilipu/Arduino_FreeRTOS](https://github.com/feilipu/Arduino_FreeRTOS)

### Installation

**Option 1 — Arduino Library Manager (recommended):**

1. Open the Arduino IDE.
2. Navigate to **Tools → Manage Libraries…**
3. Search for `Arduino_FreeRTOS`.
4. Select the entry by **Richard Barry** and click **Install**.
5. Close the Library Manager once installation completes.

**Option 2 — Manual installation (ZIP):**

1. Download the library from the [GitHub repository](https://github.com/feilipu/Arduino_FreeRTOS/archive/refs/heads/master.zip).
2. In Arduino IDE, go to **Sketch → Include Library → Add .ZIP Library…**
3. Select the downloaded ZIP file.

After adding `#include "Arduino_FreeRTOS.h"` to a sketch, all required FreeRTOS headers are automatically available.

## Board Setup

1. Install Arduino IDE 2.x from [arduino.cc](https://www.arduino.cc/en/software).
2. Open **Tools → Board → Boards Manager…**
3. Search for `Arduino Uno R4`.
4. Install **"Arduino Uno R4 Boards"** by Arduino.
5. Select the board: **Tools → Board → Arduino Uno R4 → Arduino Uno R4 Minima** (or **WiFi**).
6. Select the port: **Tools → Port** (e.g., `COM3`).

## Running a Program

1. Open a program file (e.g., `LED_Blinking.ino`).
2. Verify the board and port are selected.
3. Click **Upload**.
4. Wait for "Done uploading" in the terminal.

## Resources

- [Official FreeRTOS Documentation](https://www.freertos.org/Documentation/00-WhereToStart)
- [Arduino_FreeRTOS Repository](https://github.com/feilipu/Arduino_FreeRTOS)
- [Arduino Uno R4 Documentation](https://docs.arduino.cc/hardware/uno-r4/)