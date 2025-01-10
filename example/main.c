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
    GC_init(&(GCAttribute){ 
        .threshold = GC_INFINTY_THRESHOLD // Set the GC threshold to infinity (disable automatic collection)
    });
    printf("Allocated %d frames.\n", GC_size_of_frames());

    srand(time(NULL));

    /* Random string allocated on heap */
    char* string = random_string(10);
    printf("String: %s\n", string);

    /* Weak reference */
    GCWeakRef(int) integer = random_int(1, 10);
    printf("Integer: %d\n", GC_deref(int, integer));

    /* Strong reference */
    char *message = "I am strong reference.";
    GCRef ref; // Declare as a generic reference
    GC_malloc_ref(ref, sizeof(message));
    GC_assign_ref(char*, ref, message);
    printf("Strong reference: %s\n", GC_deref(char*, ref));

    /* Explicitly collect garbage */
    GC_collect();

    /* Show how many objects are still allocated */
    int object_count = GC_size_of_objects();
    if (object_count == 0) {
        printf("No allocated objects.\n");
    } else {
        printf("Still remains %d allocated objects.\n", object_count);
    }
    GC_destroy();

    return 0;
}