# automotive-absbrake-ecu-cpp
A safety-critical C++ firmware template for Automotive ABS ECUs, demonstrating RAII, zero-dynamic-memory Ring Buffers, and safe memory-mapped I/O.

# Automotive ABS Sensor Firmware (Safe C++)

## Overview
This repository contains a modular, production-ready C++ implementation designed for a safety-critical **Anti-lock Braking System (ABS) Electronic Control Unit (ECU)**. It demonstrates how to safely interface with wheel speed sensors using memory-mapped peripherals, manage hardware lifecycles automatically with RAII, and store high-speed telemetry data efficiently without relying on dynamic memory allocation.

## Key Features & Architecture

* **Zero Dynamic Memory Allocation:** Adheres to strict safety-critical automotive guidelines (like MISRA C++ and AUTOSAR) by avoiding `malloc`/`free` or `new`/`delete` during the ABS operation. This prevents memory fragmentation and catastrophic out-of-memory errors while the vehicle is in motion.
* **O(1) Ring Buffer for Wheel Telemetry:** Implements a highly optimized, generic `RingBuffer` to store a constant stream of wheel speed data. It utilizes modulo arithmetic (`%`) for deterministic $O(1)$ cyclic indexing without branching, saving valuable CPU cycles.
* **RAII Resource Management:** The `SensorLector` class guarantees safe initialization and teardown of the ABS hardware peripherals. The constructor powers on the sensor, and the destructor ensures it is safely turned off or put into a safe state when the object goes out of scope.
* **Safe Memory-Mapped I/O:** The ABS hardware registers are accessed using `reinterpret_cast<volatile uint32_t*>` to strict physical memory addresses. The `volatile` keyword ensures that the compiler does not optimize away hardware reads, guaranteeing fresh wheel telemetry data on every microsecond.
* **Modular Design:** Cleanly separated into generic logic (`ring_buffer.h`), the ABS sensor interface (`sensor_abs.h`), and low-level hardware implementation (`sensor_abs.cpp`).

## Project Structure
```text
📦 project-root
 ┣ 📜 ring_buffer.h    # Generic template logic for the telemetry buffer
 ┣ 📜 sensor_abs.h     # Interface and RAII class for the ABS wheel sensor
 ┣ 📜 sensor_abs.cpp   # Low-level memory mapping for the ABS hardware
 ┗ 📜 main.cpp         # Main ECU application logic
```
### How to Build & Run
1. Native PC Simulation (Windows / Linux / macOS)
Compile and test the ABS software logic natively on your computer. (Note: The physical memory read is swapped to simulated data to prevent a Segmentation Fault).
g++ -c main.cpp -o main.o
g++ -c sensor_abs.cpp -o sensor_abs.o
g++ main.o sensor_abs.o -o abs_ecu_sim
./abs_ecu_sim
2. Cross-Compilation for ARM Cortex-M (Bare-metal)
To build the firmware for the actual microcontroller inside the vehicle's ECU:
arm-none-eabi-g++ -c main.cpp -o main.o
arm-none-eabi-g++ -c sensor_abs.cpp -o sensor_abs.o
arm-none-eabi-g++ main.o sensor_abs.o -o firmware.elf
arm-none-eabi-objcopy -O binary firmware.elf firmware.bin
