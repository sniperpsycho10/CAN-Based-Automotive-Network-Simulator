# Automotive CAN Network & UDS Diagnostic Simulator

## Overview

This project is a Linux-based Automotive CAN Network and UDS Diagnostic Simulator developed in C using SocketCAN and Virtual CAN (vcan).

The project simulates multiple automotive Electronic Control Units (ECUs) communicating over a CAN network through a Gateway ECU. It also implements basic Unified Diagnostic Services (UDS) functionality used in real automotive diagnostics.

The objective of this project is to learn and demonstrate:

* CAN Communication
* Multi-ECU Architecture
* Gateway Routing
* CAN Message Filtering
* Diagnostic Trouble Codes (DTCs)
* UDS Diagnostics
* Automotive Software Concepts
* Embedded C Development

---

## Project Architecture

```text
Speed ECU
      |
      | CAN ID 0x100
      |
      v

Temp ECU
      |
      | CAN ID 0x101
      |
      v

Battery ECU
      |
      | CAN ID 0x102
      |
      v

-------------------------
      Gateway ECU
-------------------------

Routes:
0x100 -> 0x500
0x101 -> 0x501

Blocks:
0x102

-------------------------
     Dashboard ECU
-------------------------

Features:
- Speed Monitoring
- Temperature Monitoring
- DTC Storage
- UDS Services

            ^
            |
            |

-------------------------
      UDS Scanner
-------------------------
```

---

## Technologies Used

* C
* Embedded C Concepts
* Linux
* SocketCAN
* Virtual CAN (vcan)
* GCC
* Git
* GitHub

---

## Implemented Features

### Phase 1 - Basic CAN Communication

Implemented:

* CAN Sender ECU
* CAN Receiver ECU
* SocketCAN Communication
* Virtual CAN Bus

Features:

* Send CAN Frames
* Receive CAN Frames
* Decode CAN Data

---

### Phase 2 - Multi ECU Simulation

Implemented:

* Speed ECU
* Temperature ECU
* Battery ECU

Features:

* Independent ECU Nodes
* Individual CAN IDs
* Continuous CAN Communication

CAN IDs:

| ECU         | CAN ID |
| ----------- | ------ |
| Speed ECU   | 0x100  |
| Temp ECU    | 0x101  |
| Battery ECU | 0x102  |

---

### Phase 3 - Dashboard ECU

Implemented:

* Central Dashboard ECU

Features:

* Speed Monitoring
* Temperature Monitoring
* Battery Monitoring
* Warning Generation

Examples:

```text
[Dashboard] Speed: 75 km/h
[Dashboard] Temp: 85 C
!!! OVERHEAT WARNING !!!
```

---

### Phase 4 - Diagnostic Trouble Codes (DTC)

Implemented:

* DTC Storage
* Fault Detection

Supported DTCs:

| DTC  | Description     |
| ---- | --------------- |
| P001 | Engine Overheat |
| P002 | Low Battery     |

Example:

```text
ACTIVE DTCs

P001 - Engine Overheat
P002 - Low Battery
```

---

### Phase 5 - Diagnostic Tool

Implemented:

* Diagnostic Request ECU
* DTC Reader

Features:

* Request DTC Information
* Read Stored Faults

---

### Phase 6 - Smart Gateway ECU

Implemented:

* Gateway Routing
* Message Filtering

Routing Table:

| Source ID | Destination ID |
| --------- | -------------- |
| 0x100     | 0x500          |
| 0x101     | 0x501          |

Blocked:

| Source ID |
| --------- |
| 0x102     |

Example:

```text
FORWARDED: 0x100 -> 0x500
FORWARDED: 0x101 -> 0x501
BLOCKED: 0x102
```

---

### Phase 7 - CAN Acceptance Filtering

Implemented:

SocketCAN Acceptance Filters

Dashboard ECU accepts:

```text
0x500
0x501
0x502
0x700
```

Benefits:

* Reduced CPU Load
* Cleaner ECU Processing
* Realistic Automotive Filtering

---

### Phase 8 - UDS Service 0x19

Implemented:

Read DTC Information

Request:

```text
19 01
```

Response:

```text
59 01
```

Features:

* Read Stored DTCs
* Positive Response Handling

---

### Phase 9 - UDS Service 0x22

Implemented:

Read Data By Identifier

Supported DIDs:

| DID  | Description   |
| ---- | ------------- |
| F101 | Vehicle Speed |
| F102 | Temperature   |
| F103 | Battery Level |

Examples:

Request:

```text
22 F1 01
```

Response:

```text
62 F1 01 64
```

Vehicle Speed = 100 km/h

---

### UDS Scanner

Implemented:

```bash
./uds_scanner speed
./uds_scanner temp
./uds_scanner battery
./uds_scanner dtc
```

Examples:

```text
Vehicle Speed = 75 km/h
```

```text
Temperature = 40 C
```

```text
Battery = 90 %
```

```text
P001 - Engine Overheat
```

---

## Current Project Structure

```text
can_engine_temp_monitor/

├── src/
│   ├── speed_sender.c
│   ├── temp_sender.c
│   ├── battery_sender.c
│   ├── dashboard_receiver.c
│   ├── diagnostic_tool.c
│   ├── dtc_reader.c
│   ├── gateway_ecu.c
│   ├── gateway.h
│   ├── uds_scanner.c
│   └── dtc.h
│
├── Makefile
├── run_all.sh
├── README.md
└── screenshots/
```

---

## Build

```bash
make clean
make
```

---

## Setup Virtual CAN

```bash
sudo modprobe vcan

sudo ip link add dev vcan0 type vcan

sudo ip link set up vcan0
```

Verify:

```bash
ip link show vcan0
```

---

## Run Project

```bash
./run_all.sh
```

---

## Example Commands

Read Vehicle Speed:

```bash
./uds_scanner speed
```

Read Temperature:

```bash
./uds_scanner temp
```

Read Battery:

```bash
./uds_scanner battery
```

Read DTCs:

```bash
./uds_scanner dtc
```

---

## Learning Outcomes

This project demonstrates:

* Embedded C Programming
* Linux Socket Programming
* CAN Protocol
* Automotive Diagnostics
* UDS Protocol
* Gateway Routing
* DTC Management
* Multi-ECU Communication
* Automotive Software Architecture

---

## Future Work

Planned Features:

* UDS Service 0x14 (Clear DTC)
* UDS Service 0x11 (ECU Reset)
* UDS Service 0x10 (Diagnostic Session Control)
* Negative Response Codes (NRC)
* ISO-TP Transport Layer
* Multi-Frame UDS Messages
* CAN FD Support
* LIN Bus Simulation
* FlexRay Concepts
* AUTOSAR-inspired Software Layers

---

## Author

Developed as a hands-on Automotive Embedded Systems and UDS Diagnostics learning project using C and SocketCAN.
