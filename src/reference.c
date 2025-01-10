#include "../include/reference.h"

int _GC_size_of_ref(GCAnyRef ref) {
    GCFrame* frame = __gc_state.frames;
    if (frame == NULL) {
        return 0;
    }

    GCObject* obj = __GC_find_object_by_root(&(GCRoot){ .allocated_address = ref });
    if (obj == NULL) {
        return 0;
    }

    return obj->size;
}