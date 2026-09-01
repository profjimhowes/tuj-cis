# Core Concepts and Principles

## Overview

This document defines the semantic backbone of the game: the **concepts** that carry meaning across all phases of play, and the **principles** (derived from thermodynamics and economics) that govern how those concepts are translated into era-specific mechanics.

The hierarchy is: **Concepts** (what matters) → **Principles** (the laws) → **Mechanics** (what the player does). Mechanics evolve through technological eras; concepts and principles do not.

-----

## Concepts

The concepts are the stable semantics of the game. Every era-specific mechanic must be an instantiation of one or more of these concepts. When designing mechanics for a new era, each concept should be examined: “How does this manifest now?”

### Place

Things exist somewhere. Geography matters. Resources are located, facilities are situated, distance is real. The *meaning* of place evolves through the progression — early game, walking distance is the hard constraint; late game, global and even interplanetary logistics networks compress distance — but the concept never stops mattering. Place creates the spatial puzzle that underlies all logistics, because movement has a cost and proximity is a scarce resource.

### Process

Transforming inputs into outputs under specific conditions. This is the heart of the game. Every process has:

- **Inputs**: materials, energy
- **Constraints**: temperature, pressure, catalysts, purity, etc.
- **Outputs**: products, byproducts, waste

A campfire reducing ore and a plasma etcher patterning a wafer are the same *concept* — wildly different mechanics, same semantic slot. The player’s engagement with Process shifts from direct hands-on manipulation (early) to systems design and orchestration (late).

### Time

Things take time. Processes have duration. Some things can happen in parallel, some are sequential. Time pressure may come from decay, spoilage, wear, demand, or narrative goals. The *granularity* of meaningful time shifts through the progression — early game the player cares about hours, late game about project timelines spanning months or years.

### Labor

Work requires agency. The concept is always “who or what performs this process and what are they capable of?” but the instantiation shifts dramatically:

- **Early**: the player directly, perhaps with simple tools
- **Middle**: workers and teams with varying skills
- **Late**: specialists, technicians, engineers with deep domain expertise
- **Advanced**: automation and robotics that still require design, maintenance, and oversight

Labor connects to Process (who performs it), Knowledge (what they’re capable of), and Place (where they work).

### Knowledge

Understanding how to do things. The game’s setting assumes formal knowledge is *available* but not necessarily *applied*. In this sense, formal knowledge is not necessarily a research/discovery mechanic but an *activation* mechanic — the player has access to blueprints and reference materials, but needs the prerequisites (materials, equipment, precision, trained labor) to make them real.

Knowledge also has an implicit locality that is realized in Labor as applied to a specific Process: the map is not the territory, and a blueprint of a complex industrial processing chain is not the actual operating equipment. Accumulation of domain expertise — what we usually refer to as Learning — is the process by which Knowledge enhances the capabilities of Labor.

### Value

The measure of what things are worth, emerging from scarcity, effort, and opportunity cost. Whether Value is an explicit mechanic (currency, markets, trade) or implicit in resource allocation decisions is a design choice, but the concept is always present: every decision to do X is a decision not to do Y, and the relative worth of X and Y depends on the player’s specific situation.

-----

## Principles

The principles are the laws governing how concepts translate into mechanics. They are derived from thermodynamics and economics — two fields that are deeply analogous, both being fundamentally about transformations, constraints, and irreversibility.

### 1. Conservation

**Matter and energy are conserved. Every process has a full mass and energy balance.**

Nothing is created or destroyed — only transformed. The books must always balance. This is the First Law of Thermodynamics, and economically it manifests as budget constraints and accounting identities.

*Implications:* Every process produces byproducts and waste alongside desired outputs. You don’t just “make steel” — you make steel *and* slag *and* CO₂ *and* waste heat. Managing unwanted outputs is as real as producing wanted ones. Waste streams become feedstocks for other processes, or they become pollution problems that must be solved.

### 2. Dissipation

**Every transformation has a cost. Energy degrades, materials disperse, order requires work to maintain.**

You can never perfectly recycle, never achieve 100% yield, never run a process without losses. This is the Second Law. The player is always running uphill against entropy, and the measure of technological progress is *how efficiently you fight that battle*.

*Implications:* Equipment wears out. Skills atrophy without practice. Stockpiles degrade. Infrastructure requires maintenance or it decays. Early processes are thermodynamically brutal (a bloomery wastes most of its energy); advanced processes are more efficient but require vastly more complex supporting infrastructure. The player trades local inefficiency for systemic complexity.

*Economic parallels:* Transaction costs, depreciation, diminishing returns.

### 3. Thresholds

**Some transformations require minimum concentrations of energy or resources to occur at all.**

You cannot smelt iron with body heat regardless of time invested. Certain processes demand a minimum temperature, minimum ore grade, minimum pressure, minimum purity of inputs. This is the activation energy concept from chemistry and the minimum efficient scale from economics.

*Implications:* Natural gating in the tech progression without arbitrary “research this first” locks. The player literally *cannot* perform certain processes until the required conditions are achievable. Reaching those conditions is the puzzle. Cleverness with catalysts, fluxes, or process design can sometimes lower thresholds — a direct application of Knowledge.

