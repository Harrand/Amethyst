//
// Created by Harrand on 14/12/2018.
//

#include "socket.hpp"
#include "amethyst.hpp"
#include <iostream>

bool am::detail::is_initialised = false;

SocketDescriptor::SocketDescriptor(am::net::transmission::protocol transmission_protocol, am::net::internet::protocol internet_protocol): transmission_protocol(transmission_protocol), internet_protocol(internet_protocol){}

ISocket::ISocket(SocketDescriptor descriptor): descriptor(descriptor), bound(false), destination_address(std::nullopt), port(std::nullopt)
{
    if(!am::detail::is_initialised)
        std::cerr << "ISocket::ISocket(...): Amethyst not initialised.\n";
}

const SocketDescriptor& ISocket::get_info() const
{
    return this->descriptor;
}

#ifdef AMETHYST_WINDOWS
    SocketWindows::SocketWindows(SocketDescriptor descriptor): ISocket(descriptor), socket_handle(INVALID_SOCKET), connection_handle(INVALID_SOCKET)
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

    SocketWindows::~SocketWindows()
    {
        this->unbind();
        closesocket(this->socket_handle);
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
                result = ::bind(this->socket_handle, reinterpret_cast<sockaddr*>(&address), sizeof(address));
            }
                break;
            case protocol::IPV4:
            {
                sockaddr_in address;
                memset(&address, 0, sizeof(address));
                address.sin_family = AF_INET;
                address.sin_port = htons(port);
                address.sin_addr.s_addr = inet_addr("0.0.0.0");
                result = ::bind(this->socket_handle, reinterpret_cast<sockaddr*>(&address), sizeof(address));
            }
                break;
        }
        if(result == -1)
        {
            std::cerr << "SocketWindows::bind(...) produced error code " << WSAGetLastError() << "\n";
            return false;
        }
        this->bound = true;
        this->port = {port};
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

    bool SocketWindows::connect(const std::string& address, unsigned int port)
    {
        using namespace am::net::transmission;
        if(this->get_info().transmission_protocol == protocol::TCP)
        {
            addrinfo connection_info;
            addrinfo *response = nullptr;
            memset(&connection_info, 0, sizeof(connection_info));
            connection_info.ai_family = AF_UNSPEC;
            switch (this->get_info().transmission_protocol)
            {
                case protocol::TCP:
                    connection_info.ai_socktype = SOCK_STREAM;
                    connection_info.ai_protocol = IPPROTO_TCP;
                    break;
                case protocol::UDP:
                    connection_info.ai_socktype = SOCK_DGRAM;
                    connection_info.ai_protocol = IPPROTO_UDP;
                    break;
            }
            if (getaddrinfo(address.c_str(), std::to_string(port).c_str(), &connection_info, &response) != 0)
            {
                AMETHYST_DEBUG_PRINT((std::string(
                        "SocketWindows::connect(...) could not resolve the target address. It produced error ") +
                                      std::to_string(WSAGetLastError()) + ".\n").c_str());
                return false;
            }
            addrinfo *response_pointer = nullptr;
            for (response_pointer = response; response_pointer != nullptr; response_pointer = response_pointer->ai_next)
            {
                if (::connect(this->socket_handle, response_pointer->ai_addr, static_cast<int>(response_pointer->ai_addrlen)) == SOCKET_ERROR)
                {
                    AMETHYST_DEBUG_PRINT((std::string("SocketWindows::connect(...) could not connect to the target address. It produced error ") +std::to_string(WSAGetLastError()) + ".\n").c_str());
                    return false;
                }
            }
            freeaddrinfo(response);
        }
        this->destination_address = {{{this->get_info().internet_protocol}, address}};
        this->port = {port};
        return true;
    }

    std::optional<Address> SocketWindows::accept()
    {
        this->connection_handle = ::accept(this->socket_handle, nullptr, nullptr);
        if(this->connection_handle == -1)
        {
            AMETHYST_DEBUG_PRINT((std::string("SocketWindows::accept(...) did not connect to a valid socket and produced error code ") + std::to_string(WSAGetLastError())).c_str())
            return std::nullopt;
        }
        sockaddr_in other_socket;
        socklen_t len = sizeof(other_socket);
        getpeername(this->connection_handle, (sockaddr*)&other_socket, &len);
        AddressWindows other_socket_address{other_socket};
        this->destination_address = {static_cast<Address>(other_socket_address)};
        return this->destination_address;
    }

    bool SocketWindows::send(const std::string& data)
    {
        using namespace am::net::transmission;
        if(this->get_info().transmission_protocol == protocol::TCP)
        {
            int result = ::send(this->socket_handle, data.c_str(), data.size(), 0);
            if (result == SOCKET_ERROR)
            {
                AMETHYST_DEBUG_PRINT((std::string(
                        "SocketWindows::send(...) could not send data correctly (TCP) and produced error code ") +
                                      std::to_string(WSAGetLastError())).c_str());
                return false;
            }
            return true;
        }
        else // UDP Send
        {
            if(!this->destination_address.has_value() || !this->port.has_value())
            {
                // Never connected.
                AMETHYST_DEBUG_PRINT("SocketWindows::send(...) invoked (UDP) but there is no destination address!");
                return false;
            }
            AddressWindows dest_address = {this->destination_address.value()};
            sockaddr_in win_address = dest_address.get_winsock_address_ipv4(this->port.value());
            int result = sendto(this->socket_handle, data.c_str(), data.size(), 0, reinterpret_cast<sockaddr*>(&win_address), sizeof(win_address));
            if(result == SOCKET_ERROR)
            {
                AMETHYST_DEBUG_PRINT((std::string("SocketWindows::send(...) could not send data correctly (UDP) and produced error code") + std::to_string(WSAGetLastError())).c_str());
                return false;
            }
            return true;
        }
    }

    std::size_t SocketWindows::peek_receive_size()
    {
        using namespace am::net::transmission;
        std::string buffer;
        if(this->get_info().transmission_protocol == protocol::TCP)
        {
            if (this->connection_handle == INVALID_SOCKET)
            {
                AMETHYST_DEBUG_PRINT(
                        "SocketWindows::receive(...) was invoked (TCP) but has not accepted an incoming connection. Aborting...");
                return 0;
            }
            buffer.resize(am::net::consts::maximum_tcp_packet_size);
            int result = ::recv(this->connection_handle, buffer.data(), buffer.size(), MSG_PEEK);
            if (result == SOCKET_ERROR)
            {
                AMETHYST_DEBUG_PRINT((std::string("SocketWindows::receive(...) could not receive data properly (TCP) and produced error code ") + std::to_string(WSAGetLastError())).c_str());
                return 0;
            }
            return result;
        }
        else // UDP Receive
        {
            buffer.resize(am::net::consts::maximum_udp_packet_size);
            int result = recvfrom(this->socket_handle, buffer.data(), buffer.size(), MSG_PEEK, nullptr, nullptr);
            if(result == SOCKET_ERROR)
            {
                AMETHYST_DEBUG_PRINT((std::string("SocketWindows::receive(...) could not receive data properly (UDP) and produced error code ") + std::to_string(WSAGetLastError())).c_str());
                return 0;
            }
            return result;
        }
    }

    std::optional<std::string> SocketWindows::receive(std::size_t buffer_size)
    {
        using namespace am::net::transmission;
        std::string buffer;
        buffer.resize(buffer_size);
        if(this->get_info().transmission_protocol == protocol::TCP)
        {
            if (this->connection_handle == INVALID_SOCKET)
            {
                AMETHYST_DEBUG_PRINT(
                        "SocketWindows::receive(...) was invoked (TCP) but has not accepted an incoming connection. Aborting...");
                return std::nullopt;
            }
            int result = ::recv(this->connection_handle, buffer.data(), buffer_size, 0);
            if (result == SOCKET_ERROR)
            {
                AMETHYST_DEBUG_PRINT((std::string("SocketWindows::receive(...) could not receive data properly (TCP) and produced error code ") + std::to_string(WSAGetLastError())).c_str());
                return std::nullopt;
            }
            return buffer;
        }
        else // UDP Receive
        {
            int result = recvfrom(this->socket_handle, buffer.data(), buffer_size, 0, nullptr, nullptr);
            if(result == SOCKET_ERROR)
            {
                AMETHYST_DEBUG_PRINT((std::string("SocketWindows::receive(...) could not receive data properly (UDP) and produced error code ") + std::to_string(WSAGetLastError())).c_str());
                return std::nullopt;
            }
            return buffer;
        }
    }

    std::optional<std::string> SocketWindows::receive_boundless()
    {
        return this->receive(this->peek_receive_size());
    }

    bool SocketWindows::unbind()
    {
        return closesocket(this->socket_handle) == 0;
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
