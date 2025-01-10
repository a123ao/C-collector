#include <stddef.h>
#include <stdlib.h>
#include "../include/frame.h"

GCFrame* __GC_frame_create(GCFrame* prev, GCFrame* next) {
    GCFrame* frame = (GCFrame*)malloc(sizeof(GCFrame));
    if (frame == NULL) {
        return NULL;
    }

    frame->objects      = NULL;
    frame->object_count = 0;
    frame->roots        = NULL;
    frame->prev         = prev;
    frame->next         = next;

    return frame;
}

void __GC_frame_free(GCFrame* frame) {
    if (frame == NULL) {
        return;
    }

    for (GCObject* obj = frame->objects, *next = NULL; obj != NULL; obj = next) {
        next = obj->next;

        __GC_object_free(obj);
    }

    for (GCRoot* root = frame->roots, *next = NULL; root != NULL; root = next) {
        next = root->next;

        __GC_root_free(root);
    }

    free(frame);
}