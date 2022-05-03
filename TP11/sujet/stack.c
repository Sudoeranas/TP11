#include <stdlib.h>

#include "stack.h"

dstack_t *make_stack(size_t maxelts) {
  dstack_t *s;
  if (!(s = malloc(sizeof(dstack_t))))
    return NULL;
  if (!(s->data = malloc(maxelts * sizeof(stack_element_t))))
    return NULL;
  s->next = s->data;
  s->past_one = s->data + maxelts;
  return s;
}
void free_stack(dstack_t *s) {
  free(s->data);
  free(s);
}

bool is_empty_stack(dstack_t *s) {
  return s->next == s->data;
}

stack_element_t *pop(dstack_t *s) {
  if (is_empty_stack(s))
    return NULL;
  return --(s->next);
}

void push(dstack_t *s, stack_element_t e) {
  *(s->next++) = e;
}
