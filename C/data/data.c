#include "data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

error_t get_one(size_t id, Item *item){
    return list_get(id, item);
}

error_t get_all(Node **node){
    error_t err = list_get_all(node);
    return err;
}

error_t post(char *text, Item *item){
    // NOTE: it would be more effiecient to do the malloc in cmd and not copy the string here,
    // but this is just a SoC thing
    char *text_in = malloc(sizeof(char) * strlen(text) + 1);
    strcpy(text_in, text);

    item->text = text_in;
    item->completed = false;
    error_t err = list_insert(item);
    item->text = text; // Make text_in reference unavailable to caller. There is probably a less silly way to do it
    if(err){ ; return unexpected_err; }
    return no_err;
}

error_t delete(size_t id){
    return list_delete(id);
}

error_t put(size_t id, char* text, bool completed, Item *item){
    char *text_in = NULL; 
    bool has_text = *text != '\0';

    if(has_text) {
        text_in = malloc(sizeof(char) * strlen(text) + 1);
        strcpy(text_in, text);
    }

    item->text = text_in;
    item->completed = completed;
    error_t err = list_update(id, item);
    if(!err && !has_text){
        text = malloc(sizeof(char) * strlen(item->text) + 1);
        strcpy(text, item->text);
    }

    item->text = text; // same as post()
    return err;
}