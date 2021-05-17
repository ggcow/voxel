#include "list.h"

void plist_destroy(plist_t *list) {
    //TODO FIX MEMORY LEAK
    plist_t next = *list;
    for (; next; *list = next) {
        next = (*list)->next;
        deallocate(*list);
    }
    *list = NULL;
}

bool plist_contains(plist_t list, void *pointer) {
    plist_foreach(list, p, void) {
        if (p == pointer) {
            return TRUE;
        }
    }
    return FALSE;
}

void plist_remove(plist_t *list, void *pointer) {
    plist_t last = NULL;
    plist_t l = *list;
    for (void *p = l->value; l; last = l, l = l->next, p = l->value) {
        if (p == pointer) {
            if (last) {
                last->next = l->next;
                deallocate(l);
            } else if (l->next) {
                *list = l->next;
                deallocate(l);
            } else {
                deallocate(*list);
                *list = NULL;
            }
            break;
        }
    }
}

void plist_add(plist_t *list, void *pointer) {
    plist_t new = allocate(sizeof *new);
    *new = (struct plist_t) {pointer};
    if (*list) {
        plist_t l = *list;
        for (; l->next; l = l->next);
        l->next = new;
    } else {
        *list = new;
    }
}