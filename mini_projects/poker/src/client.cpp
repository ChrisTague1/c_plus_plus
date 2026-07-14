#include <asio.hpp>
#include <iostream>
#include <string>

using asio::ip::tcp;

int main() {
    try {
        asio::io_context io;

        tcp::resolver resolver(io);
        auto endpoints = resolver.resolve("127.0.0.1", "9090");

        tcp::socket socket(io);
        asio::connect(socket, endpoints);

        // Read welcome message
        char buf[1024];
        size_t n = socket.read_some(asio::buffer(buf));
        std::cout << std::string(buf, n);

        // Send messages from stdin
        std::string line;
        while (std::cout << "> " && std::getline(std::cin, line)) {
            line += "\n";
            asio::write(socket, asio::buffer(line));

            asio::error_code ec;
            n = socket.read_some(asio::buffer(buf), ec);
            if (ec) break;

            std::cout << std::string(buf, n);
        }
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}
