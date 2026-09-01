📄 Effective Java – 3rd Edition Cheat Sheet
(Joshua Bloch – 2018)

# Creating and Destroying Objects

1. Prefer static factory methods over constructors.
2. Consider a builder when faced with many constructor parameters.
3. Enforce the singleton property with a private constructor or enum.
4. Enforce noninstantiability with a private constructor.
5. Prefer dependency injection to hardwiring resources.
6. Avoid creating unnecessary objects.
7. Eliminate obsolete object references to avoid memory leaks.
8. Avoid finalizers and cleaners; use try-with-resources instead.
9. Prefer try-with-resources to try-finally.

# Methods Common to All Objects

10. Override equals when logical equality is needed.
11. Always override hashCode when overriding equals.
12. Always override toString for human-readable output.
13. Override clone judiciously (prefer copy constructors/factories).
14. Consider implementing Comparable for natural ordering.

# Classes and Interfaces

15. Minimize the accessibility of classes and members.
16. In public classes, use accessor methods, not public fields.
17. Minimize mutability.
18. Favor composition over inheritance.
19. Design and document for inheritance, or prohibit it.
20. Prefer interfaces to abstract classes.
21. Design interfaces for static methods where appropriate.
22. Use interfaces only to define types.
23. Prefer class hierarchies to tagged classes.
24. Favor static member classes over nonstatic.
25. Limit source files to a single top-level class.

# Generics

26. Don’t use raw types.
27. Eliminate unchecked warnings.
28. Prefer lists to arrays.
29. Favor generic types.
30. Favor generic methods.
31. Use bounded wildcards for flexibility.
32. Combine generics and varargs carefully.
33. Prefer type-safe heterogeneous containers.

# Enums and Annotations

34. Use enums instead of int constants.
35. Use instance fields instead of ordinals.
36. Use EnumSet instead of bit fields.
37. Use EnumMap instead of ordinal indexing.
38. Emulate extensible enums with interfaces.
39. Prefer annotations to naming patterns.
40. Consistently use the @Override annotation.
41. Use marker interfaces to define types.

# Lambdas and Streams

42. Prefer lambdas to anonymous classes.
43. Prefer method references to lambdas.
44. Favor the standard functional interfaces.
45. Use streams judiciously.
46. Prefer side-effect-free functions in streams.
47. Prefer Collection to Stream for method return types.
48. Use caution when making streams parallel.

# Methods

49. Check parameters for validity.
50. Make defensive copies when needed.
51. Design method signatures carefully.
52. Use overloading judiciously.
53. Use varargs judiciously.
54. Return empty collections or arrays, not null.
55. Return optionals judiciously.
56. Write toString methods that are informative. (reinforces Item 12)

# General Programming

57. Minimize the scope of local variables.
58. Prefer for-each loops to traditional for loops.
59. Know and use the libraries.
60. Avoid float and double for exact calculations.
61. Prefer primitive types to boxed primitives.
62. Avoid strings where other types are more appropriate.
63. Beware of performance costs of string concatenation in loops.
64. Refer to objects by their interfaces.
65. Prefer interfaces to reflection.
66. Use native methods judiciously.
67. Optimize only after profiling.
68. Adhere to generally accepted naming conventions.

# Exceptions

69. Use exceptions only for exceptional conditions.
70. Use checked exceptions for recoverable conditions, runtime exceptions for programming errors.
71. Avoid unnecessary checked exceptions.
72. Favor standard exceptions.
73. Throw exceptions appropriate to the abstraction.
74. Document all exceptions thrown by each method.
75. Include failure-capture information in exceptions.
76. Strive for failure atomicity.
77. Don’t ignore exceptions.

# Concurrency

78. Synchronize access to shared mutable data.
79. Avoid excessive synchronization.
80. Prefer executors, tasks, and streams to threads.
81. Prefer concurrency utilities to wait and notify.
82. Document thread safety.
83. Use lazy initialization judiciously.
84. Don’t depend on the thread scheduler.
85. Avoid thread groups.

# Serialization

86. Prefer alternatives to Java serialization.
87. Implement Serializable with great caution.
88. Write readObject methods defensively.
89. For instance control, prefer enum types to readResolve.
90. Consider serialization proxies instead of serialized instances.
