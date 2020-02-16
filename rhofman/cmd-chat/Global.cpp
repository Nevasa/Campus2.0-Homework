#include "Global.h"

#include <winsock2.h>
#include <iostream>

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