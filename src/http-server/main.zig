const std = @import("std");
const net = @import("std").net;
const print = @import("std").debug.print;

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    const server_allocator = gpa.allocator();
    var server = std.http.Server.init(server_allocator, .{ .reuse_address = true });
    defer server.deinit();

    const port = 3678;
    const bind_addr = try net.Address.parseIp4("127.0.0.1", port);
    try server.listen(bind_addr);
    print("[INFO]: HTTP Server listening at port {}\n", .{port});

    while (true) {
        var response = try server.accept(.{.allocator = server_allocator});
        defer response.deinit();

        while (response.reset() != .closing) {
            response.wait() catch |err| switch (err) {
                error.HttpHeadersInvalid => break,
                error.HttpHeadersExceededSizeLimit => {
                    response.status = .request_header_fields_too_large;
                    response.do() catch break;
                    break;
                },
                else => {
                    response.status = .bad_request;
                    response.do() catch break;
                    break;
                },
            };

            response.status = .ok;
            response.transfer_encoding = .chunked;

            try response.do();
            try response.writeAll("Hello, World!\n");
            try response.finish();

        }

        break;
    }
}

