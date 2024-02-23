const std = @import("std");
const data = @import("data.zig");

const allocator = (std.heap.GeneralPurposeAllocator(.{}){}).allocator();
const map = std.AutoHashMap(u64, data.todo_item).init(allocator);
var id_counter: u64 = 0;

pub fn get_one(id: u64) ?data.todo_item {
    return map.get(id);
}

pub fn get_all() map.ValueIterator {
    return map.valueIterator();
}

pub fn post(item: data.todo_item) !u64 {
    id_counter += 1;
    try map.put(id_counter, item);
}

pub fn put() void {
    // TODO
}

pub fn del() void {
    // TODO
}
