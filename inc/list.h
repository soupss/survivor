#ifndef LIST_H
#define LIST_H

typedef struct List List;
List *list_create(int cap);
void list_free(List *l);
void *list_get(List *l, int i);
int list_len(List *l);
void list_insert(List *l, void *el);
void *list_delete(List *l, int i);

#endif
