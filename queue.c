#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

void show_queue(queue_t *q)
{
    printf("\033[31m====    print start queue   ====\033[0m\n");
    if (!q)
        return;
    list_ele_t *tmp = q->head;
    while (tmp) {
        printf("\033[32mtmp -> value : %s\033[0m\n", tmp->value);
        tmp = tmp->next;
    }
    printf("\033[31m====    print end queue   ====\033[0m\n");
}

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new(void)
{
    queue_t *q = malloc(sizeof(queue_t));
    /* TODO: What if malloc returned NULL? */
    if (!q)
        return q;
    q->head = q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    /* TODO: How about freeing the list elements and the strings? */
    /* Free queue structure */
    if (!q)
        return;
    // show_queue(q);
    list_ele_t *tmp = q->head;
    while (tmp) {
        list_ele_t *ptr = tmp;
        free((tmp->value));
        tmp = tmp->next;
        free(ptr);
    }
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    /* TODO: What should you do if the q is NULL? */
    if (!q)
        return false;

    list_ele_t *newh = malloc(sizeof(list_ele_t));
    if (!newh) {
        free(newh);
        return false;
    }

    newh->value = strdup(s);
    if (!newh->value) {
        free(newh);
        return false;
    }
    newh->next = q->head;

    q->head = newh;
    if (!q_size(q))
        q->tail = newh;
    /* Don't forget to allocate space for the string and copy it */
    ++q->size;
    // show_queue(q);
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    /* TODO: You need to write the complete code for this function */
    /* Remember: It should operate in O(1) time */
    /* TODO: Remove the above comment when you are about to implement. */
    if (!q)
        return false;

    if (!q_size(q)) {
        q_insert_head(q, s);
        return true;
    }

    list_ele_t *newt = malloc(sizeof(list_ele_t));
    if (!newt) {
        free(newt);
        return false;
    }

    newt->value = strdup(s);
    if (!newt->value) {
        free(newt);
        return false;
    }
    newt->next = NULL;

    q->tail->next = newt;
    q->tail = newt;

    ++q->size;
    // show_queue(q);
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    /* TODO: You need to fix up this code. */
    /* TODO: Remove the above comment when you are about to implement. */
    if (!q || !q->head)
        return false;

    if (bufsize > 0 && sp) {
        strncpy(sp, q->head->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }

    list_ele_t *tmp = q->head;
    q->head = tmp->next;
    --q->size;
    if (!q_size(q))
        q->tail = NULL;
    // show_queue(q);
    free(tmp->value);
    free(tmp);
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    /* TODO: You need to write the code for this function */
    /* Remember: It should operate in O(1) time */
    /* TODO: Remove the above comment when you are about to implement. */
    return (q == NULL) ? 0 : q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    /* TODO: You need to write the code for this function */
    /* TODO: Remove the above comment when you are about to implement. */

    if (!q || !q->head)
        return;

    list_ele_t *prev = NULL, *curr = q->head, *prec = q->head->next;

    while (prec) {
        curr->next = prev;
        prev = curr;
        curr = prec;
        prec = prec->next;
    }
    curr->next = prev;

    q->tail = q->head;
    q->head = curr;
    // show_queue(q);
    return;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (!q || !q->head)
        return;

    list_ele_t *head_prev = NULL, *head = q->head, *head_prec = head->next;

    while (head->next) {
        list_ele_t *min_prev = head_prev, *min_curr = head,
                   *min_prec = head_prec;
        list_ele_t *prev = head, *curr = head->next;
        while (curr) {
            list_ele_t *prec = curr->next;
            if (strcasecmp(curr->value, min_curr->value) < 0) {
                min_prev = prev;
                min_curr = curr;
                min_prec = prec;
            }
            prev = curr;
            curr = prec;
            prec = prec->next;
        }

        if (!head_prev)
            q->head = min_curr;
        else
            head_prev->next = min_curr;

        if (!min_prec)
            q->tail = head;

        if (&min_curr->value == &head_prec->value)
            min_curr->next = head;
        else
            min_curr->next = head_prec;

        if (&head->value == &min_prev->value)
            head_prec = head;
        else
            min_prev->next = head;
        head->next = min_prec;

        head = min_curr;
        head_prev = head;
        head = head_prec;
        head_prec = head_prec->next;
    }
    return;
}