### 4. Diminishing Returns

**Every resource has declining marginal value. Every process has an efficiency frontier.**

The first unit of iron is transformative; the thousandth is routine. The richest ore deposits are exploited first; progressively poorer deposits require more effort per unit of output. This is foundational in both thermodynamics (exergy depletion) and economics (marginal analysis).

*Implications:* Drives the exploration-vs-intensification trade-off. At some point, squeezing more from the current setup costs more than expanding to new resources. That crossover point depends on the player’s specific situation, making it a genuine decision rather than a solved optimization.

### 5. Comparative Advantage

**Not every place is equally suited to every process.**

Geography creates natural advantages — hydroelectric potential, ore deposits, fertile soil, timber access, water availability. The rational response is specialization and exchange, which creates logistics demands. This principle links Place, Process, and Value into a coherent system.

*Implications:* The map is not decorative — it is a primary driver of strategy. The player’s local resource mix shapes which technological paths are accessible and which require expansion. This is the mechanism through which replayability emerges from the tech dependency graph.

### 6. Coordination Costs

**Complexity is expensive. More complex processes require more precise coordination and are more sensitive to error.**

This is an information-theoretic principle: complex systems require more bits to specify and more effort to maintain against noise and drift. A simple forge is robust to sloppiness; a semiconductor fab fails if a single parameter wanders.

*Implications:* Natural complexity cost that scales with technological advancement. Connects to Labor (more skilled workers needed), Knowledge (more precise understanding required), and Process (tighter tolerances, more parameters to control). The cost of precision is nonlinear — ±10% is easy, ±1% is hard, ±0.1% is enormous.

### 7. Error Accumulation

**Errors compound through serial processes and multiply through coupled systems.**

A five-step process with 90% reliability at each step yields ~59% overall. Quality degrades through long transformation chains unless actively managed. This principle creates natural gameplay around inspection, quality control, feedback loops, and process isolation.

*Implications:* Making a complex product isn’t just about having all the inputs — it’s about maintaining quality through every transformation. The player discovers the need for metrology, standardization, and quality assurance as emergent necessities, not as arbitrary requirements.

### 8. Coupling and Propagation

**Systems can be loosely or tightly coupled. Tight coupling enables efficiency but transmits failures.**

As technology advances, systems tend toward tighter coupling because it improves throughput and efficiency. But tightly coupled systems are fragile — a failure in one component cascades through the whole. This is Perrow’s normal accident theory.

*Implications:* The player faces a genuine trade-off: tightly coupled systems are more productive but more vulnerable. Building buffers, redundancy, decoupling points, and safety margins costs resources but buys resilience. This is a real engineering decision learned through experience, not a tutorial.

### 9. Deterministic Failure

**Catastrophic failure is the endpoint of accumulated, unmanaged degradation — not random misfortune.**

Bridges don’t randomly collapse. Reactors don’t randomly melt down. These failures result from fatigue uninspected, loads unaccounted for, materials not up to spec, warnings ignored. Timing may be stochastic, but failure given sufficient neglect is *certain*.

*Implications:* The game must make accumulating risk *legible*. Cracks appear. Yields drift. Temperatures creep. The attentive player can always see failure approaching. The mechanics for managing failure are engineering practices: redundancy, safety margins, maintenance schedules, inspection, graceful degradation. The game teaches these as necessities, not chores.

### 10. Persistent Knowledge, Recoverable Ruin

**Ruin is possible in subsystems but not in the whole, because knowledge persists and raw materials remain.**

A collapsed facility is a catastrophe — years of accumulated work lost. But the player still knows how to make fire, and the rocks are still there. Total regression to zero is essentially impossible, mirroring the historical reality that civilizations suffer enormous setbacks but never fully lose their accumulated understanding.

*Implications:* The cost of catastrophic failure is *time and accumulated work*, which is the most honest currency in the game. No artificial “game over” — just the real consequence of having to rebuild, with the hard-won understanding of why margins and maintenance mattered. The entire game is justified by this principle: the fun is in finding the path to recovery given the prevailing environment.

-----

## Design Implications

### On Mechanics

Era-specific mechanics are implementations of the concepts, governed by the principles. When designing a new mechanic, ask:

1. Which concepts does it instantiate?
1. Which principles constrain it?
1. Does it teach something true about reality?

### On Progression

The game should feel like an ongoing orchestration of evolving mini-games. Phase transitions introduce new mechanics while retiring or minimizing previously mastered ones, keeping the active scope manageable. What persists across all phases are the concepts and principles — the player’s understanding deepens even as the surface mechanics change.

### On Education

The game is educational above all else. Mechanics should derive from physical and economic reality. The player should learn genuine lessons about engineering, thermodynamics, and economics through play — not through exposition, but through the natural consequences of decisions made within a system that faithfully reflects how the real world works.

# Fundamental game structures
Every composable piece of a complex game is a defined by mechanics--the space of possible moves--and payoff structure--the change in game state that results from executing those moves.

## Resource types
