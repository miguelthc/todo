#pragma once

#include <string>
#include <concepts>

namespace todo_data {
    template <typename T>
    concept Entity = std::same_as<decltype(std::declval<T>().id), unsigned long>;

    class todo {
        public:
            unsigned long id;
            std::string text;
            bool completed;
    };
}