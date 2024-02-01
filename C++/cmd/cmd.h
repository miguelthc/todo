#pragma once

#include "../data/data.h"
#include "../data/store/store.h"

#include <string_view>
#include <array>
#include <functional>

namespace todo_cmd {
    void help(std::shared_ptr<store::Data_Store<todo_data::todo>>);
    void get_one(std::shared_ptr<store::Data_Store<todo_data::todo>>);
    void get_all(std::shared_ptr<store::Data_Store<todo_data::todo>>);
    void post(std::shared_ptr<store::Data_Store<todo_data::todo>>);
    void put(std::shared_ptr<store::Data_Store<todo_data::todo>>);
    void del(std::shared_ptr<store::Data_Store<todo_data::todo>>);

    void printItem(todo_data::todo);

    struct op {
        std::string_view op_text;
        std::string_view op_description;
        void (*op_f)(std::shared_ptr<store::Data_Store<todo_data::todo>>);
    };

    constexpr op HELP ={ "help", "print help menu", help };
    constexpr op GET_ONE ={ "get", "get todo item by id", get_one };
    constexpr op GET_ALL ={ "all", "get all todo items", get_all };
    constexpr op POST ={ "post", "create todo item", post };
    constexpr op PUT ={ "put", "update todo item with given id", put };
    constexpr op DELETE ={ "del", "delete the todo item with given id", del };
    constexpr std::array<op, 6> ops = { GET_ONE, GET_ALL, POST, PUT, DELETE, HELP };

    constexpr std::string_view EXIT = "exit";
    constexpr std::string_view EXIT_DESC = "exit programm execution";

    constexpr std::string_view UNKNOWN_MSG = "Unkown command; type \"help\" to see available commands";

    int run_cmd(std::shared_ptr<store::Data_Store<todo_data::todo>>);
}