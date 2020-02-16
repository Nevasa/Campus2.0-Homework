#include "winsock2.h"
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

#define PORT 27015

int main()
{
    WSADATA wsaData;
    int error = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (error != 0)
    {
        std::cout << "Networking lib cannot be initialized\n";
        std::cout << "Error code: " << error << '\n';
        return -1;
    }

    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET)
    {
        std::cout << "Unable to create socket\n";
        std::cout << "Error code: " << WSAGetLastError() << '\n';
        return -1;
    }

    char broadcast = '1';

    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) == SOCKET_ERROR)
    {
        std::cout << "Unable to set socket parameters\n";
        std::cout << "Error code: " << WSAGetLastError() << '\n';
        closesocket(sock);
        return -1;
    }

    int len = sizeof(struct sockaddr_in);

    sockaddr_in addressReceiver;
    addressReceiver.sin_family = AF_INET;
    addressReceiver.sin_port = htons(PORT);
    addressReceiver.sin_addr.s_addr  = INADDR_BROADCAST;

    char message[] = "Broadcast message from SLAVE TAG";
    sendto(sock, message, strlen(message) + 1, 0, (sockaddr*)& addressReceiver, sizeof(addressReceiver));

    char buffer[50] = "";
    int bufferSize = 50;
    recvfrom(sock, buffer, bufferSize, 0, (sockaddr*)& addressReceiver, &len);

    std::cout << "\n\n\tReceived message from Reader is =>  " << buffer;

    std::cout << "\n\n\tpress any key to CONT...";
    getchar();

    closesocket(sock);
    WSACleanup();
}
