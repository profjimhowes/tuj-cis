# Project overview
In this project students will implement a satellite telemetry monitoring system, as well as a command and control system that consumes the telemetry stream and issues guidance commands.
The educational goal of the project is to expose intro-level programming students to modern application architectural patterns and software engineering practices in a way that is approachable but still challenging and rewarding.

# Architecture

The TANCHO platform follows a layered architecture that mirrors real spacecraft systems while remaining accessible to introductory C programmers:

## Layered Architecture Overview
```
┌─────────────────────────────────────────────────────────┐
│                Application Layer                        │
│        (Student Mission Control Software)               │
├─────────────────────────────────────────────────────────┤
│             TANCHO API Abstraction Layer                │
│    (Simplified spacecraft-like interfaces)              │
├─────────────────────────────────────────────────────────┤
│              Hardware Simulation Layer                  │
│     (Simulated sensors, timing, failures)               │
├─────────────────────────────────────────────────────────┤
│                Platform/OS Layer                        │
│          (Standard C library, OS services)              │
└─────────────────────────────────────────────────────────┘
```

## Data Flow Architecture
```
Sensors → Data Acquisition → Processing → Telemetry → Command & Control
   ↓            ↓               ↓           ↓              ↓
STTS22H     Ring Buffers    Validation   Packet         Mission
GPS Sim  →  Timestamping →  Filtering →  Formatting →   Logic
IMU Sim     Error Detect    Analytics    Downlink       Commands
```

## Key Architectural Principles

1. **Separation of Concerns**: Each subsystem has a single, well-defined responsibility
2. **Data-Driven Design**: Telemetry flows through clearly defined data structures
3. **Error Resilience**: Every interface includes comprehensive error handling
4. **Testability**: Components can be tested individually and in integration

# Components

## Sensor Abstraction Layer (`src/sensors/`)
**Purpose**: Provide unified interface to all spacecraft sensors
**Key Files**:
- `sensor_manager.c` - Central sensor coordination
- `temperature/stts22h_driver.c` - Temperature sensor simulation
- `pressure/lps22hh_driver.c` - Pressure sensor simulation  
- `navigation/gps_driver.c` - GPS/position simulation
- `navigation/attitude_sensors.c` - IMU/magnetometer simulation

## Data Acquisition System (`src/telemetry/`)
**Purpose**: Collect, validate, and buffer sensor data in real-time
**Key Files**:
- `data_acquisition.c` - Raw data collection and timestamping
- `data_validation.c` - Quality checks and range validation
- `packet_formatter.c` - Convert data to telemetry packets
- `downlink_manager.c` - Manage communication scheduling

## Command and Control System (`src/navigation/`)
**Purpose**: Process telemetry and generate spacecraft guidance commands
**Key Files**:
- `mission_controller.c` - High-level mission logic
- `orbit_propagation.c` - Predict spacecraft position
- `attitude_determination.c` - Calculate spacecraft orientation
- `asteroid_targeting.c` - Compute flyby trajectories

## Core Infrastructure (`src/core/`)
**Purpose**: Fundamental platform services used by all subsystems
**Key Files**:
- `tancho_time.c` - Timing and scheduling services
- `tancho_memory.c` - Memory management and leak detection
- `tancho_logging.c` - Debug and operational logging
- `ring_buffer.c` - Circular buffers for streaming data

## Utility Libraries (`src/utils/`)
**Purpose**: Common functionality shared across subsystems
**Key Files**:
- `string_utils.c` - Safe string operations
- `crc_checksum.c` - Data integrity verification
- `math_utils.c` - Vector math for orbital mechanics
- `config_parser.c` - Parse mission configuration files

## Example applications (`examples/`)
**Purpose**: Progressive example applications that build complete system
**Structure**:
```
week04_arrays/
├── basic_sensor_reading.c      # Simple temperature monitoring
├── temperature_logger.c        # Array-based data logging
└── sensor_statistics.c         # Calculate min/max/average

week05_pointers/
├── dynamic_buffer_manager.c    # Dynamic memory for sensors
├── sensor_data_processor.c     # Pointer manipulation
└── memory_leak_detector.c      # Debug memory issues

week06_structs/
├── telemetry_packet_builder.c  # Create telemetry structures
├── mission_config_manager.c    # Configuration file handling
└── asteroid_tracker.c          # Complex orbital data structures

week07_files/
├── telemetry_file_logger.c     # Log data to files
├── mission_replay_system.c     # Read and replay missions
└── performance_analyzer.c      # Analyze logged data

week08_integration/
├── autonomous_mission.c        # Complete closed-loop system
├── constellation_coordinator.c # Multi-satellite coordination
└── asteroid_flyby_planner.c    # End-to-end mission planning
```

## 7. Testing Framework (`test/`)
**Purpose**: Validate implementations and catch common errors
**Key Files**:
- `unit/test_sensors.c` - Individual component testing
- `integration/test_full_mission.c` - System-level validation
- `data/reference/` - Golden reference data for comparison

# Development plan
