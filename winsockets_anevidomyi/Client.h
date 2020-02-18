#pragma once
#include "includes.h"

class Client
{
public:
    Client();
    ~Client();
    void receiveMessages();
private:
    struct sockaddr_in Recv_addr;

    struct sockaddr_in Sender_addr;

    SOCKET sock;
    WSAData wsaData;
};

