#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "winsock2.h"
#pragma comment(lib, "Ws2_32.lib")

#include "Global.h"
#include "SocketUDP.h"
#include "AddressUtility.h"

#include <iostream>
#include <string>

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

std::string ReadUserInput()
{
    std::cout << "Enter message to send:\n";
    std::string input;
    std::getline(std::cin, input);

    return input;
}
