#include <stddef.h>
#include <stdlib.h>
#include "../include/root.h"

GCRoot* __GC_root_create(void* reference, void* data, GCRoot* next) {
    GCRoot* root = (GCRoot*)malloc(sizeof(GCRoot));
    if (root == NULL) {
        return NULL;
    }

    root->reference_address = reference;
    root->allocated_address = data;
    root->next = next;

    return root;
}

void __GC_root_free(GCRoot* root) {
    if (root == NULL) {
        return;
    }

    free(root);
}

void __GC_remove_root_from_list(GCRoot* head, void* root) {
    // If the list is empty, return
    if (head == NULL) {
        return;
    }

    for (GCRoot* current = head, *prev = NULL; current != NULL; prev = current, current = current->next) {
        if (current->reference_address == root) {
            if (prev == NULL) {
                head = current->next;
            } else {
                prev->next = current->next;
            }

            __GC_root_free(current);
            return;
        }
    }
}