# Java review

## Misconception questions

- Primitives vs. reference types
- Strong static typing
- Reference type declaration vs. constructor invocation
- Purpose of inheritance
- static vs. dynamic linkage in expressions

## Language features

The Java programming language...
- is general-purpose, concurrent, class-based, and object-oriented
- is designed to be simple enough that many programmers can achieve fluency
- is strongly and statically typed, clearly distinguishing between errors that can and must be detected at compile time, and those that occur at run time
- is relatively high-level, in that details of the machine representation are not available through the language
- includes automatic storage management, typically using a garbage collector
- does not include any unsafe constructs, such as array accesses without index checking
- normally compiled to the bytecode instruction set and binary format defined in the Java Virtual Machine (JVM) specification

## Code organization

Java programs are organized into packages, which define hierarchical namespaces. A *member* is anything defined inside of a package, which includes classes, interfaces, and subpackages.

## Classes

Class members are classes, interfaces, fields (variables), and methods (functions). Class variables and methods, specified with the `static` keyword, exist independent of any specific object. Instance variables are dynamically created in objects that are instances of this class. Instance methods are invoked in the context of a specific instance object, which can be referenced through an implicit `this` variable.

### Access modifiers

Top-level classes (defined at the package level) have *package* access by default, which means they are accessible only within the same package. They may be accessed by other packages if explicitly declared as `public`.

Member classes (defined inside other classes or interfaces) also have package access by default, but may be explicitly declared with `public`, `protected`, or `private` access modifiers.

### Inheritance

All classes are defined through single inheritance, in which the class implicitly includes each non-private member of its superclass. If the superclass is not explicitly declared (using the `extends` keyword) then it is assumed to be `Object`, such that every class ultimately inherits from the class `Object`.

Classes may also inherit from one or more interfaces (using the `implements` keyword), which inherits the `abstract` and `default` methods of those interfaces.

A class which is declared `final` may not be extended.
A class may be declared `sealed` if only the direct subclasses that are known at declaration are permitted. Furthermore, each direct subclass of a `sealed` class *must* be explicitly declared as `sealed`, `final`, or `non-sealed`.

Note: an interesting comment in the Java SE specs:
> Explicit and exhaustive control over a class's direct subclasses is useful when the class hierarchy is used to model the kinds of values in a domain, rather than as a mechanism for code inheritance and reuse.

### Abstract classes

A class may be declared `abstract` which prevents objects of that class from being instantiated. Any class which is incomplete--that is, contains methods lacking an implementation--*must* be declared `abstract`.

More precisely, any class instance creation expression for an `abstract` class is a compile-time error. Invoking the constructor of an `abstract` class within the constructor of a concrete subclass is acceptable and expected.

### Methods

Methods with the same name but different signatures are referred to as *overloaded*. Due to inheritance-based subtyping, a particular method call may fit the signature of multiple overloaded implementations but the "most specific" one will always be chosen. In truly ambiguous cases a compiler error is raised.

## Interfaces

Interface members are classes, interfaces, constant fields, and methods.

Interfaces follow the same access modifier rules as classes. All interfaces are implicitly `abstract`, and all nested interfaces are implicitly `static`, so neither of these modifiers should be used.

The `sealed` and `non-sealed` modifiers may be used as with normal classes, but apply to both subclasses and subinterfaces. The `final` modifier is not allowed since an interface is useless without at least one concrete implementing class.

### Interface members

#### Fields

All fields in an interface are implicitly `public`, `static`, and `final`. Fields *must* be initialized in the declaration.

It is possible for an interface to inherit multiple fields with the same name, in which case fully-qualified names must be used. Any attempt to refer to such a field by its simple name is a compile-time error.

#### Methods

Interface methods are `public` and `abstract` by default. There are only four possible explicit modifier combinations allowed, each of which requires a block as the method body:
1. `default`: provides a default implementation of the method for any implementing class which does not override it
2. `private`: restricts access to other `private` and `default` methods of the interface; i.e. a helper function
3. `static`: same as other class methods
4. `private static`: helper function for class methods