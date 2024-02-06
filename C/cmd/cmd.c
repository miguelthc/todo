#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmd.h"
#include "../data/data.h"

#define OP_MAX_LEN 4
#define ID_MAX_LEN 10
#define ITEM_TEXT_MAX_LEN 50
#define COMPLETED_MAX_LEN 5
#define TRUE "true"
#define FALSE "false"

/* Possible operations */
typedef struct {
    error_t (*execute)();
    char* op_s;
} op_i;

const op_i ops[] = {
    {&get_one_cmd, GET_ONE}, {&get_all_cmd, GET_ALL}, {&post_cmd, POST}, {&put_cmd, PUT}, 
    {&del_cmd, DELETE}, {&help, HELP}, {&quit, QUIT}
};

const op_i error_i = { &err, NULL };

/* - - - - - - - - - - - - - - */

/* main functions declarations */
op_i get_op();

int main(int argc, char *argv[]) {
    printf("\nCommand-line todo app\n");
    printf("type %s for help\n\n", HELP);

    while(true){
        printf("op> ");

        op_i op = get_op();
        error_t res = op.execute();
        if(res){ return res; }
    }

    return 0;
}

size_t get_input(char *in, size_t max_len) {
    char c;
    int i = 0;
    int j = 0;
    while((c = getchar()) != '\n' && c != EOF) {
        if(i < max_len){
            in[i++] = c;
        }
        j++;
    }
    in[i] = '\0';
    return j;
}

op_i get_op_i(size_t input_len, char* input){
    if(input_len > OP_MAX_LEN){ return error_i; }

    for(int i = 0; i < sizeof(ops)/sizeof(op_i); i++){
        if(strncasecmp(input, ops[i].op_s, input_len + 1) == 0){
            return ops[i];
        }
    }
    return error_i;
}

op_i get_op(){
    char cmd[OP_MAX_LEN + 1];
    size_t input_len = get_input(cmd, OP_MAX_LEN);
    return get_op_i(input_len, cmd);
}

void printItem(Item item){
    printf("****************\n");
    printf("Id: %zu\n", item.id);
    printf("Text: %s\n", item.text);
    printf("Completed: %s\n", item.completed ? "true" : "false");
    printf("****************\n");
}

error_t get_one_cmd(){
    printf("id: ");
    
    char id_s[ID_MAX_LEN + 1];
    get_input(id_s, ID_MAX_LEN);
    // NOTE: this can go wrong if size_t is defined as something smaller than unsigned long
    // NOTE: strtoul returns negative numbers interpreted as unsigned longs. weird...
    size_t id = strtoul(id_s, NULL, 10);
    if(id == 0) { printf("Invalid id\n"); return no_err; }

    Item item;
    error_t err = get_one(id, &item);
    if(!err){
        printItem(item);
        return no_err;
    }

    if(err == not_found_err){
        printf("Not found\n");
        return no_err;
    }else if(err == empty_err){
        printf("Empty list\n");
        return no_err;
    }

    return no_err;
}

error_t get_all_cmd(){
    Node *node = NULL;
    error_t err = get_all(&node);
    if(err == empty_err){ printf("No items in todo list\n"); return no_err; }

    do {
        printItem(node->item);
        node = node->next;
    }while(node != NULL);

    return no_err;
}

error_t post_cmd(){
    printf("text (%d char max.): ", ITEM_TEXT_MAX_LEN);

    char *text = malloc(sizeof(char) * ITEM_TEXT_MAX_LEN + 1);
    size_t written = get_input(text, ITEM_TEXT_MAX_LEN);
    if(written > ITEM_TEXT_MAX_LEN){ printf("Error: %d char max.\n", ITEM_TEXT_MAX_LEN); return no_err; }

    Item item;
    error_t err = post(text, &item);
    if(err){ printf("Unexpected err. Try again.\n"); return no_err; }
    
    printItem(item);

    return no_err;
} 

error_t put_cmd(){
    printf("id: ");
    
    char id_s[ID_MAX_LEN + 1];
    get_input(id_s, ID_MAX_LEN);
    // NOTE: this can go wrong if size_t is defined as something smaller than unsigned long
    // NOTE: strtoul returns negative numbers interpreted as unsigned longs. weird...
    size_t id = strtoul(id_s, NULL, 10);
    if(id == 0) { printf("Invalid id\n"); return no_err; }

    // NOTE this is very inneficient because the list is implemented as a linked-list;
    Item item;
    error_t err = get_one(id, &item);

    if(err == empty_err){ printf("Empty list\n"); return no_err; } // It's silly to only print this after asking for an id
    else if(err == not_found_err){ printf("Not found\n"); return no_err; }

    printf("text (%d char max.): ", ITEM_TEXT_MAX_LEN);
    char *text = malloc(sizeof(char) * ITEM_TEXT_MAX_LEN + 1);
    size_t written = get_input(text, ITEM_TEXT_MAX_LEN);
    if(written > ITEM_TEXT_MAX_LEN){ printf("Error: %d char max.\n", ITEM_TEXT_MAX_LEN); return no_err; }

    printf("completed (true or false): ");
    char *completed_t = malloc(sizeof(char) * COMPLETED_MAX_LEN + 1);
    written = get_input(completed_t, COMPLETED_MAX_LEN);
    if( written > COMPLETED_MAX_LEN){ printf("Error: true or false.\n"); return no_err; }

    bool completed = false;
    if(strcmp(completed_t, TRUE) == 0){
        completed = true;
    }else if(strcmp(completed_t, FALSE) != 0){
        printf("Error: true or false.\n"); return no_err;
    }

    err = put(id, text, completed, &item);
    if(err){ printf("Unexpected error. Try again."); }
    else{
        printItem(item);
    }

    return no_err;
}

error_t del_cmd(){
    printf("id: ");
    
    char id_s[ID_MAX_LEN + 1];
    get_input(id_s, ID_MAX_LEN);
    // NOTE: this can go wrong if size_t is defined as something smaller than unsigned long
    // NOTE: strtoul returns negative numbers interpreted as unsigned longs. weird...
    size_t id = strtoul(id_s, NULL, 10);
    if(id == 0) { printf("Invalid id\n"); return no_err; }

    error_t err = delete(id);

    if(err == not_found_err){
        printf("Not found\n");
        return no_err;
    }else if(err == empty_err){
        printf("Empty list\n");
        return no_err;
    }

    return no_err;
}

error_t help(){
    for(int i = 0; i < sizeof(help_menu)/sizeof(HelpItem); i++){
        printf("%-8s%s\n", help_menu[i].command, help_menu[i].desc);
    }
    return no_err;
}

error_t quit(){
    exit(0);
    return no_err;
}

error_t err(){
    printf("Unkown command; type \"help\" to see available commands\n");
    return no_err;
}