//
// Created by Harrand on 14/12/2018.
//
#include "socket.hpp"
#include "amethyst.hpp"

int main()
{
    am::initialise();
    using namespace am::net;
    Socket socket{SocketDescriptor{transmission::protocol::TCP, internet::protocol::IPV4}};
    am::terminate();
}

