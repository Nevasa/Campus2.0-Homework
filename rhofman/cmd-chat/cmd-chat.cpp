#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "winsock2.h"
#pragma comment(lib, "Ws2_32.lib")

#include <iostream>
#include <string>

#define PORT 27015
#define BUFFER_SIZE 256

const sockaddr* ToSockaddr(const sockaddr_in& address);
sockaddr* ToSockaddr(sockaddr_in& address);

bool Initialize();
bool Deinitialize();

bool SocketUDPCreate(SOCKET& socketData);
bool SocketUDPSetBroadcastOption(SOCKET& socketData);
bool SocketUDPBindToReceive(SOCKET& socket, const sockaddr_in& address);

bool SocketUDPSendMessage(SOCKET& socket, const sockaddr_in& address, const std::string& message);
bool SocketUDPReceiveMessage(SOCKET& socket, sockaddr_in& address, std::string& message);


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
    if (!SocketUDPCreate(socketSend) || !SocketUDPSetBroadcastOption(socketSend))
    {
        closesocket(socketSend);
        return -1;
    }

    sockaddr_in addressBroadcast = CreateBroadcastAddress();

    const std::string message = ReadUserInput();
    if (!SocketUDPSendMessage(socketSend, addressBroadcast, message))
    {
        closesocket(socketSend);
        return -1;
    }






    SOCKET socketReceive;
    if (!SocketUDPCreate(socketReceive) || !SocketUDPSetBroadcastOption(socketSend))
    {
        closesocket(socketReceive);
        return -1;
    }

    sockaddr_in addressReceive = CreateReceiveAddress();
    if (!SocketUDPBindToReceive(socketReceive, addressReceive))
    {
        closesocket(socketReceive);
        return -1;
    }

    std::string input;
    if (!SocketUDPReceiveMessage(socketReceive, addressReceive, input))
    {
        closesocket(socketReceive);
        return -1;
    }

    std::cout << "\n\n\tReceived message from Reader is =>  " << input;

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

const sockaddr* ToSockaddr(const sockaddr_in& address)
{
    return reinterpret_cast<const sockaddr*>(&address);
}

sockaddr* ToSockaddr(sockaddr_in& address)
{
    return reinterpret_cast<sockaddr*>(&address);
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
        return false;
    }
    return true;
}

bool SocketUDPSendMessage(SOCKET& socket, const sockaddr_in& address, const std::string& message)
{
    const int messageSize = static_cast<int>(message.size()) + 1;
    const sockaddr* addressData = ToSockaddr(address);
    if (sendto(socket, message.c_str(), messageSize, 0, addressData, sizeof(address)) == SOCKET_ERROR)
    {
        std::cout << "Cannot send message\n";
        std::cout << "Error code: " << WSAGetLastError() << '\n';
        return false;
    }
    return true;
}

bool SocketUDPReceiveMessage(SOCKET& socket, sockaddr_in& address, std::string& message)
{
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);

    int structSize = sizeof(sockaddr_in);
    sockaddr* addressData = ToSockaddr(address);
    if (recvfrom(socket, buffer, BUFFER_SIZE, 0, addressData, &structSize) == SOCKET_ERROR)
    {
        std::cout << "Unable receive messages\n";
        std::cout << "Error code: " << WSAGetLastError() << '\n';
        return false;
    }

    message = buffer;
    return true;
}

bool SocketUDPBindToReceive(SOCKET& socket, const sockaddr_in& address)
{
    if (bind(socket, (sockaddr*)& address, sizeof(address)) < 0)
    {
        std::cout << "Unable to bind receive socket\n";
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
