#include <asio.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <mutex>

using asio::ip::tcp;

class Server {
public:
    Server(asio::io_context& io, unsigned short port)
        : acceptor_(io, tcp::endpoint(tcp::v4(), port)) {
        std::cout << "Server listening on port " << port << "\n";
    }

    void run() {
        while (true) {
            tcp::socket socket(acceptor_.get_executor());
            acceptor_.accept(socket);

            std::string addr = socket.remote_endpoint().address().to_string();
            std::cout << "Client connected: " << addr << "\n";

            std::thread([this, s = std::move(socket)]() mutable {
                handle_client(std::move(s));
            }).detach();
        }
    }

private:
    void handle_client(tcp::socket socket) {
        try {
            // Send welcome
            std::string welcome = "Welcome to the poker server!\n";
            asio::write(socket, asio::buffer(welcome));

            // Echo loop
            char buf[1024];
            while (true) {
                asio::error_code ec;
                size_t n = socket.read_some(asio::buffer(buf), ec);

                if (ec == asio::error::eof) {
                    std::cout << "Client disconnected\n";
                    break;
                }
                if (ec) throw asio::system_error(ec);

                std::string msg(buf, n);
                std::cout << "Received: " << msg;

                std::string response = "Echo: " + msg;
                asio::write(socket, asio::buffer(response));
            }
        } catch (std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
        }
    }

    tcp::acceptor acceptor_;
};

int main() {
    try {
        asio::io_context io;
        Server server(io, 9090);
        server.run();
    } catch (std::exception& e) {
        std::cerr << "Fatal: " << e.what() << "\n";
        return 1;
    }
}
