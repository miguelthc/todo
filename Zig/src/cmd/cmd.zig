const std = @import("std");
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
    try stdout.print(">", .{});

    var line_buf: [22]u8 = undefined;
    const bytes_read = try stdin.read(&line_buf);
    if (bytes_read == line_buf.len) {
        try stdout.print("Input too long\n", .{});

        // TODO there's probably a better way to do this
        if (line_buf[21] != '\n') {
            try stdin.skipUntilDelimiterOrEof('\n');
        }

        return;
    }

    const command_lws = line_buf[0..(bytes_read - 1)];
    command.* = std.mem.trimLeft(u8, command_lws, " ");
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
    // TODO
}

fn get_all() CmdError!void {
    // TODO
}

fn post() CmdError!void {
    // TODO
}

fn put() CmdError!void {
    // TODO
}

fn del() CmdError!void {
    // TODO
}
