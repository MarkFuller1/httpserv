#pragma once

#include <string>
#include <netinet/in.h>

class TCP_Server
{
public:
    const int PORT = 8080;
    const int MESSAGE_SIZE = 30000;

    int server_fd = -1, new_socket = -1;
    struct sockaddr_in address;
    char *message = new char[MESSAGE_SIZE];

    long valread = 0;
    long addrlen = 0;

    static TCP_Server *getInstance();
    char *connect();
    void sockRead();
    void respond(const char *data, int size);
    bool disconnect();
    bool isClosed();
    std::string toString();

private:
    TCP_Server();
    static TCP_Server *instance;
};
