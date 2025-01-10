#ifndef OBJECT_H
#define OBJECT_H

typedef struct GCObject {
    void*   data;   /* Pointer to the actual data */
    size_t  size;   /* Size of the data */
    int     marked; /* Flag to indicate if the object is reachable */
    struct GCObject* next;
} GCObject;

GCObject* __GC_object_create(size_t size, GCObject* next);
void __GC_object_free(GCObject* obj);
void __GC_object_mark(GCObject* obj);

#endif /* OBJECT_H */