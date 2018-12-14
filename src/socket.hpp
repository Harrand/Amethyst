//
// Created by Harrand on 14/12/2018.
//

#ifndef AMETHYST_SOCKET_HPP
#define AMETHYST_SOCKET_HPP
#include "address.hpp"
#include <optional>

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
    virtual bool listen(std::size_t maximum_queue_length) = 0;
    virtual std::optional<IAddress> accept() = 0;
    virtual std::string receive(std::size_t buffer_size) = 0;
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
        virtual bool listen(std::size_t maximum_queue_length = SOMAXCONN) override;
        virtual std::optional<IAddress> accept() override;
        virtual std::string receive(std::size_t buffer_size) override;
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
