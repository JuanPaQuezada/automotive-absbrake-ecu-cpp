# Automotive ABS Sensor Firmware (Safe C++) 

*A safety-critical C++ firmware template for Automotive ABS ECUs, demonstrating RAII, zero-dynamic-memory Ring Buffers, and safe memory-mapped I/O.*

## Overview
This repository contains a modular, production-ready C++ implementation designed for a safety-critical **Anti-lock Braking System (ABS) Electronic Control Unit (ECU)**. It demonstrates how to safely interface with wheel speed sensors using memory-mapped peripherals, manage hardware lifecycles automatically with RAII, and store high-speed telemetry data efficiently without relying on dynamic memory allocation.

## Key Features & Architecture

* **Zero Dynamic Memory Allocation:** Adheres to strict safety-critical automotive guidelines (like MISRA C++ and AUTOSAR) by avoiding `malloc`/`free` or `new`/`delete` during the ABS operation. This prevents memory fragmentation and catastrophic out-of-memory errors while the vehicle is in motion.
* **O(1) Ring Buffer for Wheel Telemetry:** Implements a highly optimized, generic `RingBuffer` to store a constant stream of wheel speed data. It utilizes modulo arithmetic (`%`) for deterministic $O(1)$ cyclic indexing without branching, saving valuable CPU cycles.
* **RAII Resource Management:** The `SensorLector` class guarantees safe initialization and teardown of the ABS hardware peripherals. The constructor powers on the sensor, and the destructor ensures it is safely turned off or put into a safe state when the object goes out of scope.
* **Safe Memory-Mapped I/O:** The ABS hardware registers are accessed using `reinterpret_cast<volatile uint32_t*>` to strict physical memory addresses. The `volatile` keyword ensures that the compiler does not optimize away hardware reads, guaranteeing fresh wheel telemetry data on every microsecond.
* **Modular Design:** Cleanly separated into generic logic (`ring_buffer.h`), the ABS sensor interface (`sensor_abs.h`), and low-level hardware implementation (`sensor_abs.cpp`).
* **Type-Safe Hardware Abstraction Layer (HAL):** Replaces dangerous raw pointers and `reinterpret_cast` with a robust, compile-time validated HAL [4]. It utilizes C++ Template Metaprogramming and **SFINAE** (`std::enable_if_t`) to enforce read/write access permissions at compile time, guaranteeing that invalid hardware register access prevents compilation [5, 6].
* **Strong Types for Physical Units:** Employs the `explicit` constructor paradigm to create "Strong Types" (e.g., `velocidad_rueda`) [7]. This prevents dangerous implicit conversions (like accidentally assigning a hydraulic pressure `float` to a wheel speed variable), enhancing type-safety in critical automotive physics calculations [3, 8, 9].
* **Hard Real-Time Profiling (WCET):** Integrates the ARM Cortex-M Data Watchpoint and Trace (**DWT**) unit to profile the Worst-Case Execution Time. By reading the `DWT_CYCCNT` register, the firmware measures function execution duration in precise CPU cycles, ensuring hard real-time deadlines are met [10-12].
* **Flash-Optimized Lightweight Asserts:** Replaces the standard C++ `<cassert>` (which wastes valuable Flash memory by storing long filenames and strings) with a custom `light_assert` macro [13, 14]. Upon failure, it halts the ECU and logs only the Program Counter (PC) memory address for efficient post-mortem debugging [15, 16].
* **Deterministic Execution & Panic Handling:** Compiled with strict flags (`-fno-exceptions` and `-fno-rtti`) to disable non-deterministic C++ features and reduce binary size [2, 17]. Critical failures route to a custom `std::set_terminate` handler to safely shut down the ABS actuators before a system reset [18, 19].
* **Zero Namespace Pollution:** Strict adherence to C++ best practices in header design, avoiding `using namespace std;` in `.h` files to prevent catastrophic naming collisions in large-scale integrations [20].


## Project Structure
```text
📦 project-root
 ┣ 📜 ring_buffer.h    # Generic template logic for the telemetry buffer
 ┣ 📜 sensor_abs.h     # Interface and RAII class for the ABS wheel sensor
 ┣ 📜 sensor_abs.cpp   # Low-level memory mapping for the ABS hardware
 ┗ 📜 main.cpp         # Main ECU application logic
```

## How to Build & Run

### 1. Native PC Simulation (Windows / Linux / macOS)
Compile and test the ABS software logic natively on your computer. *(Note: The physical memory read is swapped to simulated data to prevent a Segmentation Fault).*
# Compile with strict determinism flags (No exceptions, No RTTI)

```bash
g++ -fno-exceptions -fno-rtti -c main.cpp -o main.o
g++ -fno-exceptions -fno-rtti -c sensor_abs.cpp -o sensor_abs.o
g++ main.o sensor_abs.o -o abs_ecu_sim
./abs_ecu_sim
```

### 2. Cross-Compilation for ARM Cortex-M (Bare-metal)
To build the firmware for the actual microcontroller inside the vehicle's ECU:

```bash
arm-none-eabi-g++ -fno-exceptions -fno-rtti -c main.cpp -o main.o
arm-none-eabi-g++ -fno-exceptions -fno-rtti -c sensor_abs.cpp -o sensor_abs.o
arm-none-eabi-g++ main.o sensor_abs.o -o firmware.elf
arm-none-eabi-objcopy -O binary firmware.elf firmware.bin
```

## Author

**Juan Pablo Quezada**
B.Sc. in Intelligent Computing Engineering
Universidad Autónoma de Aguascalientes (UAA)
