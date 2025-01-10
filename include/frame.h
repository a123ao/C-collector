#ifndef FRAME_H
#define FRAME_H

#include "object.h"
#include "root.h"

typedef struct GCFrame {
    GCObject*   objects;
    size_t      object_count;
    GCRoot*     roots;
    struct GCFrame* prev;
    struct GCFrame* next;
} GCFrame;

GCFrame* __GC_frame_create(GCFrame* prev, GCFrame* next);
void __GC_frame_free(GCFrame* frame);

#endif /* FRAME_H */