const std = @import("std");
const data = @import("../data/data.zig");
const mem = @import("../data/mem.zig");

const CmdError = error{Print};

const op_i = struct {
    op_text: []const u8,
    op_desc: []const u8,
    op_fn: *const fn () CmdError!void,
};

const ops = [_]op_i{
    .{ .op_text = "help", .op_desc = "Print (this) help menu", .op_fn = &help },
    .{ .op_text = "exit", .op_desc = "Exit program execution", .op_fn = &exit },
    .{ .op_text = "get", .op_desc = "Get one todo item by id", .op_fn = &get_one },
    .{ .op_text = "all", .op_desc = "Get all todo items", .op_fn = &get_all },
    .{ .op_text = "post", .op_desc = "Create one todo item", .op_fn = &post },
    .{ .op_text = "put", .op_desc = "Update the todo item with the given id", .op_fn = &put },
    .{ .op_text = "del", .op_desc = "Delete the todo item with the given id", .op_fn = &del },
};

fn ops_max_length() comptime_int {
    var max_len: comptime_int = 0;
    for (ops) |op| {
        if (op.op_text.len > max_len) {
            max_len = op.op_text.len;
        }
    }
    return max_len;
}

// TODO use buffered readers and writers?
const stdout = std.io.getStdOut().writer();
const stdin = std.io.getStdIn().reader();

pub fn run_app() !void {
    outer: while (true) {
        var command: []const u8 = undefined;
        try get_command(&command);

        for (ops) |op| {
            if (std.mem.eql(u8, op.op_text, command)) {
                try op.op_fn();
                continue :outer;
            }
        }

        try stdout.print("Unkown command; type \"help\" to see available commands\n", .{});
    }
}

fn get_command(command: *[]const u8) !void {
    try stdout.print("> ", .{});

    const res = read_stdin_to_buffer(ops_max_length()) catch |err|
        switch (err) {
        error.BufferOverflow => {
            try stdout.print("Input too long\n", .{});
            return;
        },
        else => |e| return e,
    };

    command.* = res.buf[0..res.bytes_read];
    command.* = std.mem.trimLeft(u8, command.*, " ");
}

fn help() CmdError!void {
    //              123456789012
    const max_ws = "            ";

    stdout.print("\n", .{}) catch {
        return error.Print;
    };

    for (ops) |op| {
        const ws = max_ws[0..(max_ws.len - op.op_text.len)];
        stdout.print("{s}{s}{s}\n", .{ op.op_text, ws, op.op_desc }) catch {
            return error.Print;
        };
    }

    stdout.print("\n", .{}) catch {
        return error.Print;
    };
}

fn exit() CmdError!void {
    std.process.exit(0);
}

fn get_one() CmdError!void {
    stdout.print("id: ", .{}) catch {
        return CmdError.Print;
    };

    // TODO parse id

    //mem.get_one(id: u64)

    //print_item(item: mem.todo_item)
}

fn get_all() CmdError!void {
    // TODO
}

fn post() CmdError!void {
    stdout.print("text: ", .{}) catch {
        return CmdError.Print;
    };

    // TODO
}

fn put() CmdError!void {
    // TODO
}

fn del() CmdError!void {
    // TODO
}

fn print_item(item: data.todo_item) !void {
    try stdout.println("****************", .{});
    try stdout.println("Id: {}", .{item.id});
    try stdout.println("Text: {s}", .{item.text});
    try stdout.println("Completed: {s}", .{if(item.completed) "true" else "false"});
    try stdout.println("****************");
}

fn read_stdin_to_buffer(comptime max_length: comptime_int) !struct { buf: [max_length]u8, bytes_read: usize } {
    var line_buf: [max_length + 2]u8 = undefined;
    const bytes_read = try stdin.read(&line_buf);
    if (bytes_read == line_buf.len) {
        // TODO there's probably a better way to do this
        if (line_buf[max_length + 1] != '\n') {
            try stdin.skipUntilDelimiterOrEof('\n');
        }

        return error.BufferOverflow;
    }

    return .{ .buf = line_buf[0..max_length].*, .bytes_read = bytes_read - 1 };
}
