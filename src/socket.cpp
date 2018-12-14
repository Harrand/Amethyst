//
// Created by Harrand on 14/12/2018.
//

#include "socket.hpp"
#include "amethyst.hpp"
#include <iostream>

bool am::detail::is_initialised = false;

SocketDescriptor::SocketDescriptor(am::net::transmission::protocol transmission_protocol, am::net::internet::protocol internet_protocol): transmission_protocol(transmission_protocol), internet_protocol(internet_protocol){}

ISocket::ISocket(SocketDescriptor descriptor): descriptor(descriptor), bound(false)
{
    if(!am::detail::is_initialised)
        std::cerr << "ISocket::ISocket(...): Amethyst not initialised.\n";
}

const SocketDescriptor& ISocket::get_info() const
{
    return this->descriptor;
}

#ifdef AMETHYST_WINDOWS
    SocketWindows::SocketWindows(SocketDescriptor descriptor): ISocket(descriptor), socket_handle(INVALID_SOCKET)
    {
        using namespace am::net;
        int af;
        switch(this->descriptor.internet_protocol)
        {
            default:
            case internet::protocol::IPV4:
                af = AF_INET;
                break;
            case internet::protocol::IPV6:
                af = AF_INET6;
                break;
        }
        int type, protocol;
        switch(this->descriptor.transmission_protocol)
        {
            default:
            case transmission::protocol::TCP:
                type = SOCK_STREAM;
                protocol = IPPROTO_TCP;
                break;
            case transmission::protocol::UDP:
                type = SOCK_DGRAM;
                protocol = IPPROTO_UDP;
                break;
        }
        this->socket_handle = socket(af, type, protocol);
        AMETHYST_DEBUG_PRINT((std::string("SocketWindows::SocketWindows(...) Initialisation Error Code: ") + std::to_string(WSAGetLastError())).c_str());
    }

    bool SocketWindows::bind(unsigned int port)
    {
        using namespace am::net::internet;
        int result;
        switch(this->descriptor.internet_protocol)
        {
            case protocol::IPV6:
            {
                sockaddr_in6 address;
                memset(&address, 0, sizeof(address));
                address.sin6_family = AF_INET6;
                address.sin6_port = htons(port);
                //address.sin6_addr.s_addr = inet_addr("0.0.0.0");
                result = ::bind(this->socket_handle, (sockaddr*)&address, sizeof(address));
            }
                break;
            case protocol::IPV4:
            {
                sockaddr_in address;
                memset(&address, 0, sizeof(address));
                address.sin_family = AF_INET;
                address.sin_port = htons(port);
                address.sin_addr.s_addr = inet_addr("0.0.0.0");
                result = ::bind(this->socket_handle, (sockaddr*)&address, sizeof(address));
            }
                break;
        }
        if(result == -1)
        {
            std::cerr << "SocketWindows::bind(...) produced error code " << WSAGetLastError() << "\n";
            return false;
        }
        this->bound = true;
        return true;
    }

    bool SocketWindows::listen(std::size_t maximum_queue_length)
    {
        if(!this->bound)
        {
            std::cerr << "SocketWindows::listen(...) invoked but the Socket is not bound! Aborting.\n";
            return false;
        }
        int result = ::listen(this->socket_handle, maximum_queue_length);
        if(result == -1)
        {
            std::cerr << "SocketWindows::listen(...) produced error code " << WSAGetLastError() << "\n";
            return false;
        }
        return true;
    }

    std::optional<IAddress> SocketWindows::accept()
    {
        SOCKET other_socket_handle = ::accept(this->socket_handle, nullptr, nullptr);
        if(other_socket_handle == -1)
        {
            AMETHYST_DEBUG_PRINT((std::string("SocketWindows::accept(...) did not connect to a valid socket and produced error code ") + std::to_string(WSAGetLastError())).c_str())
            return std::nullopt;
        }
        sockaddr_in other_socket;
        socklen_t len = sizeof(other_socket);
        getpeername(other_socket_handle, (sockaddr*)&other_socket, &len);
        AddressWindows other_socket_address{other_socket};
        return {static_cast<IAddress>(other_socket_address)};
    }

    std::string SocketWindows::receive(std::size_t buffer_size)
    {
        char* buffer = new char[buffer_size];
        ::recv(this->socket_handle, buffer, buffer_size, 0);
        std::string buffer_copy = buffer;
        delete[] buffer;
        return buffer;
    }
#elif AMETHYST_UNIX
    SocketUnix::SocketUnix(SocketDescriptor descriptor): ISocket(descriptor), socket_handle(-1)
    {
        using namespace am::net;
        int af;
        switch(this->descriptor.internet_protocol)
        {
            case internet::protocol::IPV4:
            default:
                af = AF_INET;
                break;
            case internet::protocol::IPV6:
                af = AF_INET6;
                break;
        }
        int type;
        switch(this->descriptor.transmission_protocol)
        {
            case transmission::protocol::TCP:
            default:
                type = SOCK_STREAM;
                break;
            case transmission::protocol::UDP:
                type = SOCK_DGRAM;
                break;
        }
        this->socket_handle = socket(af, type, 0);
        AMETHYST_DEBUG_PRINT((std::string("SocketUnix::SocketUnix(...) Initialisation Error Code: ") + strerror(errno)).c_str());
    }

    bool SocketUnix::bind(unsigned int port)
    {
        // TODO: Implement
    }
#endif
