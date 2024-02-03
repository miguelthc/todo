#include "list.h"
#include <string.h>

static List list = {NULL, NULL};

// NOTE implementing this as a list is obviously very bad - reads and writes are O(n).
// Something like a hash-map or a pointer array would be a lot better

error_t list_insert(Item *item){
    Node *newNode = malloc(sizeof(Node));
    if(newNode == NULL){ return malloc_err; }

    newNode->item = *item;
    newNode->next = NULL;

    size_t nextId = 1;

    if(list.head == NULL){
        list.head = newNode;
    }else{
        nextId = list.tail->item.id + 1;
        list.tail->next = newNode;
    }
    
    newNode->item.id = nextId;
    item->id = nextId;

    list.tail = newNode;

    return no_err;
}

error_t get_node(size_t id, Node **node_r){
    if(list.head == NULL){ return empty_err; }
    
    Node *node = list.head;
    while(node != NULL) {
        if(node->item.id == id){
            *node_r = node;
            return no_err;
        }
        node = node->next;
    }
    
    return not_found_err;
}

error_t list_delete(size_t id){
    if(list.head == NULL){ return empty_err; }
    
    Node *previous = NULL;
    Node *node = list.head;
    while(node != NULL) {
        if(node->item.id == id){
            if(previous != NULL){
                previous->next = node->next;
            }else{
                list.head = node->next;
            }
            free(node->item.text);
            free(node);
            return no_err;
        }
        previous = node;
        node = node->next;
    }
    
    return not_found_err;
} 

error_t list_get(size_t id, Item *item){
    Node *node;
    error_t err = get_node(id, &node);
    if(err){ return err; }

    *item = node->item; // returning text pointer to cmd... "unsafe"
    return no_err;
}

// NOTE not really "safe". Gives access to actual list. Copying would be a inneficient? 
// How to show reference without allowing modification?
error_t list_get_all(Node **head){
    if(list.head == NULL){ return empty_err; }
    *head = list.head;

    return no_err;
}

error_t list_update(size_t id, Item *item){
    Node *node;
    error_t err = get_node(id, &node);
    if(err){ return err; }

    if(item->text != NULL){
        free(node->item.text);
        node->item.text = item->text;
    }
    node->item.completed = item->completed;
    *item = node->item;

    return no_err;
}