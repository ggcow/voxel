#include "list.h"

plist_t plist_create(void *pointer) {
    plist_t list = allocate(sizeof *list);
    *list = (struct plist_tt) {pointer};
    return list;
}

void plist_destroy(plist_t list) {
    plist_t last;
    for (last = list; list; last = list, list = list->next) {
        if (last != list)
        deallocate(last);
    }
    if (!last)
    deallocate(last);
}

bool plist_contains(plist_t list, void *pointer) {
    plist_foreach(list, p, void) {
        if (p == pointer) {
            return TRUE;
        }
    }
    return FALSE;
}

void plist_remove(plist_t list, void *pointer) {
    plist_t last = list;
    for (void *p = list->value; list; last = list, list = list->next) {
        if (p == pointer) {
            if (last) {
                last->next = list->next;
            } else {
                *list = *list->next;
            }
            break;
        }
    }
}

void plist_add(plist_t list, void *pointer) {
    for (; list->next; list = list->next);
    plist_t new = allocate(sizeof *new);
    new = (plist_t) {pointer};
    list->next = new;
}