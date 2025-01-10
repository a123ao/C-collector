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

    // Check for remaining objects
    int remained_object_count = GC_size_of_objects();
    if (remained_object_count > 0) {
        printf("There are %d objects that are not collected.\n", remained_object_count);
    } else {
        printf("All objects are collected.\n");
    }

    GC_pop_frame(); // Pop the GC frame and cleanup
}

int main() {
    // Initialize the garbage collector with custom attributes
    GC_init(&(GCAttribute){
        .threshold = GC_INFINTY_THRESHOLD // Disable automatic garbage collection
    });

    printf("Allocated %d frames.\n", GC_size_of_frames());

    // Seed the random number generator
    srand(time(NULL));

    printf("--------------------------------\n");
    printf("Allocated %d objects before foo().\n", GC_size_of_objects());
    printf("\n");

    // Call the example function
    foo();

    printf("\n");
    printf("Remained %d objects after foo().\n", GC_size_of_objects());
    printf("--------------------------------\n");

    // Cleanup and destroy the garbage collector
    GC_destroy();

    return 0;
}
