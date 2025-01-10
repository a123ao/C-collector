#ifndef ROOT_H
#define ROOT_H

typedef struct GCRoot {
    void*   reference_address; /* Address of the reference */
    void*   allocated_address; /* Address of the allocated object */
    struct GCRoot* next;
} GCRoot;

GCRoot* __GC_root_create(void* reference, void* data, GCRoot* next);
void __GC_root_free(GCRoot* root);
void __GC_remove_root_from_list(GCRoot* head, void* root);

#endif /* ROOT_H */