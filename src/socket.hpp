//
// Created by Harrand on 14/12/2018.
//

#ifndef AMETHYST_SOCKET_HPP
#define AMETHYST_SOCKET_HPP
#include "utility/platform.hpp"
#include "networking.hpp"

struct SocketDescriptor
{
    SocketDescriptor(am::net::transmission::protocol transmission_protocol = am::net::transmission::protocol::TCP, am::net::internet::protocol ip = am::net::internet::protocol::IPV4);
    am::net::transmission::protocol transmission_protocol;
    am::net::internet::protocol internet_protocol;
};

class ISocket
{
public:
    ISocket(SocketDescriptor descriptor);
    const SocketDescriptor& get_info() const;
    virtual bool bind(unsigned int port) = 0;
protected:
    SocketDescriptor descriptor;
    bool bound;
};

#ifdef AMETHYST_WINDOWS
    class SocketWindows : public ISocket
    {
    public:
        SocketWindows(SocketDescriptor descriptor);
        virtual bool bind(unsigned int port) override;
    private:
        SOCKET socket_handle;
    };
    using Socket = SocketWindows;
#elif AMETHYST_UNIX
    class SocketUnix : public ISocket
    {
    public:
        SocketUnix(SocketDescriptor descriptor);
        virtual bool bind(unsigned int port) override;
    private:
        int socket_handle;
    };
    using Socket = SocketUnix;
#endif

#include "socket.inl"
#endif //AMETHYST_SOCKET_HPP
