#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    size_t id;
    bool completed;
    char* text;
} Item;