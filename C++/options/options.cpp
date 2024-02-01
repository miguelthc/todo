#include "options.h"

#include <string>
#include <string_view>
#include <vector>
#include <stdexcept>
#include <iostream>

using namespace options;

void options::parse(int argc, char* argv[]) {
    const std::vector<std::string_view> args(argv+1, argv+argc);

    if(argc > max_argc){
        throw std::runtime_error("Number of arguments should not exceed " + std::to_string(max_argc));
    }

    for(const auto& arg: args){
        // TODO
        if(arg == "-show"){
            _show = true;
        }else{
            throw std::runtime_error("Unkown option: " + std::string(arg));
        }
    }
}

bool options::get_show() {
    return _show;
}

bool options::get_default(){
    return _default;
}