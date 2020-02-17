#pragma once

#include <winsock2.h>
#include <string>

bool SocketUDPCreate(SOCKET& socketData);
bool SocketUDPSetBroadcastOption(SOCKET& socketData);
bool SocketUDPBindToReceive(SOCKET& socket, const sockaddr_in& address);

bool SocketUDPSendMessage(SOCKET& socket, const sockaddr_in& address, const std::string& message);
bool SocketUDPReceiveMessage(SOCKET& socket, sockaddr_in& address, std::string& message);
