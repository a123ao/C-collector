# C Garbage Collector Library

A simple garbage collection library implemented in C for educational purposes. This library provides basic garbage collection functionality including strong references, weak references, and automatic/manual memory management.

## Features

- Mark-and-sweep garbage collection algorithm
- Support for strong and weak references
- Configurable collection threshold
- Manual garbage collection triggering
- Memory leak prevention
- Simple and educational implementation

## Installation

1. Clone the repository
2. Include the library headers in your project:
```c
#include "path/to/collector/include/collector.h"
#include "path/to/collector/include/reference.h"
```
3. Link the library with your project:
```bash
gcc -o my_program my_program.c path/to/collector/build/collector.a
```

## Usage

### Initialization

Initialize the garbage collector at the start of your program:

```c
GC_init((&(GCAttribute){
    .threshold      = GC_INFINTY_THRESHOLD, // Disable automatic garbage collection
    .debug_level    = GC_DEBUG_INFO         // Print debug information
}));
```

### Memory Allocation

The library provides several ways to allocate memory:

1. Basic allocation:
```c
void* data = GC_malloc(size);
```

2. Weak references:
```c
GCWeakRef(int) integer;
GC_malloc_weak_ref(integer, sizeof(int));
```

3. Strong references:
```c
GCRef ref;
GC_malloc_ref(ref, sizeof(data));
```

### Memory Management

Trigger garbage collection manually:
```c
GC_collect();
```

Check the number of allocated objects:
```c
int count = GC_size_of_objects();
```

Clean up when done:
```c
GC_destroy();
```

## Example

Here's a complete example demonstrating various features of the library:

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/collector.h"
#include "../include/reference.h"

// Generate a random string of specified length
GCWeakRef(char) random_string(size_t length) {
    GCWeakRef(char) ref;
    GC_malloc_weak_ref(ref, length + 1); // Allocate memory for the string (with null terminator)

    for (size_t i = 0; i < length; i++) {
        ref[i] = 'A' + rand() % 26; // Fill with random uppercase letters
    }
    ref[length] = '\0'; // Null-terminate the string

    return ref;
}

// Example function demonstrating garbage collector usage
void foo() {
    GC_push_frame(); // Push a new GC frame

    // Allocate memory for a string reference
    GCRef(char) ref;
    GC_malloc_ref(ref, 64); // Memory managed by GC

    memcpy(ref, "This reference will not be collected.", 64);
    printf("%s\n", ref);

    // Generate and print a random string
    printf("Here's a random string: %s\n", random_string(8));

    // Perform explicit garbage collection
    GC_collect();

    GC_pop_frame(); // Pop the GC frame and cleanup
}

int main() {
    // Initialize the garbage collector with custom attributes
    GC_init((&(GCAttribute){
        .threshold      = GC_INFINTY_THRESHOLD, // Disable automatic garbage collection
        .debug_level    = GC_DEBUG_INFO         // Print debug information
    }));

    printf("Allocated %d frames.\n", GC_size_of_frames());

    // Seed the random number generator
    srand(time(NULL));

    // Call the example function
    foo();

    // Cleanup and destroy the garbage collector
    GC_destroy();

    return 0;
}
```

## Contributing

This is an educational project aimed at understanding garbage collection concepts. Feel free to:
- Report bugs
- Suggest improvements
- Submit pull requests
- Use it for learning purposes

## License

This project is available under the MIT License. See the LICENSE file for more details.

## Acknowledgments

This library was created for educational purposes to better understand garbage collection mechanisms and memory management in C.