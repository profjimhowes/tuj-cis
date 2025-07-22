# Java platform
- Java is a platform: both a programming language and a virtual machine

## Java virtual machine
The JVM runs Java bytecode, much like real hardware runs instructions from an ISA like x86 or ARM.
Any programming language can (in theory) be compiled into Java bytecode and run on the JVM.
Aside from Java itself, Kotlin, Scala, Groovy, and Clojure were specifically designed to be run on the JVM.
Current implementations of the JVM use just-in-time (JIT) compilation to improve performance when appropriate.

## Java programming language
General-purpose, class-based, object-oriented language with strong static typing.
Includes automatic storage management, which means the runtime environment must handle object lifetimes (typically through a garbage collector).

### Type system
Statically-typed: every variable and every expression had a well-defined type which is known at compilation
Strongly-typed: types strictly limit the values that can be held by a variable or produced by an expression, limit the operations that can be performed on those values, and determine the meaning of those operations.
Primitive types include integers, floats, and booleans. The values and operations for these types are essentially fixed by the JVM specification.
Reference types include classes, interfaces, type variables, and arrays.
Objects are instances of classes and arrays, and their reference values are pointers.