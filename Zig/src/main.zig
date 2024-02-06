const std = @import("std");
const cmd = @import("cmd/cmd.zig");

pub fn main() !void {

    // TODO parse command line args

    try cmd.run_app();
}
