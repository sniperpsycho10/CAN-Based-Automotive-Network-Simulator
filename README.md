# CAN-Based Automotive Network Simulator

A pure C implementation of an Automotive Controller Area Network (CAN) simulation using Linux SocketCAN.

This project is being developed incrementally to emulate real-world automotive ECU communication and vehicle network architecture.

## Current Features

- Virtual CAN (vcan0) setup
- CAN Sender ECU
- CAN Receiver ECU
- Engine Temperature Monitoring
- Overheat Warning Detection
- Linux SocketCAN Communication

## Technologies Used

- C
- Linux
- SocketCAN
- CAN Protocol
- GCC
- Git

## Project Roadmap

### Phase 1
- Engine Temperature ECU
- Dashboard ECU

### Phase 2
- Speed ECU
- Battery ECU
- Multi-node CAN Network

### Phase 3
- Diagnostic Trouble Codes (DTC)

### Phase 4
- UDS Diagnostic Services

### Phase 5
- CAN Gateway ECU

### Phase 6
- EV Battery Management System Simulation

### Phase 7
- Automotive Security and Message Validation

## Build

```bash
make