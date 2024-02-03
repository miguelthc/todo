#include "../types/types.h"
#include "../../common/common.h"

typedef struct Node {
    Item item;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
    Node *tail;
} List;

error_t list_insert(Item *item);

error_t list_delete(size_t id);

error_t list_get(size_t id, Item *item);

error_t list_get_all(Node **head);

error_t list_update(size_t id, Item *item);