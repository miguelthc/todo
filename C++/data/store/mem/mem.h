#pragma once

#include "../store.h"

#include <unordered_map>
#include <memory>
#include <optional>
#include <iostream>

using namespace store;

namespace mem {

    template<todo_data::Entity T>
    class Mem_Store;

    template<typename T>
    class Mem_Store_Iterator : public Iterator<T> {
        typedef typename std::unordered_map<unsigned long, T>::const_iterator const_mem_iterator;

        public:
            Mem_Store_Iterator(const Mem_Store<T>& store) : store(store) {}

            const T& next() override {
                if(!current.has_value()){
                    current.emplace(store.begin());
                }

                const_mem_iterator pair = current.value()++;
                return std::get<1>(*pair);
            }

            bool hasNext() const override {
                return current.has_value() ? current.value() != store.end() :  store.begin() != store.end();
            }
        private:
            std::optional<const_mem_iterator> current = std::nullopt;
            const Mem_Store<T>& store;
    };

    template<todo_data::Entity T>
    class Mem_Store : public Data_Store<T> {
        typedef typename std::unordered_map<unsigned long, T>::const_iterator const_mem_iterator;

        public:
            std::optional<T> get_one(unsigned long& id) const override {
                const_mem_iterator elem = map.find(id);
                return elem != map.end() ? std::optional<T>{std::get<1>(*elem)} : std::nullopt;
            }

            std::shared_ptr<Iterator<T>> get_all() const override {
                auto mem_it = std::make_shared<Mem_Store_Iterator<T>>(*this);
                return mem_it;
            }

            unsigned long post(T& item) override {
                // TODO why store the id twice?
                item.id = ++count;
                map.insert({count, item});
                return count;
            }

            std::optional<T> put(unsigned long& id, T& item) override {
                const_mem_iterator elem = map.find(id);

                if(elem == map.end()){
                    return std::nullopt;
                }
                //TODO id stored twice
                item.id = id;

                map.insert_or_assign(id, item);
                return std::optional<todo_data::todo>(item);
            }

            std::optional<T> del(unsigned long& id) override {
                const_mem_iterator elem = map.find(id);

                if(elem == map.end()){
                    return std::nullopt;
                }

                T item = std::get<1>(*elem);
                map.erase(elem);

                return std::optional<T>(item);
            }

            const_mem_iterator begin() const {
                return map.begin();
            }

            const_mem_iterator end() const {
                return map.end();
            }

        private:
            unsigned long count = 0;
            std::unordered_map<unsigned long, T> map;
    };
}