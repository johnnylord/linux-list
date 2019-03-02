#include <stdio.h>

#define offsetof(TYPE, MEMBER) ((size_t) & ((TYPE *) 0)->MEMBER)

struct list_head {
    struct list_head *next, *prev;
};

struct A {
    char c[24];
    struct list_head list;
};

int main()
{
    printf("Offset from list elment's address to struct A's address: %d\n",
           (int) offsetof(struct A, list));
    return 0;
}
