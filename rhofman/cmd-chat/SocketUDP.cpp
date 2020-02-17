#include "SocketUDP.h"

#include "AddressUtility.h"
#include "Global.h"

#include <iostream>

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