const std = @import("std");
const net = @import("std.net");

pub fn main() !void {
    var server = std.http.Server.init(.{ .reuse_address = true });
    defer server.deinit();

    const port = 3678;
    const bind_addr = net.Address.parseIp4("127.0.0.1", port);
    try server.listen(bind_addr);
}

