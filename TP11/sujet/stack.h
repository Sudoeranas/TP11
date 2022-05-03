#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include <stdbool.h>

typedef int pair_element_t;

typedef struct {
  pair_element_t x;
  pair_element_t y;
} pair_t;

pair_t pair(pair_element_t x, pair_element_t y);

typedef pair_t stack_element_t;

// Une type pile très basique
typedef struct {
  stack_element_t *data; // zone des données de la pile
  stack_element_t *next; // pointeur sur le prochain élément inutilisé
                         // (peut déborder data de 1 case si la pile
                         // est pleine)
  stack_element_t *past_one; // pointeur sur l'élément suivant le
                             // dernier élément de la pile
} dstack_t;

dstack_t *make_stack(size_t maxelts);
void free_stack(dstack_t *s);
bool is_empty_stack(dstack_t *s);
stack_element_t *pop(dstack_t *s);
void push(dstack_t *s, stack_element_t e);

#endif // STACK_H
