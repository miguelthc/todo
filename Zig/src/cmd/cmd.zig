const std = @import("std");

pub fn run_app() void {
    while (true) {
        // TODO use buffered readers and writers?
        const stdout = std.io.getStdOut().writer();
        const stdin = std.io.getStdIn().reader();

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

        // TODO parse commands
    }
}
