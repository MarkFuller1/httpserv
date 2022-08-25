#include "src/include/TCP_Server.h"
#include <iostream>
#include <cstdlib>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void sig_int_handler(int s)
{
    std::cout << "Caught signal:" << s << std::endl;

    TCP_Server *server = TCP_Server::getInstance();

    if (server->new_socket != -1)
    {
        server->disconnect();
    }

    delete[] server->message;
    exit(1);
}

void handle_sig_int()
{
    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = sig_int_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, NULL);
}

int main()
{
    handle_sig_int();
    std::cout << "Starting Server on port " << 8080 << std::endl;

    TCP_Server *tcp_server = TCP_Server::getInstance();

    tcp_server->connect();

    std::string message = std::string(tcp_server->message);

    std::string response = message + " but from the server \r\n";

    do
    {
        std::cout << "responding with:" << tcp_server->message << std::endl;
        tcp_server->respond(tcp_server->message, std::string(tcp_server->message).length());
        std::cout << "responded" << std::endl;

        tcp_server->sockRead();
        message = std::string(tcp_server->message);

    } while (!tcp_server->isClosed() && message.length() != 0);

    tcp_server->disconnect();
    return 0;
}
