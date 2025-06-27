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

## Phase 1: Core Infrastructure Foundation (Week 1-2)

### 1.1 Project Setup and Build System
- [ ] Set up CMake build configuration with proper compiler flags
- [ ] Create cross-platform build scripts (Windows/Linux/macOS)
- [ ] Establish coding standards and configure clang-format
- [ ] Set up continuous integration pipeline
- [ ] Create project documentation structure

### 1.2 Core Platform Services (`src/core/`)
**Priority: Critical - Required by all other components**

- [ ] **tancho_core.c**: Platform initialization and shutdown
  - Global state management
  - Subsystem registration and lifecycle
  - Platform version and capability reporting
  
- [ ] **tancho_time.c**: Timing and scheduling services
  - High-resolution timestamp generation
  - Mission elapsed time tracking
  - Simulated spacecraft clock with configurable drift
  - Time synchronization utilities
  
- [ ] **tancho_memory.c**: Memory management and debugging
  - Wrapper functions for malloc/free with leak detection
  - Memory pool allocation for real-time constraints
  - Buffer overflow detection for student safety
  - Memory usage statistics and reporting

- [ ] **tancho_logging.c**: Debug and operational logging
  - Multi-level logging (DEBUG, INFO, WARN, ERROR, CRITICAL)
  - File-based and console output options
  - Thread-safe logging for future extensions
  - Log rotation and size management

### 1.3 Utility Libraries (`src/utils/`)
**Priority: High - Needed by multiple subsystems**

- [ ] **ring_buffer.c**: Circular buffers for streaming data
  - Generic ring buffer implementation
  - Thread-safe operations
  - Overflow handling strategies (drop oldest vs. reject new)
  - Statistics tracking (fill level, overflow count)

- [ ] **string_utils.c**: Safe string operations
  - Buffer-safe string copying and concatenation
  - String parsing utilities for configuration files
  - Safe string-to-number conversions
  - String validation functions

- [ ] **crc_checksum.c**: Data integrity verification
  - CRC-16 and CRC-32 implementations
  - Packet integrity checking for telemetry
  - Simple checksum algorithms for student understanding

## Phase 2: Sensor Simulation Infrastructure (Week 3-4)

### 2.1 Hardware Simulation Layer (`tools/simulator/`)
**Priority: Critical - Provides realistic data for all sensors**

- [ ] **sensor_simulator.c**: Base simulation framework
  - Configurable sensor behavior models
  - Noise injection and drift simulation
  - Failure mode simulation (intermittent, permanent)
  - Realistic timing constraints and latency

- [ ] **orbital_simulator.c**: Spacecraft dynamics simulation
  - Simple Keplerian orbital mechanics
  - Attitude propagation with realistic rates
  - Eclipse detection for power/thermal modeling
  - Asteroid encounter geometry calculation

- [ ] **asteroid_database.c**: Target data management
  - Real asteroid orbital elements database
  - Ephemeris calculation for encounter planning
  - Distance and relative velocity computation
  - Visibility window determination

### 2.2 Sensor Abstraction Layer (`src/sensors/`)
**Priority: High - Core student interface**

- [ ] **sensor_manager.c**: Central sensor coordination
  - Unified sensor registration and discovery
  - Sensor health monitoring and status reporting
  - Coordinated sensor sampling and synchronization
  - Error aggregation and reporting

- [ ] **temperature/stts22h_driver.c**: Temperature sensor simulation
  - Realistic STTS22H behavior model (±0.5°C accuracy)
  - Temperature gradient simulation during orbital cycle
  - Thermal time constants and lag modeling
  - Calibration drift over mission lifetime

- [ ] **pressure/lps22hh_driver.c**: Pressure sensor simulation
  - Cabin pressure monitoring simulation
  - Leak detection algorithms
  - Pressure transient modeling
  - Multi-range pressure measurement

- [ ] **navigation/gps_driver.c**: GPS/position simulation
  - ECEF to geodetic coordinate conversion
  - GPS constellation visibility modeling
  - Position accuracy degradation with altitude
  - Signal acquisition and loss simulation

- [ ] **navigation/attitude_sensors.c**: IMU/magnetometer simulation
  - 3-axis gyroscope simulation with realistic drift
  - 3-axis magnetometer with Earth's magnetic field model
  - 3-axis accelerometer with orbital acceleration
  - Sensor fusion preparation for attitude determination

## Phase 3: Data Acquisition and Telemetry (Week 5-6)

### 3.1 Data Acquisition System (`src/telemetry/`)
**Priority: High - Core data processing pipeline**

- [ ] **data_acquisition.c**: Raw data collection and timestamping
  - Multi-sensor data synchronization
  - Configurable sampling rates per sensor
  - Data buffering with overflow protection
  - Timestamp correlation and drift correction

