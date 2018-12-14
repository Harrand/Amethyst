//
// Created by Harrand on 14/12/2018.
//

#ifndef AMETHYST_SOCKET_HPP
#define AMETHYST_SOCKET_HPP
#include "amethyst.hpp"
#include "networking.hpp"

bool am::detail::is_initialised = false;

struct SocketDescriptor
{
    SocketDescriptor(am::net::transmission::protocol transmission_protocol = am::net::transmission::protocol::TCP);
    am::net::transmission::protocol transmission_protocol;
};

class ISocket
{
public:
    ISocket(SocketDescriptor descriptor);
private:
    SocketDescriptor descriptor;
};

#ifdef AMETHYST_WINDOWS
    class SocketWindows : public ISocket
    {
    public:
        SocketWindows(SocketDescriptor descriptor);
    };
    using Socket = SocketWindows;
#elif AMETHYST_UNIX
    class SocketUnix : public ISocket
    {
    public:
        SocketUnix(SocketDescriptor descriptor);
    };
    using Socket = SocketUnix;
#endif

#include "socket.inl"
#endif //AMETHYST_SOCKET_HPP
