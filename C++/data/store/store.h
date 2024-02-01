#pragma once

#include "../data.h"

#include <optional>
#include <memory>

namespace store {
    template<typename T>
    class Iterator {
        public:
            virtual const T& next() = 0;
            virtual bool hasNext() const = 0;
    };

    template<todo_data::Entity T>
    class Data_Store {
        public:
            virtual std::optional<T> get_one(unsigned long& id) const = 0;
            virtual std::shared_ptr<Iterator<T>> get_all() const = 0;
            virtual unsigned long post(T& item) = 0;
            virtual std::optional<T> put(unsigned long& id, T& item) = 0;
            virtual std::optional<T> del(unsigned long& id) = 0;
    };
}