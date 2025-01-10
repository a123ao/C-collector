#ifndef REFERENCE_H
#define REFERENCE_H

#include "collector.h"

typedef void* GCRef;     /* Opaque reference to a garbage collected object that will not be automatically collected */
typedef void* GCWeakRef; /* Opaque reference to a garbage collected object that will be automatically collected */
typedef void* GCAnyRef;  /* Opaque reference to GCRef or GCWeakRef */
#define GCRef(T) T*
#define GCWeakRef(T) T*

int _GC_size_of_ref(GCAnyRef ref);

#define GC_deref(type, ref) (*((type*)ref))
#define GC_size_of_ref(ref) _GC_size_of_ref(ref)

#define GC_malloc_ref(ref, size) \
    do { \
        (ref) = GC_malloc(size); \
        if ((ref) != NULL) { \
            GC_register_root(ref); \
        } \
    } while (0)

#define GC_malloc_weak_ref(ref, size) (ref) = GC_malloc(size);

#define GC_cast_to_ref(weak_ref) GC_register_root(weak_ref)
#define GC_cast_to_weak_ref(ref) GC_unregister_root(ref)

#define GC_assign_ref(type, ref, value) \
    do { \
        if ((ref) != NULL) { \
            GC_deref(type, ref) = (value); \
        } \
    } while (0)

#endif /* REFERENCE_H */