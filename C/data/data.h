#include "../common/common.h"
#include "list/list.h"

#include <stdlib.h>
#include <stdbool.h>

error_t get_one(size_t id, Item *item);

error_t get_all(Node **node);

error_t post(char* text, Item *item);

error_t delete(size_t id);

error_t put(size_t id, char* text, bool completed, Item *item);