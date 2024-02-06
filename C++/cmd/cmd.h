#pragma once

#include "../data/data.h"
#include "../data/store/store.h"

#include <string_view>
#include <array>
#include <functional>

namespace todo_cmd {
    using store_t = std::shared_ptr<store::Data_Store<todo_data::todo>>;

    void help(store_t);
    void get_one(store_t);
    void get_all(store_t);
    void post(store_t);
    void put(store_t);
    void del(store_t);

    void printItem(todo_data::todo);

    struct op {
        std::string_view op_text;
        std::string_view op_description;
        void (*op_f)(store_t);
    };

    constexpr op HELP ={ "help", "Print (this) help menu", help };
    constexpr op GET_ONE ={ "get", "Get one todo item by id", get_one };
    constexpr op GET_ALL ={ "all", "Get all todo items", get_all };
    constexpr op POST ={ "post", "Create one todo item", post };
    constexpr op PUT ={ "put", "Update the todo item with the given id", put };
    constexpr op DELETE ={ "del", "Delete the todo item with the given id", del };
    constexpr std::array<op, 6> ops = { GET_ONE, GET_ALL, POST, PUT, DELETE, HELP };

    constexpr std::string_view EXIT = "exit";
    constexpr std::string_view EXIT_DESC = "Exit program execution";

    constexpr std::string_view UNKNOWN_MSG = "Unkown command; type \"help\" to see available commands";

    int run_cmd(store_t);
}