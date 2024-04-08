#include <stdlib.h>
#include <stdio.h>
#include "list.h"

struct List {
    int len;
    int cap;
    void **arr;
};

List *list_create(int cap) {
    List *l = (List*)malloc(sizeof(List));
    if (l == NULL) {
        printf("Memory allocation failed\n");
        exit(-1);
    }
    l->arr = malloc(cap * sizeof(void*));
    if (l->arr == NULL) {
        printf("Memory allocation failed\n");
        free(l);
        exit(-1);
    }
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
        if (l->arr == NULL) {
            printf("Memory allocation failed\n");
            exit(-1);
        }
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
