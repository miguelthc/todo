#include "options/options.h"
#include "cmd/cmd.h"
#include "data/data.h"
#include "data/store/store.h"
#include "data/store/mem/mem.h"

#include <iostream>
#include <stdexcept>
#include <optional>
#include <memory>


struct App_Components {
    std::shared_ptr<store::Data_Store<todo_data::todo>> todo_store;
};

App_Components init_app(){
    auto mem_store = std::make_shared<mem::Mem_Store<todo_data::todo>>();

    return App_Components{ mem_store };
}

int run_app(App_Components& components){
    return todo_cmd::run_cmd(components.todo_store);
}


int main(int argc, char* argv[]){

    try{
        options::parse(argc, argv);
    }catch(const std::exception &e){
        std::cerr << "todo: " << e.what() << "\n";
        std::cerr << "usage: todo \n";
        return EXIT_FAILURE;
    }

    if(options::get_default()){
        //TODO
        std::cout << "using default options: -cmd -mem\n";
    }

    std::optional<App_Components> components;

    try{
        components.emplace(init_app());
    }catch(const std::exception &e){
        std::cerr << "todo: could not launch app with given options\n";
        std::cerr << "error: " << e.what() << "\n";
        return EXIT_FAILURE;
    }

    // TODO abstract cmd as just a command parser - have an independent executor that can receive commands from different interfaces
    // TODO init structures

    return run_app(components.value());
}