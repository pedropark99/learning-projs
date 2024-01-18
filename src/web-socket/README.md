# Web Sockets in C

This folder contain some very simple examples I developed of Web Sockets in C.
Here, I am using web sockets as a very basic HTTP server (i.e. a server that responds to income connections with HTTP responses).

This small project uses CMake to build, so, if ou want to build this project in
our machine, run these commands:

```bash
cmake .
make
# Then call the executable
```

After calling the executable, a new socket will be allocated, and it will
wait, listening for an incoming connection. You can connect to this socket
by visiting the URL `localhost:3490` in your browser.