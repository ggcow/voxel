#ifndef LIST_H
#define LIST_H

#include "cube.h"

typedef struct plist_t * plist_t;
struct plist_t {
    void *value;
    plist_t next;
};

void plist_destroy(plist_t *list);

int plist_contains(plist_t list, void *pointer);
void plist_remove(plist_t *list, void *pointer);
void plist_add(plist_t *list, void *pointer);

#define plist_foreach(loop_list, loop_pointer, loop_list_type) \
for (plist_t _l = loop_list; _l; _l = _l->next) \
for (loop_list_type *loop_pointer = (loop_list_type *) _l->value; loop_pointer; loop_pointer = NULL)



#define list_type(type) struct list_##type

#define define_list_type(type) list_type(type)  { \
    type value;                                   \
    list_type(type) *next;                        \
}

define_list_type(cube_t);

#undef define_list_type

#define list_remove(list, remove) \
{                                  \
typeof(*list) last = NULL; \
typeof(*list) l = *list; \
for (typeof(l->value) v = l->value; l; last = l, l = l->next, v = l->value) { \
    if (!memcmp(&remove, &v, sizeof(typeof(l->value)))) { \
        if (last) { \
            last->next = l->next; \
            deallocate(l); \
        } else if (l->next) { \
            *list = l->next; \
            deallocate(l); \
        } else { \
            deallocate(*list); \
            *list = NULL; \
        } \
        break; \
    } \
}} \

#define list_add(list, v) \
{                          \
typeof(*list) _node = callocate(sizeof *_node); \
_node->value = v; \
if (*list) { \
    _node->next = *list;\
} \
*list = _node; \
}}

#define list_destroy(list) \
{                          \
typeof(*list) l = *list;                           \
for (typeof(l) next = *list; next; l = next) { \
    next = l->next; \
    deallocate(l); \
}} \
*list = NULL;

#define list_foreach(list, var) \
for (typeof(*list) _l = *list; _l; _l = _l->next) \
for (typeof(_l->value) var = (typeof(_l->value)) _l->value; iu_list_loop_check(1); iu_list_loop_check(0))

int iu_list_loop_check(int b);

#endif
