#include <stdlib.h>
#include <stdio.h>
#include "list.h"
#include "util.h"

struct List {
    int len;
    int cap;
    void **arr;
};

List *list_create(int cap) {
    List *l = malloc(sizeof(List));
    check_alloc(l);
    l->arr = malloc(cap * sizeof(void*));
    check_alloc(l->arr);
    l->len = 0;
    l->cap = cap;
    return l;
}

void list_destroy(List *l) {
    for (int i = 0; i < l->len; i++) free(l->arr[i]);
    free(l->arr);
    free(l);
}

void *list_get(List *l, int i) { return l->arr[i]; }

int list_len(List *l) { return l->len; }

void list_insert(List *l, void *el) {
    if (l->len == l->cap) {
        l->cap *= 2;
        l->arr = realloc(l->arr, l->cap * sizeof(void*));
        check_alloc(l->arr);
    }
    l->arr[l->len] = el;
    l->len++;
}

void *list_delete(List *l, int i) {
    //TODO: reduce arr
    if (i < 0 || i >= l->len) {
        printf("Invalid index, no element deleted");
        return NULL;
    }
    void *el = l->arr[i];
    int end = l->len - 1;
    if (i != end) {
        l->arr[i] = l->arr[end];
    }
    l->len--;
    return el;
}
