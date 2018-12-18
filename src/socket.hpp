//
// Created by Harrand on 14/12/2018.
//

#ifndef AMETHYST_SOCKET_HPP
#define AMETHYST_SOCKET_HPP
#include "address.hpp"
#include <optional>

namespace am::net::transmission
{
    constexpr std::size_t default_receive_buffer_size = 2;
}

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
    virtual bool connect(const std::string& address, unsigned int port) = 0;
    virtual std::optional<Address> accept() = 0;
    virtual bool send(const std::string& data) = 0;
    virtual std::optional<std::string> receive(std::size_t buffer_size) = 0;
    virtual bool unbind() = 0;
protected:
    SocketDescriptor descriptor;
    bool bound;
    std::optional<unsigned int> port;
    std::optional<Address> destination_address;
};

#ifdef AMETHYST_WINDOWS
    class SocketWindows : public ISocket
    {
    public:
        SocketWindows(SocketDescriptor descriptor);
        ~SocketWindows();
        virtual bool bind(unsigned int port) override;
        virtual bool listen(std::size_t maximum_queue_length = SOMAXCONN) override;
        virtual bool connect(const std::string& address, unsigned int port) override;
        virtual std::optional<Address> accept() override;
        virtual bool send(const std::string& data) override;
        virtual std::optional<std::string> receive(std::size_t buffer_size) override;
        virtual bool unbind() override;
    private:
        SOCKET socket_handle, connection_handle;
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
