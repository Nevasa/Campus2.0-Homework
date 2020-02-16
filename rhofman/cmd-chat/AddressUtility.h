#pragma once

#include <winsock2.h>

const sockaddr* ToSockaddr(const sockaddr_in& address);
sockaddr* ToSockaddr(sockaddr_in& address);

sockaddr_in CreateBroadcastAddress();
sockaddr_in CreateReceiveAddress();