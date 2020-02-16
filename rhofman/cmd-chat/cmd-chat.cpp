#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "winsock2.h"
#pragma comment(lib, "Ws2_32.lib")

#include "Global.h"
#include "SocketUDP.h"
#include "AddressUtility.h"

#include <iostream>
#include <string>
#include <thread>

void ThreadMessagesReceiveAndPrint();
void ThreadMessagesReadAndSend();

std::string ReadUserInput();


int main()
{
    if (!Initialize())
    {
        return -1;
    }

    std::thread threadReceive(&ThreadMessagesReceiveAndPrint);
    threadReceive.detach();

    std::cout << "Enter " << EXIT_MESSAGE << " to close the program\n\n";
    ThreadMessagesReadAndSend();

    if (!Deinitialize())
    {
        return -1;
    }

    return 0;
}

void ThreadMessagesReceiveAndPrint()
{
    SOCKET socketReceive;
    if (!SocketUDPCreate(socketReceive) || !SocketUDPSetBroadcastOption(socketReceive))
    {
        closesocket(socketReceive);
        return;
    }

    sockaddr_in addressReceive = CreateReceiveAddress();
    if (!SocketUDPBindToReceive(socketReceive, addressReceive))
    {
        closesocket(socketReceive);
        return;
    }

    while (true)
    {
        sockaddr_in address = CreateReceiveAddress();

        std::string input;
        if (!SocketUDPReceiveMessage(socketReceive, address, input))
        {
            closesocket(socketReceive);
            return;
        }
        std::cout << "\t\t\t[ >> ] " << input << "\n";
    }
}

void ThreadMessagesReadAndSend()
{
    SOCKET socketSend;
    if (!SocketUDPCreate(socketSend) || !SocketUDPSetBroadcastOption(socketSend))
    {
        closesocket(socketSend);
        return;
    }
    sockaddr_in addressBroadcast = CreateBroadcastAddress();

    while (true)
    {
        const std::string message = ReadUserInput();
        if (message == EXIT_MESSAGE)
        {
            return;
        }

        if (!SocketUDPSendMessage(socketSend, addressBroadcast, message))
        {
            closesocket(socketSend);
            return;
        }
    }
}

std::string ReadUserInput()
{
    std::string input;
    std::getline(std::cin, input);

    return input;
}
