#include "list.h"

void plist_destroy(plist_t *list) {
    plist_t next;
    for (; *list; *list = next) {
        next = (*list)->next;
        deallocate(*list);
    }
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
            } else if (l->next) {
                *l = *l->next;
            } else {
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