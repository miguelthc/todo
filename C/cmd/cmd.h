#include "../common/common.h"

/**
 *  Input and help consts
 */

#define GET_ONE "get"
#define GET_ALL "all"
#define POST "post"
#define PUT "put"
#define DELETE "del"
#define HELP "help"
#define QUIT "exit"

#define GET_ONE_DESC "Get one todo item by id"
#define GET_ALL_DESC "Get all todo items"
#define POST_DESC "Create one todo item"
#define PUT_DESC "Update the todo item with the given id"
#define DELETE_DESC "Delete the todo item with the given id"
#define HELP_DESC "Print (this) help menu"
#define QUIT_DESC "Exit program execution"

typedef struct {
    char* command;
    char* desc;
} HelpItem;

const HelpItem help_menu[] = {
    {GET_ONE, GET_ONE_DESC}, {GET_ALL, GET_ALL_DESC}, {POST, POST_DESC}, {PUT, PUT_DESC},
    {DELETE, DELETE_DESC}, {HELP, HELP_DESC}, {QUIT, QUIT_DESC}
};

/**
 *  Ops 
 */

error_t get_one_cmd();
error_t get_all_cmd();
error_t post_cmd();
error_t put_cmd();
error_t del_cmd();
error_t help();
error_t quit();
error_t err();