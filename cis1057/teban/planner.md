# Project overview
In this project students will implement moderately complex turn-based games using an extensible object-oriented game engine.
The educational goal of the project is to expose intro-level programming students to modern application architectural patterns and software engineering practices in a way that is approachable but still challenging and rewarding.

# Architecture
The key goal is extensibility, so the entire game engine from top-to-bottom will be designed in accordance with proven object-oriented patterns.

## Object base system
Data types are defined by structs. Inheritance of data types is implied by struct extension.

### Behaviors vs Protocols
The system distinguishes between two types of polymorphic interfaces:

**Behaviors** represent a coherent stateful interface that naturally belongs to a data type (essentially an abstract class). Core methods that are intrinsic to a type are stored in the class's function table, enabling fast dispatch via `object->class->vtable->method`. Examples include `move()` on a `Piece` or `draw()` on a `Card`.

**Protocols** define cross-cutting interfaces that multiple unrelated types can implement. Protocol dispatch occurs by looking up implementations registered on the protocol object itself: `protocol->implementations[type]->function`. This allows unrelated types to implement the same protocol (e.g., `Serializable` implemented by `GameState`, `Player`, and `Board`) and enables adding new protocols to existing types without modifying their class definitions.

### Memory Management
The system uses arena allocation to manage object lifetimes hierarchically:
- **Program arena**: Core type system objects (classes, protocols) that live for the program duration
- **Session arena**: Game-specific objects that live for a game session
- **Turn arena**: Temporary objects that live for a single turn
- **Temp arena**: Scratch objects for immediate use

This eliminates the need for destructors since object cleanup is handled automatically when arenas are reset.

### Core Architecture
Instances of an abstract class are concrete classes, and the constructor of the abstract class describes how to construct concrete classes.
The Class data type extends Type by implementing behaviors.

# Components
## Game state management
- Game state can be defined at a global level (i.e. whose turn it is) but is mostly encapsulated within the state of various game objects: boards, decks, tokens, counters, etc.
- All stateful game components are owned by a singleton state manager, which updates the state based on the outcome of game actions and tracks the history to allow undo/redo and game replays.
## Event handling
- The state manager also includes an event manager subcomponent, which allows for reactive design where state changes can trigger further state changes or even additional action resolution steps.
- Core game rules such as victory conditions or game phase changes are best defined through event handlers as well.
## Action system
- Game actions will follow the command behavioral pattern, which naturally implies a builder creational interface.
- The builder interface naturally fits into a chain-of-responsibility behavioral pattern for specifying, validating and executing the action against the current game state.
- A key feature of this design is that validation logic can be used to not only determine whether a fully-specified action is legal, but also to derive some or all possible legal parameters of a given action type based on the current game state.
- This validation-driven process can dramatically simplify the game client since the input interface doesn't need to replicate validation logic and can instead just poll the game state to determine legal actions.

# Project plan

## Phase 1: Core Object System & Infrastructure
**Goal**: Establish the foundational OOP patterns in C that will support the entire architecture.

### Tasks:
1. **Object Base System**
   - Design base object struct with type information and virtual function table
   - Implement object creation, destruction, and reference counting
   - Create inheritance mechanism through struct composition

2. **Memory Management Framework**
   - Implement object pools for efficient allocation
   - Design ownership patterns (who owns/frees objects)
   - Add debugging support for memory leaks

3. **Basic Collection Types**
   - Dynamic arrays (for lists of objects)
   - Hash tables (for fast object lookups)
   - Iterator patterns for traversing collections

4. **Build System & Testing**
   - Set up Makefile with proper dependencies
   - Create basic test framework
   - Implement simple object creation/destruction tests

## Phase 2: State Management System
**Goal**: Build the singleton state manager that will own all game objects and track history.

### Tasks:
1. **State Manager Core**
   - Implement singleton pattern for global state access
   - Design state container that owns all game objects
   - Create state serialization/deserialization for save/load

2. **History Tracking**
   - Implement command history stack for undo/redo
   - Design state snapshots for game replays
   - Add state validation and integrity checks

3. **Object Registration**
   - Create registry system for different object types
   - Implement factory pattern for object creation
   - Add object lifecycle management (creation, updates, cleanup)

## Phase 3: Action/Command System
**Goal**: Implement the command pattern with builder and chain-of-responsibility for game actions.

### Tasks:
1. **Command Pattern Infrastructure**
   - Design base Action interface with execute/undo methods
   - Implement action factory and registration system
   - Create action parameter validation framework

2. **Builder Pattern Interface**
   - Design fluent builder API for constructing actions
   - Implement parameter validation during building
   - Add support for partial action specification

3. **Chain-of-Responsibility Validation**
   - Create validation chain for action legality
   - Implement context-aware validation rules
   - Add system to derive legal parameters from game state

4. **Action Execution Engine**
   - Integrate actions with state manager
   - Implement transaction-like execution with rollback
   - Add action logging and debugging support

## Phase 4: Event System
**Goal**: Build reactive event handling for state changes and rule enforcement.

### Tasks:
1. **Event Infrastructure**
   - Design event types and event data structures
   - Implement event queue and dispatch system
   - Create event listener registration mechanism

2. **State Change Events**
   - Automatically trigger events on object state changes
   - Implement event filtering and prioritization
   - Add event history tracking

3. **Rule Engine Integration**
   - Define victory conditions as event handlers
   - Implement game phase transitions through events
   - Create cascade handling for multi-step rule resolutions

## Phase 5: Simple Game Implementation (Tic-Tac-Toe)
**Goal**: Validate architecture with minimal viable game to identify design issues.

### Tasks:
1. **Game Objects**
   - Implement Board, Player, and Mark objects
   - Define TicTacToe-specific actions (PlaceMark)
   - Create game state validation rules

2. **Game Logic**
   - Implement turn management
   - Add win condition detection
   - Create game initialization and cleanup

3. **Testing & Refinement**
   - Comprehensive game logic testing
   - Performance analysis and optimization
   - Architecture refinement based on learnings

## Phase 6: Medium Complexity Game (Connect Four)
**Goal**: Test extensibility by adding a more complex game with similar mechanics.

### Tasks:
1. **Extended Game Objects**
   - Implement larger Board with gravity mechanics
   - Create DropToken action with validation
   - Add multi-directional win condition checking

2. **Architecture Validation**
   - Verify code reuse between games
   - Test plugin-style game registration
   - Validate that no core changes are needed

## Phase 7: Complex Game (Custom Board Game)
**Goal**: Showcase full engine capabilities with a multi-phase, resource-management game.

### Tasks:
1. **Advanced Game Objects**
   - Implement Cards, Resources, and complex Board states
   - Create multi-step actions and action sequences
   - Add resource management and trading mechanics

2. **Advanced Features**
   - Multiple game phases with different rules
   - Player hand management and hidden information
   - Complex interaction chains and rule dependencies

3. **Polish & Documentation**
   - Complete API documentation
   - Create game development guide
   - Add example code and tutorials for students