# Arduino Uno R4 · FreeRTOS Examples

A collection of self-contained FreeRTOS example programs for the Arduino Uno R4 platform. Each sketch is a minimal, focused demonstration of a core real-time operating system concept — from basic task scheduling to inter-task communication — and is intended as a practical reference for anyone developing FreeRTOS-based applications on the Uno R4.

## Table of Contents

- [Overview](#overview)
- [Repository Structure](#repository-structure)
- [Hardware Requirements](#hardware-requirements)
- [Prerequisites](#prerequisites)
- [Getting Started](#getting-started)
- [Contributing](#contributing)
- [Resources](#resources)
- [License](#license)

## Overview

FreeRTOS is an open-source real-time operating system kernel for microcontrollers that provides deterministic, priority-based preemptive scheduling. It enables multiple tasks to execute concurrently on a single core, with well-defined mechanisms for timing, synchronization, and inter-task communication.

This repository demonstrates the core FreeRTOS abstractions — **tasks, queues, semaphores, mutexes, and software timers** — in small, independently runnable sketches. Each example targets the Arduino Uno R4 and builds on the ones before it, forming a structured progression from fundamentals to more advanced patterns. The examples are designed to be approachable for developers who are new to real-time operating systems, while remaining useful as a quick reference for more experienced users.

## Repository Structure

Each example resides in its own directory as a complete Arduino sketch (`.ino`), with no dependencies beyond the `Arduino_FreeRTOS` library.

```
.
├── LED_Blinking/          # Task creation + vTaskDelay timing
└── README.md
```

## Hardware Requirements

| Component | Description |
|-----------|--------------|
| Arduino Uno R4 Minima | Renesas RA4M1, 48 MHz, 32 KB SRAM |
| Arduino Uno R4 WiFi | Renesas RA4M1 + ESP32-S3 (WiFi / Bluetooth) |
| USB-C cable | Required for both Uno R4 boards |

> **Note:** Code in this repository targets the Uno R4 (Renesas RA4M1 architecture). The classic Uno R3 (ATmega328P) is also supported by the underlying library, but pin mappings and available RAM differ, so some examples may require adjustment before running on an R3.

## Prerequisites

### 1. Board Support Package

1. Install [Arduino IDE 2.x](https://www.arduino.cc/en/software).
2. Open **Tools → Board → Boards Manager…**.
3. Search for `Arduino Uno R4`.
4. Install **"Arduino Uno R4 Boards"** by Arduino.
5. Select your board under **Tools → Board → Arduino Uno R4 → Minima or WiFi**.

### 2. Arduino_FreeRTOS Library

The examples depend on [Arduino_FreeRTOS](https://github.com/feilipu/Arduino_FreeRTOS) — a maintained port of the FreeRTOS kernel for Arduino boards.

| Component | Maintainer |
|-----------|------------|
| FreeRTOS kernel | Richard Barry |
| Arduino port | Phillip Stevens (feilipu) |
| Required version | 3.1.0 or later (includes Uno R4 / Renesas support) |

**Option A — Arduino Library Manager (recommended):**

1. Open **Tools → Manage Libraries…**.
2. Search for `Arduino_FreeRTOS`.
3. Select the entry by **Richard Barry** and click **Install**.

**Option B — Manual installation (ZIP):**

1. Download the library from the [GitHub repository](https://github.com/feilipu/Arduino_FreeRTOS/archive/refs/heads/master.zip).
2. In the Arduino IDE, navigate to **Sketch → Include Library → Add .ZIP Library…**.
3. Select the downloaded archive.

Once installed, `#include "Arduino_FreeRTOS.h"` exposes the complete FreeRTOS API.

## Getting Started

1. Connect the board via USB-C.
2. Open an example sketch, e.g. `LED_Blinking/LED_Blinking.ino`.
3. Verify the correct board and port are selected under **Tools → Board** and **Tools → Port**.
4. Click **Upload** and wait for the IDE to report "Done uploading".
5. Where applicable, open the **Serial Monitor** at **115200 baud** to view output.

If the upload fails, double-check that the FreeRTOS library and board package versions meet the minimums listed under [Prerequisites](#prerequisites).

## Contributing

Contributions are welcome. If you'd like to add a new example, improve documentation, or fix an issue:

1. Fork the repository and create a feature branch.
2. Keep each example self-contained in its own directory, following the existing structure.
3. Include a brief comment block at the top of each sketch describing the concept it demonstrates.
4. Open a pull request with a clear description of the change.

## Resources

- [Official FreeRTOS Documentation](https://www.freertos.org/Documentation/00-Overview)
- [Arduino_FreeRTOS Repository](https://github.com/feilipu/Arduino_FreeRTOS_Library)
- [Arduino Uno R4 Documentation](https://docs.arduino.cc/hardware/uno-r4-wifi/)

## License

This repository is licensed under the [MIT License](LICENSE). Note that the `Arduino_FreeRTOS` library is licensed separately under its own terms; refer to its repository for details.
