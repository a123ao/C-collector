#include <stdio.h>
#include <stdlib.h>
#include "../include/collector.h"

GCAttribute __gc_default_attr = {
    .threshold      = GC_DEFAULT_THRESHOLD,
    .debug_level    = GC_DEBUG_NONE
};

GCAttribute __gc_attr;
GCState __gc_state;

int __GC_should_collect() {
    if (__gc_attr.threshold == GC_INFINTY_THRESHOLD) {
        return 0;
    }

    return _GC_size_of_objects() >= __gc_attr.threshold;
}

GCRoot* __GC_find_registered_root(void* root) {
    GCFrame* frame = __gc_state.frames;
    if (frame == NULL) {
        return NULL;
    }

    for (GCRoot* r = frame->roots; r != NULL; r = r->next) {
        if (r->reference_address == root) {
            return r;
        }
    }

    return NULL;
}

GCObject* __GC_find_object_by_root(GCRoot* root) {
    GCFrame* frame = __gc_state.frames;
    if (frame == NULL) {
        return NULL;
    }

    for (GCObject* obj = frame->objects; obj != NULL; obj = obj->next) {
        if (obj->data == root->allocated_address) {
            return obj;
        }
    }

    return NULL;
}

void __GC_mark_all() {
    GCFrame* frame = __gc_state.frames;
    if (frame == NULL) {
        return;
    }

    for (GCRoot* root = frame->roots; root != NULL; root = root->next) {
        GCObject* obj = __GC_find_object_by_root(root);
        if (obj != NULL) {
            __GC_object_mark(obj);
        }
    }
}

void __GC_sweep() {
    GCFrame* frame = __gc_state.frames;
    if (frame == NULL) {
        return;
    }

    int collected = 0;

    GCObject** obj = &frame->objects;
    while (*obj != NULL) {
        if (!(*obj)->marked) {
            GCObject* unreached = *obj;
            *obj = unreached->next;

            if (__gc_attr.debug_level <= GC_DEBUG_INFO) {
                printf("[GC] Collect %Iu bytes -> %p\n", unreached->size, unreached->data);
            }

            __GC_object_free(unreached);
            frame->object_count--;

            collected++;

            continue;
        }

        (*obj)->marked = 0;
        obj = &(*obj)->next;
    }

    if (__gc_attr.debug_level <= GC_DEBUG_INFO) {
        if (collected > 0) {
            printf("[GC] Collected %d objects.\n", collected);
        } else {
            printf("[GC] No objects are collected.\n");
        }

        int root_count = _GC_size_of_roots();
        if (root_count > 0) {
            printf("[GC] Remaining %d roots.\n", root_count);
        }
    }
}

int _GC_size_of_objects() {
    GCFrame* frame = __gc_state.frames;
    if (frame == NULL) {
        return 0;
    }

    return frame->object_count;
}

int _GC_size_of_roots() {
    GCFrame* frame = __gc_state.frames;
    if (frame == NULL) {
        return 0;
    }

    int count = 0;
    for (GCRoot* root = frame->roots; root != NULL; root = root->next) {
        count++;
    }
    return count;
}

int _GC_size_of_frames() {
    int count = 0;
    for (GCFrame* frame = __gc_state.frames; frame != NULL; frame = frame->prev) {
        count++;
    }
    return count;
}

void _GC_init(GCAttribute* attr) {
    __gc_attr   = attr != NULL ? *attr : __gc_default_attr;
    __gc_state  = (GCState){ NULL };

    _GC_push_frame();
}

void _GC_destroy() {
    if (__gc_attr.debug_level <= GC_DEBUG_INFO) {
        printf("[GC] Destroying garbage collector...\n");
    }

    for (; __gc_state.frames != NULL; ) {
        _GC_pop_frame();
    }
}

void _GC_register_root(void* root, void* data) {
    GCFrame* frame = __gc_state.frames;
    if (frame == NULL) {
        return;
    }

    if (frame->roots == NULL) {
        frame->roots = __GC_root_create(root, data, NULL);
        return;
    }

    GCRoot* existing = __GC_find_registered_root(root);
    if (existing != NULL) {
        existing->allocated_address = data;
        return;
    }

    GCRoot* new_root = __GC_root_create(root, data, frame->roots);
    if (new_root == NULL) {
        return;
    }

    frame->roots = new_root;
}

void _GC_unregister_root(void* root) {
    GCFrame* frame = __gc_state.frames;
    if (frame == NULL) {
        return;
    }

    __GC_remove_root_from_list(frame->roots, root);
}

void _GC_collect() {
    if (__gc_attr.debug_level <= GC_DEBUG_INFO) {
        printf("[GC] Collecting garbage...\n");
    }

    __GC_mark_all();
    __GC_sweep();
}

void _GC_push_frame() {
    GCFrame* frame = __GC_frame_create(__gc_state.frames, NULL);
    if (frame == NULL) {
        return;
    }

    if (__gc_state.frames == NULL) {
        frame->prev = NULL;
        __gc_state.frames = frame;
    } else {
        frame->prev = __gc_state.frames;
        __gc_state.frames->next = frame;
    }

    __gc_state.frames = frame;
}

void _GC_pop_frame() {
    GCFrame* frame = __gc_state.frames;
    if (frame == NULL) {
        if (__gc_attr.debug_level <= GC_DEBUG_WARNING) {
            printf("[GC] No frame to pop.\n");
        }

        return;
    }

    GCFrame* removed = frame;

    __gc_state.frames = frame->prev;
    if (__gc_state.frames != NULL) {
        __gc_state.frames->next = NULL;
    } else {
        __gc_state.frames = NULL;
    }

    __GC_frame_free(removed);
}

void* _GC_malloc(size_t size) {
    GCFrame* frame = __gc_state.frames;
    if (frame == NULL) {
        return NULL;
    }

    if (__GC_should_collect()) {
        _GC_collect();
    }

    GCObject* obj = __GC_object_create(size, frame->objects);
    if (obj == NULL) {
        return NULL;
    }

    frame->objects = obj;
    frame->object_count++;

    return obj->data;
}