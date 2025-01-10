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
GC_init(&(GCAttribute){ 
    .threshold = GC_INFINTY_THRESHOLD  // Set the GC threshold to infinity (disable automatic collection)
});
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
GC_assign_ref(type, ref, data);
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
#include <time.h>
#include "../include/collector.h"
#include "../include/reference.h"

// Function to generate a random string of a given length
char* random_string(size_t length) {
    char* string = (char*)GC_malloc(length + 1);

    for (size_t i = 0; i < length; i++) {
        string[i] = 'a' + rand() % 26;
    }
    string[length] = '\0';

    return string;
}

// Function to create a weak reference to a random integer in a given range
GCWeakRef(int) random_int(int min, int max) {
    GCWeakRef(int) integer;
    GC_malloc_weak_ref(integer, sizeof(int));

    *integer = min + rand() % (max - min + 1);

    return integer;
}

int main() {
    // Initialize the garbage collector
    GC_init(&(GCAttribute){ 
        .threshold = GC_INFINTY_THRESHOLD // Set the GC threshold to infinity (disable automatic collection)
    });
    printf("Allocated %d frames.\n", GC_size_of_frames());

    srand(time(NULL));

    // Generate a random string
    char* string = random_string(10);
    printf("String: %s\n", string);

    // Weak reference
    GCWeakRef(int) integer = random_int(1, 10);
    printf("Integer: %d\n", *integer);

    char *message = "I am strong reference.";

    // Strong reference
    GCRef ref;
    GC_malloc_ref(ref, sizeof(message));
    GC_assign_ref(char*, ref, message);
    printf("Strong reference: %s\n", GC_deref(char*, ref));

    // Explicitly collect garbage
    GC_collect();

    // Check if there are still allocated objects
    int object_count = GC_size_of_objects();
    if (object_count == 0) {
        printf("No allocated objects.\n");
    } else {
        printf("Still remains %d allocated objects.\n", object_count);
    }

    // Cleanup
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