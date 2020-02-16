#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "winsock2.h"
#pragma comment(lib, "Ws2_32.lib")

#include <iostream>
#include <string>

#define PORT 27015

bool Initialize();
bool Deinitialize();

bool SocketUDPCreate(SOCKET& socketData);
bool SocketUDPSetBroadcastOption(SOCKET& socketData);
bool SocketUDPSendMessage(SOCKET& socket, const std::string& message, const sockaddr_in& address);

sockaddr_in CreateBroadcastAddress();
sockaddr_in CreateReceiveAddress();

std::string ReadUserInput();

int main()
{
    if (!Initialize())
    {
        return -1;
    }

    SOCKET socketSend;
    if (!SocketUDPCreate(socketSend))
    {
        return -1;
    }

    if (!SocketUDPSetBroadcastOption(socketSend))
    {
        return -1;
    }

    sockaddr_in addressBroadcast = CreateBroadcastAddress();

    const std::string message = ReadUserInput();
    if (!SocketUDPSendMessage(socketSend, message, addressBroadcast))
    {
        return -1;
    }






    SOCKET socketReceive;
    if (!SocketUDPCreate(socketReceive))
    {
        return -1;
    }

    sockaddr_in addressReceive = CreateReceiveAddress();
    if (bind(socketReceive, (sockaddr*)& addressReceive, sizeof(addressReceive)) < 0)
    {
        std::cout << "Unable to bind receive socket\n";
        std::cout << "Error code: " << WSAGetLastError() << '\n';
        closesocket(socketReceive);
        return -1;
    }

    int len = sizeof(struct sockaddr_in);
    char buffer[50] = "";
    int bufferSize = 50;
    if (recvfrom(socketReceive, buffer, bufferSize, 0, (sockaddr*)& addressReceive, &len) == SOCKET_ERROR)
    {
        std::cout << "Unable receive messages\n";
        std::cout << "Error code: " << WSAGetLastError() << '\n';
        closesocket(socketReceive);
        return -1;
    }

    std::cout << "\n\n\tReceived message from Reader is =>  " << buffer;

    std::cout << "\n\n\tpress any key to CONT...";
    getchar();


    closesocket(socketSend);
    closesocket(socketReceive);


    if (!Deinitialize())
    {
        return -1;
    }

    return 0;
}

bool Initialize()
{
    WSADATA wsaData;
    const int error = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (error != 0)
    {
        std::cout << "Networking lib cannot be initialized\n";
        std::cout << "Error code: " << error << '\n';
        return false;
    }
    return true;
}

bool Deinitialize()
{
    if (WSACleanup() == SOCKET_ERROR)
    {
        std::cout << "Networking lib cannot be deinitialized\n";
        std::cout << "Error code: " << WSAGetLastError() << '\n';
        return false;
    }
    return true;
}

bool SocketUDPCreate(SOCKET& socketData)
{
    socketData = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (socketData == INVALID_SOCKET)
    {
        std::cout << "Unable to create socket\n";
        std::cout << "Error code: " << WSAGetLastError() << '\n';
        return false;
    }
    return true;
}

bool SocketUDPSetBroadcastOption(SOCKET& socketData)
{
    const char broadcast = '1';
    if (setsockopt(socketData, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) == SOCKET_ERROR)
    {
        std::cout << "Unable to set socket parameters\n";
        std::cout << "Error code: " << WSAGetLastError() << '\n';
        closesocket(socketData);
        return false;
    }
    return true;
}

bool SocketUDPSendMessage(SOCKET& socket, const std::string& message, const sockaddr_in& address)
{
    const int messageSize = static_cast<int>(message.size()) + 1;
    const sockaddr* addressData = reinterpret_cast<const sockaddr*>(&address);
    if (sendto(socket, message.c_str(), messageSize, 0, addressData, sizeof(address)) == SOCKET_ERROR)
    {
        std::cout << "Cannot send message\n";
        std::cout << "Error code: " << WSAGetLastError() << '\n';
        return false;
    }
    return true;
}

sockaddr_in CreateBroadcastAddress()
{
    sockaddr_in addressReceiver;
    addressReceiver.sin_family = AF_INET;
    addressReceiver.sin_port = htons(PORT);
    addressReceiver.sin_addr.s_addr = INADDR_BROADCAST;

    return addressReceiver;
}

sockaddr_in CreateReceiveAddress()
{
    sockaddr_in addressReceiver;
    addressReceiver.sin_family = AF_INET;
    addressReceiver.sin_port = htons(PORT);
    addressReceiver.sin_addr.s_addr = INADDR_ANY;

    return addressReceiver;
}

std::string ReadUserInput()
{
    std::cout << "Enter message to send:\n";
    std::string input;
    std::getline(std::cin, input);

    return input;
}
