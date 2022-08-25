#pragma once

#include <string>
#include <netinet/in.h>

class TCP_Server
{
public:
    static const int PORT = 8082;
    static const int MESSAGE_SIZE = 30000;

    int g_server_fd();
    int g_new_socket();
    sockaddr_in g_address();
    std::string g_message();
    long g_valread();

    static TCP_Server *getInstance();
    ~TCP_Server();
    char *connect();
    void sockRead();
    void respond(const char *data, int size);
    bool disconnect();
    bool isClosed();
    std::string toString();

private:
    TCP_Server();
    static TCP_Server *instance;

    int server_fd = -1, new_socket = -1;
    struct sockaddr_in address;
    char *message;

    long valread = 0;
    long addrlen = 0;
};
