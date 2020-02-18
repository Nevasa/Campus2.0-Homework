#include "Server.h"

Server::Server()
{
    if (WSAStartup(MAKEWORD(2, 2), &wsaData))
    {
        assert("WSAStartup failure");
    }

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    char broadcast = '1';
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0)
    {
        closesocket(sock);

        throw("Error in setting Broadcast option");
    }

    Recv_addr.sin_family = AF_INET;
    Recv_addr.sin_port = htons(MYPORT);
    Recv_addr.sin_addr.s_addr = INADDR_BROADCAST;
}


Server::~Server()
{
    closesocket(sock);

    WSACleanup();
}

void Server::sendMessages() {
    while (true)
    {
        char sendMSG[50];
        cin.getline(sendMSG, 50);

        sendto(sock, sendMSG, strlen(sendMSG) + 1, 0, (sockaddr*)& Recv_addr, sizeof(Recv_addr));

        Sleep(50);
    }
}