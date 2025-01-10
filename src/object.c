#include <stdio.h>
#include <stdlib.h>
#include "../include/object.h"

GCObject* __GC_object_create(size_t size, GCObject* next) {
    GCObject* obj = (GCObject*)malloc(sizeof(GCObject));
    if (obj == NULL) {
        return NULL;
    }

    obj->data   = malloc(size);
    if (obj->data == NULL) {
        free(obj);
        return NULL;
    }

    obj->size   = size;
    obj->marked = 0;
    obj->next   = next;

    return obj;
}

void __GC_object_free(GCObject* obj) {
    if (obj == NULL) {
        return;
    }

    printf("Collect %Iu bytes -> %p\n", obj->size, obj->data);

    free(obj->data);
    free(obj);
}

void __GC_object_mark(GCObject* obj) {
    if (obj == NULL || obj->marked) {
        return;
    }

    obj->marked = 1;
}