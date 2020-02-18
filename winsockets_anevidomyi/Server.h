#pragma once
#include "includes.h"

class Server
{
public:

	Server();
	~Server();
	void sendMessages();
private:
	struct sockaddr_in Recv_addr;

	struct sockaddr_in Sender_addr;

	SOCKET sock;
	WSAData wsaData;
};