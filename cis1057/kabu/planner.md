# Project overview
In this project students will implement a low-latency trade matching engine with audit logs, along with a trading account management system.
The educational goal of the project is to expose intro-level programming students to modern application architectural patterns and software engineering practices in a way that is approachable but still challenging and rewarding.

# Architecture

## System Overview
Kabu is designed as a modular C application following clean architecture principles. The system separates concerns into distinct layers that can be developed and tested independently.

```
┌─────────────────────────────────────────────────────────────┐
│                    User Interface Layer                     │
│  ┌─────────────────┐  ┌─────────────────┐  ┌─────────────────┐ │
│  │   CLI Commands  │  │  Batch Loader   │  │  Status Display │ │
│  └─────────────────┘  └─────────────────┘  └─────────────────┘ │
└─────────────────────────────────────────────────────────────┘
                              │
┌─────────────────────────────────────────────────────────────┐
│                   Application Layer                         │
│  ┌─────────────────┐  ┌─────────────────┐  ┌─────────────────┐ │
│  │ Order Processor │  │ Account Manager │  │  Trade Reporter │ │
│  └─────────────────┘  └─────────────────┘  └─────────────────┘ │
└─────────────────────────────────────────────────────────────┘
                              │
┌─────────────────────────────────────────────────────────────┐
│                    Business Logic Layer                     │
│  ┌─────────────────┐  ┌─────────────────┐  ┌─────────────────┐ │
│  │ Matching Engine │  │  Risk Validator │  │  Audit System   │ │
│  └─────────────────┘  └─────────────────┘  └─────────────────┘ │
└─────────────────────────────────────────────────────────────┘
                              │
┌─────────────────────────────────────────────────────────────┐
│                     Data Access Layer                       │
│  ┌─────────────────┐  ┌─────────────────┐  ┌─────────────────┐ │
│  │   Order Books   │  │   Account Data  │  │    Log Files    │ │
│  └─────────────────┘  └─────────────────┘  └─────────────────┘ │
└─────────────────────────────────────────────────────────────┘
```

## Core Components

### 1. Order Book Management System
The heart of the trading platform manages multiple order books, one per trading symbol. Each order book maintains separate buy and sell sides using price-ordered data structures.

**Key responsibilities:**
- Maintain price-time priority for orders
- Provide fast order insertion and matching
- Support order cancellation and modification
- Generate market data snapshots

**Data flow:**
Orders arrive → Validation → Book insertion → Matching attempt → Trade generation

### 2. Matching Engine
Implements the core trading logic using standard exchange algorithms. The engine processes orders atomically and generates trades when buy/sell orders can be matched.

**Matching rules:**
- Price priority: Better prices match first
- Time priority: Earlier orders match first at same price
- Market orders execute immediately at best available price
- Limit orders only execute at specified price or better

### 3. Account Management System
Tracks trader balances and positions, ensuring orders don't exceed available funds or position limits.

**Account structure:**
- Cash balance for purchasing power
- Stock positions (long/short) with average cost tracking
- Real-time profit/loss calculation
- Transaction history

### 4. Audit and Compliance System
Comprehensive logging system that records all system events for regulatory compliance and debugging.

**Audit trail includes:**
- All order lifecycle events (received, matched, cancelled)
- Trade executions with full details
- Account balance changes
- System errors and warnings
- Performance metrics

### 5. Risk Management
Pre-trade and post-trade risk checks to prevent problematic orders.

**Risk controls:**
- Insufficient funds validation
- Position size limits
- Price reasonableness checks
- Rate limiting for order submission

## Data Architecture

### In-Memory Structures
Primary data lives in memory for low-latency access:
- Order books using linked lists for price levels
- Hash tables for fast order lookup by ID
- Account data in balanced trees
- Recent trade history in circular buffers

### Persistent Storage
Critical data persisted to disk for recovery:
- Account balances and positions
- Complete audit log
- End-of-day order book snapshots
- Configuration parameters

### File Formats
- **Binary format** for high-frequency data (trades, ticks)
- **Text format** for human-readable data (accounts, logs)
- **CSV format** for data exchange and reporting

## Error Handling Strategy

### Graceful Degradation
System continues operating even when non-critical components fail:
- Audit logging failure doesn't stop trading
- Account persistence issues trigger safe mode
- Individual order book failures isolated from others

### Error Categories
- **Fatal errors**: Memory allocation, file system corruption
- **Business errors**: Invalid orders, insufficient funds
- **Warning conditions**: High latency, unusual market conditions

## Performance Considerations

### Latency Optimization
- Minimize memory allocations during order processing
- Use object pools for frequently allocated structures
- Avoid system calls in hot paths
- Cache frequently accessed data

### Memory Management
- Pre-allocated pools for orders and trades
- Lazy cleanup of old data structures
- Memory-mapped files for large datasets
- Stack allocation preferred over heap when possible

## Modularity and Testing

### Component Isolation
Each major component has well-defined interfaces and can be tested independently:
- Mock implementations for unit testing
- Integration test suites for component interaction
- Performance benchmarks for critical paths

### Configuration Management
Runtime behavior controlled through configuration files:
- Risk parameters and limits
- Logging levels and destinations
- Performance tuning parameters
- Market data feed settings