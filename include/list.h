#ifndef LIST_H
#define LIST_H

#include "common.h"

typedef struct plist_t * plist_t;
struct plist_t {
    void *value;
    plist_t next;
};



void plist_destroy(plist_t *list);

bool plist_contains(plist_t list, void *pointer);
void plist_remove(plist_t *list, void *pointer);
void plist_add(plist_t *list, void *pointer);

#define plist_foreach(loop_list, loop_pointer, loop_list_type) \
for (plist_t _l = loop_list; _l; _l = _l->next) \
for (loop_list_type *loop_pointer = (loop_list_type *) _l->value; loop_pointer; loop_pointer = NULL)

#endif
