#ifndef LIST_H
#define LIST_H

#include "common.h"

typedef struct plist_tt * plist_t;
struct plist_tt {
    void *value;
    plist_t next;
};



plist_t plist_create(void *pointer);
void plist_destroy(plist_t list);

bool plist_contains(plist_t list, void *pointer);
void plist_remove(plist_t list, void *pointer);
void plist_add(plist_t list, void *pointer);

#define plist_foreach(l,p,t) \
for (plist_t _l = l; _l; _l = _l->next) \
for (t *p = (t *) _l->value; p; p = NULL)

#endif
