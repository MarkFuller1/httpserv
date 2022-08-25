
#include "include/TCP_Server.h"
#include <cstdio>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdlib>
#include <netinet/in.h>
#include <cstring>
#include <iostream>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sstream>

TCP_Server *TCP_Server::instance;

TCP_Server *TCP_Server::getInstance() {
    if (TCP_Server::instance == NULL) {
        TCP_Server::instance = new TCP_Server();
    }
    return instance;
}

TCP_Server::TCP_Server() {
    message = new char[TCP_Server::MESSAGE_SIZE];

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("In socket");
        this->~TCP_Server();
        exit(EXIT_FAILURE);
    }

    linger lin;
    lin.l_onoff = 0;
    lin.l_linger = 0;
    setsockopt(server_fd, SOL_SOCKET, SO_LINGER, (const char *) &lin, sizeof(int));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    memset(address.sin_zero, '\0', sizeof address.sin_zero);

    addrlen = sizeof(address);

    if (bind(server_fd, (struct sockaddr *) &address, sizeof(address)) < 0) {
        perror("In bind");
        perror("In listen");
        instance->~TCP_Server();
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0) {
        perror("In listen");
        instance->~TCP_Server();
        exit(EXIT_FAILURE);
    }

    std::cout << toString() << std::endl;
}

char *TCP_Server::connect() {
    printf("\n+++++++ Waiting for new connection ++++++++\n\n");
    std::cout << toString() << std::endl;
    if ((new_socket = accept(server_fd, (struct sockaddr *) &address, (socklen_t *) &addrlen)) < 0) {
        perror("In accept");

        instance->~TCP_Server();
        exit(EXIT_FAILURE);
    }

    return message;
}

void TCP_Server::sockRead() {

    std::cout << "reading from socket" << std::endl;
    valread = read(new_socket, message, MESSAGE_SIZE);

    if (valread < 1) {
        std::cout << "an error occurred:" << valread << std::endl;
        return;
    }
    std::cout << "read:\t" << std::string(message, valread) << std::endl;
}

void TCP_Server::respond(const char *data, int size) {
    write(instance->new_socket, data, size);
}

bool TCP_Server::disconnect() {
    close(new_socket);
    new_socket = -1;

    std::cout << "disconnected from socket" << std::endl;

    instance->~TCP_Server();
    std::cout << "deallocated memory" << std::endl;

    return true;
}

bool TCP_Server::isClosed() {
    std::cout << "-- checking if socket is closed" << std::endl;
    fd_set rfd;
    FD_ZERO(&rfd);
    FD_SET(new_socket, &rfd);
    timeval tv = {0};

    select(new_socket + 1, &rfd, 0, 0, &tv);

    if (!FD_ISSET(new_socket, &rfd)) {
        std::cout << "-- socket is not closed" << std::endl;
        return false;
    }

    int n = 0;
    ioctl(new_socket, FIONREAD, &n);

    std::cout << "-- socket is " << (n == 0 ? "closed" : "not closed") << std::endl;

    return n == 0;
}

std::string TCP_Server::toString() {
    std::stringstream ss;

    ss << "{ PORT:" << PORT << ",";
    ss << "MESSAGE_SIZE:" << MESSAGE_SIZE << ",";
    ss << "server_fd:" << g_server_fd() << ",";
    ss << "new_socket:" << g_new_socket() << ",";
    ss << "message:" << g_message() << ",";
    ss << "valread:" << valread << "}";

    return ss.str();
}

int TCP_Server::g_server_fd() {
    return TCP_Server::server_fd;
}

int TCP_Server::g_new_socket() {
    return TCP_Server::new_socket;
}

long TCP_Server::g_valread() {
    return TCP_Server::valread;
}

sockaddr_in TCP_Server::g_address() {
    return TCP_Server::address;
}

std::string TCP_Server::g_message() {
    return std::string(TCP_Server::message, TCP_Server::valread);
}

TCP_Server::~TCP_Server() {
    std::cout << "Shutting down server gracefully" << std::endl;
    delete[] message;
}



