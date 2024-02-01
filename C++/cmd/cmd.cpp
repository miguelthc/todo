#include "cmd.h"

#include <string>
#include <iostream>
#include <iomanip>
#include <optional>
#include <limits>

using namespace todo_cmd;

int todo_cmd::run_cmd(std::shared_ptr<store::Data_Store<todo_data::todo>> store){
    std::string op_in;
    
    start_loop:
        std::cout << ">";
        // TODO pass input after op as variadic args to op_f
        std::getline(std::cin, op_in);
        for(auto op: ops){
            if(op_in == op.op_text){
                op.op_f(store);
                goto start_loop;
            }
        }

        if(op_in != EXIT){
            std::cout << UNKNOWN_MSG << "\n";
            goto start_loop;
        }
        
    return 0;
}

void todo_cmd::printItem(todo_data::todo item){
    std::cout << std::left;
    std::cout << "****************\n";
    std::cout << std::setw(12) << "id: " << item.id << "\n";
    std::cout << std::setw(12) << "text: " << item.text << "\n";
    std::cout << std::setw(12) << "completed: " << item.completed << "\n";
    std::cout << "****************\n";
}

void todo_cmd::help(std::shared_ptr<store::Data_Store<todo_data::todo>> store){
    std::cout << "\n";

    std::cout << std::left;
    for(auto op: ops){
        std::cout << std::setw(8) << op.op_text <<  op.op_description << "\n";
    }
    std::cout << std::setw(8) << EXIT << EXIT_DESC << "\n\n";
}


// TODO store methods might throw? Handle here?


void todo_cmd::get_one(std::shared_ptr<store::Data_Store<todo_data::todo>> store){
    std::cout << "id: ";
    unsigned long id;
    // TODO should only accept one?
    std::cin >> id;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    auto item = store->get_one(id);
    if(item.has_value()){
        // TODO this is kinda ugly
        item.value().id = id;
        printItem(item.value());
    }else{
        std::cout << "Item does not exist\n";
    }
}

void todo_cmd::get_all(std::shared_ptr<store::Data_Store<todo_data::todo>> store){
    auto it = store->get_all();
    
    if(!it->hasNext()){
        std::cout << "No todo items... yet\n";
    }

    while(it->hasNext()){
        printItem(it->next());
    }
}

void todo_cmd::post(std::shared_ptr<store::Data_Store<todo_data::todo>> store){
    std::cout << "text: ";
    std::string text;
    std::getline(std::cin, text, '\n');

    todo_data::todo new_todo{0, text, false};
    unsigned long id = store->post(new_todo);

    new_todo.id = id;
    printItem(new_todo);
}

void todo_cmd::put(std::shared_ptr<store::Data_Store<todo_data::todo>> store){
    std::cout << "id: ";
    unsigned long id;
    // TODO should only accept one?
    std::cin >> id;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    auto item = store->get_one(id);
    if(!item.has_value()){
        std::cout << "Item does not exist\n";
    }else{
        printItem(item.value());
        std::cout << "text [" << item.value().text << "] : ";
        std::string text;
        std::getline(std::cin, text, '\n');

        if(text.length() != 0){
            item.value().text = text;
        }

        bool completedSet = false;
        bool input;
        do{
            std::cout << "completed [" << item.value().completed << "] : ";
            std::string completed_s;
            std::getline(std::cin, completed_s, '\n');

            if(completed_s.length() == 0){ completedSet = true; input = item.value().completed; }
            else if( completed_s == "true" || completed_s == "1" ) { completedSet = true; input = true; }
            else if( completed_s == "false" || completed_s == "0" ) { completedSet = true; input = false; }
        }while(!completedSet);

        item.value().completed = input;
        
        printItem(item.value());

        store->put(id, item.value());
    }
}

void todo_cmd::del(std::shared_ptr<store::Data_Store<todo_data::todo>> store){
    std::cout << "id: ";
    unsigned long id;
    // TODO should only accept one?
    std::cin >> id;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if(store->del(id) != std::nullopt){
        std::cout << "Deleted item successfully\n";
    }else{
        std::cout << "Item does not exist\n";
    }
}