- [ ] **data_validation.c**: Quality checks and range validation
  - Sensor range checking and limit validation
  - Statistical outlier detection (3-sigma rule)
  - Trend analysis for gradual sensor degradation
  - Data quality flag generation and propagation

- [ ] **packet_formatter.c**: Convert data to telemetry packets
  - Standard spacecraft telemetry packet format
  - Configurable packet types and structures
  - Binary packing/unpacking utilities
  - Packet sequencing and identification

- [ ] **downlink_manager.c**: Communication scheduling
  - Ground station contact window simulation
  - Priority-based packet transmission queuing
  - Data compression for bandwidth optimization
  - Acknowledgment and retransmission logic

## Phase 4: Command and Control Systems (Week 7-8)

### 4.1 Navigation and Guidance (`src/navigation/`)
**Priority: Medium - Advanced features for final integration**

- [ ] **mission_controller.c**: High-level mission logic
  - Mission phase management (launch, cruise, encounter, post-flyby)
  - Autonomous decision making for common scenarios
  - Emergency response procedures
  - Mission timeline execution

- [ ] **orbit_propagation.c**: Predict spacecraft position
  - Simple Keplerian orbit propagation
  - Perturbation modeling (J2, solar radiation pressure)
  - Coordinate system transformations
  - Prediction accuracy assessment

- [ ] **attitude_determination.c**: Calculate spacecraft orientation
  - Sensor fusion algorithms for attitude estimation
  - Star tracker simulation and pattern matching
  - Attitude control system interface
  - Attitude accuracy and stability monitoring

- [ ] **asteroid_targeting.c**: Compute flyby trajectories
  - Trajectory optimization for asteroid encounters
  - Close approach timing and geometry
  - Science observation planning
  - Collision avoidance maneuvers

## Phase 5: Integration and Testing (Week 9-10)

### 5.1 System Integration
**Priority: Critical - Ensure all components work together**

- [ ] **API Integration Testing**
  - Verify all component interfaces work correctly
  - Test error propagation through the system
  - Validate data flow from sensors to commands
  - Performance testing under realistic loads

- [ ] **End-to-End Mission Simulation**
  - Complete mission timeline execution
  - Multi-satellite constellation coordination
  - Realistic failure scenarios and recovery
  - Performance metrics and benchmarking

### 5.2 Testing Infrastructure (`tests/`)
**Priority: High - Essential for student assignments**

- [ ] **Unit Tests**: Individual component validation
  - Sensor driver unit tests with mock data
  - Telemetry processing unit tests
  - Navigation algorithm unit tests
  - Memory management and error handling tests

- [ ] **Integration Tests**: System-level validation
  - Full sensor-to-command data flow testing
  - Multi-component interaction testing
  - Performance and timing constraint validation
  - Resource usage and memory leak detection

- [ ] **Reference Data Generation**
  - Golden reference outputs for student comparison
  - Test data sets for various mission scenarios
  - Performance benchmarks for grading criteria
  - Error injection test cases

## Phase 6: Documentation and Polish (Week 11-12)

### 6.1 API Documentation
- [ ] **Doxygen API Documentation**
  - Complete function and structure documentation
  - Usage examples for each major component
  - Error handling and return code documentation
  - Performance characteristics and limitations

- [ ] **Student Tutorials**
  - Getting started guide with first sensor reading
  - Progressive complexity tutorials for each week
  - Debugging guide for common student errors
  - Best practices for spacecraft programming

### 6.2 Development Tools (`tools/`)
- [ ] **Analysis Tools**
  - Python scripts for telemetry data visualization
  - Performance analysis and profiling tools
  - Memory usage analysis and leak detection
  - Mission timeline analysis and optimization

- [ ] **Grading Infrastructure**
  - Automated test runners for student submissions
  - Rubric-based grading with partial credit
  - Plagiarism detection for code submissions
  - Performance benchmarking and scoring

## Implementation Priorities

### Critical Path Dependencies
1. **Core Infrastructure** → All other components
2. **Sensor Simulation** → Sensor drivers and telemetry
3. **Data Acquisition** → Command and control systems
4. **Testing Framework** → Student assignment validation

### Risk Mitigation
- **Modular Development**: Each component can be developed and tested independently
- **Interface-First Design**: Define all APIs before implementation
- **Continuous Testing**: Unit tests written alongside implementation
- **Incremental Integration**: Add one component at a time to integration tests

### Resource Allocation
- **Weeks 1-4**: 70% core infrastructure, 30% sensor simulation
- **Weeks 5-8**: 60% telemetry systems, 40% navigation systems  
- **Weeks 9-12**: 50% integration testing, 30% documentation, 20% polish

This development plan ensures a robust, educational platform that provides students with authentic spacecraft programming experience while maintaining manageable complexity for introductory C programming students.