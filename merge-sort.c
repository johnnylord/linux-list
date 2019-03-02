#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "list.h"

static uint16_t values[20];

static void list_merge(struct list_head *left,
                       struct list_head *right,
                       struct list_head *head)
{
    struct list_head *l = left->next;
    struct list_head *l_safe = l->next;
    struct list_head *r = right->next;
    struct list_head *r_safe = r->next;
    struct listitem *item;

    INIT_LIST_HEAD(head);

    while (!list_empty(left) && !list_empty(right)) {
        struct listitem *l_item = list_entry(l, struct listitem, list);
        struct listitem *r_item = list_entry(r, struct listitem, list);

        if (l_item->i < r_item->i) {
            list_del(l);
            list_add_tail(l, head);
            l = l_safe;
            l_safe = l_safe->next;
        } else {
            list_del(r);
            list_add_tail(r, head);
            r = r_safe;
            r_safe = r_safe->next;
        }
    }

    if (!list_empty(left))
        list_splice_tail(left, head);

    if (!list_empty(right))
        list_splice_tail(right, head);
}

static void list_mergesort(struct list_head *head)
{
    struct list_head left_list, right_list;
    struct list_head *f, *f_safe, *r, *r_safe;
    struct listitem *item = NULL;


    // Boundary condition
    if (list_is_singular(head))
        return;

    // Initialize two seperate list
    INIT_LIST_HEAD(&left_list);
    INIT_LIST_HEAD(&right_list);

    // Splice the head to two seperate list
    for (f = head->next, f_safe = f->next, r = head->prev, r_safe = r->prev;
         !list_empty(head);
         f = f_safe, r = r_safe, f_safe = f_safe->next, r_safe = r_safe->prev) {
        // ... ele [f, r] ele ...
        if (f == r) {
            list_del(f);
            list_add_tail(f, &left_list);
            break;
        }

        // ... ele [f] [r] ele ...
        list_del(f);
        list_add_tail(f, &left_list);
        list_del(r);
        list_add(r, &right_list);
    }

    // Process left_list and right_list
    list_mergesort(&left_list);
    list_mergesort(&right_list);

    // Merge left_list and right_list
    list_merge(&left_list, &right_list, head);
}

int main(void)
{
    struct list_head testlist;
    struct listitem *item, *safe;
    size_t i;

    // Initialize the values array
    random_shuffle_array(values, (uint16_t) ARRAY_SIZE(values));

    // Initialize testlist
    INIT_LIST_HEAD(&testlist);

    assert(list_empty(&testlist));

    // Assign each value in the array values to its associate list element
    for (i = 0; i < ARRAY_SIZE(values); i++) {
        item = (struct listitem *) malloc(sizeof(*item));
        assert(item);
        item->i = values[i];
        list_add_tail(&item->list, &testlist);
    }

    assert(!list_empty(&testlist));

    // Sort the testing group data, testlist, in ascending order
    list_mergesort(&testlist);

    // Printout the sorted list
    printf("Sorted:");
    list_for_each_entry_safe (item, safe, &testlist, list) {
        printf("%d ", (int) item->i);
    }
    printf("\n");

    return 0;
}
