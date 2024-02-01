#pragma once

#include <cstddef>

namespace options {
    constexpr size_t max_argc = 10;

    void parse(int argc, char* argv[]);

    bool get_default();
    bool get_show();

    static bool _default = true; 
    static bool _show = false;
}