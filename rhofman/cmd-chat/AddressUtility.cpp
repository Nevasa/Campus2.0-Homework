#include "AddressUtility.h"
#include "Global.h"

const sockaddr* ToSockaddr(const sockaddr_in& address)
{
    return reinterpret_cast<const sockaddr*>(&address);
}

sockaddr* ToSockaddr(sockaddr_in& address)
{
    return reinterpret_cast<sockaddr*>(&address);
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
