#ifndef COLLECTOR_H
#define COLLECTOR_H

#include <stddef.h> /* For size_t */
#include "frame.h"

#define GC_DEFAULT_THRESHOLD 8
#define GC_INFINTY_THRESHOLD -1

typedef enum GCDebugLevel {
    GC_DEBUG_INFO = 0,
    GC_DEBUG_WARNING,
    GC_DEBUG_ERROR,
    GC_DEBUG_NONE
} GCDebugLevel;

typedef struct GCAttribute {
    int             threshold;
    GCDebugLevel    debug_level;
} GCAttribute;

typedef struct GCState {
    GCFrame* frames;
} GCState;

extern GCAttribute __gc_attr;
extern GCState __gc_state;

/* Initialize the garbage collector */
void _GC_init(GCAttribute* attr);

/* Clean up and destroy the garbage collector */
void _GC_destroy(void);

/* Frame management */
void _GC_push_frame(void);
void _GC_pop_frame(void);

/* Register and unregister roots */
void _GC_register_root(void* root, void* data);
void _GC_unregister_root(void* root);

/* Collect garbage */
void _GC_collect(void);

int _GC_size_of_objects(void);
int _GC_size_of_roots(void);
int _GC_size_of_frames(void);

/* Internal functions */
int __GC_should_collect(void);
GCRoot* __GC_find_registered_root(void* root);
void __GC_mark_all(void);
void __GC_sweep(void);
GCObject* __GC_find_object_by_root(GCRoot* root);

void* _GC_malloc(size_t size);

#define GC_init(attr) _GC_init(attr)
#define GC_destroy() _GC_destroy()
#define GC_push_frame() _GC_push_frame()
#define GC_pop_frame() _GC_pop_frame()
#define GC_collect() _GC_collect()
#define GC_register_root(root) _GC_register_root((void*)&(root), (root))
#define GC_unregister_root(root) _GC_unregister_root((void*)&(root))
#define GC_malloc(size) _GC_malloc(size)

#define GC_size_of_objects() _GC_size_of_objects()
#define GC_size_of_roots() _GC_size_of_roots()
#define GC_size_of_frames() _GC_size_of_frames()

#endif /* COLLECTOR_H */