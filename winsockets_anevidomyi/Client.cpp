#include "Client.h"

Client::Client()
{
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    sock = socket(AF_INET, SOCK_DGRAM, 0);

    char broadcast = '1';
    //     This option is needed on the socket in order to be able to receive broadcast messages
    //   If not set the receiver will not receive broadcast messages in the local network.

    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0)
    {
        cout << "Error in setting Broadcast option";
        closesocket(sock);
    }

    Recv_addr.sin_family = AF_INET;
    Recv_addr.sin_port = htons(MYPORT);
    Recv_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, (sockaddr*)& Recv_addr, sizeof(Recv_addr)) < 0)
    {
        cout << "Error in BINDING" << WSAGetLastError();
        closesocket(sock);
    }

}

Client::~Client()
{
    closesocket(sock);

    WSACleanup();
}

void Client::receiveMessages()
{
    while (true)
    {
        int len = sizeof(struct sockaddr_in);
        char recvbuff[50];
        int recvbufflen = 50;

        int a = recvfrom(sock, recvbuff, recvbufflen, 0, (sockaddr*)& Sender_addr, &len);
        if (a > 0)
        {
            char ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &Sender_addr.sin_addr, ip, INET_ADDRSTRLEN);
            recvbuff[a] = '\0';
            cout << ip << ": " << recvbuff << endl;
        }
    }
}
