#include "socket.hh"
#include "util.hh"

#include <cstdlib>
#include <iostream>


void get_URL(const std::string &host, const std::string &path) {
    // Your code here.

    // You will need to connect to the "http" service on
    // the computer whose name is in the "host" string,
    // then request the URL path given in the "path" string.

    // Then you'll need to print out everything the server sends back,
    // (not just one call to read() -- everything) until you reach
    // the "eof" (end of file).
    TCPSocket sock{};
    sock.connect(Address(host,"http"));
    sock.write("GET "+path+" HTTP/1.1\r\nHost: "+host+"\r\n\r\n");
    sock.shutdown(SHUT_WR);
    while(!sock.eof()){
        std::cout<<sock.read();
    }
    sock.close();
    return;
    std::cerr << "Function called: get_URL(" << host << ", " << path << ").\n";
    std::cerr << "Warning: get_URL() has not been implemented yet.\n";
}

int main(int argc, char *argv[]) {
    try {
        if (argc <= 0) {
            abort();  // For sticklers: don't try to access argv[0] if argc <= 0.
        }

        // The program takes two command-line arguments: the hostname and "path" part of the URL.
        // Print the usage message unless there are these two arguments (plus the program name
        // itself, so arg count = 3 in total).
        if (argc != 3) {
            std::cerr << "Usage: " << argv[0] << " HOST PATH\n";
            std::cerr << "\tExample: " << argv[0] << " stanford.edu /class/cs144\n";
            return EXIT_FAILURE;
        }

        // Get the command-line arguments.
        const std::string host = argv[1];
        const std::string path = argv[2];

        // Call the student-written function.
        get_URL(host, path);
    } catch (const std::exception &e) {
        std::cerr << e.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
