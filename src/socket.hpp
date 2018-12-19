//
// Created by Harrand on 14/12/2018.
//

#ifndef AMETHYST_SOCKET_HPP
#define AMETHYST_SOCKET_HPP
#include "address.hpp"
#include <optional>

namespace am::net::transmission
{
    /// This is an arbitrary value used by a boundless receive call, so that a message is processed in fragments of this number of bytes.
    constexpr std::size_t default_receive_buffer_size = 2;
}

/**
 * Contains the following information about a socket:
 * - Transmission Protocol (TCP or UDP)
 * - Internet Protocol Version (IPV4 or IPV6)
 */
struct SocketDescriptor
{
    SocketDescriptor(am::net::transmission::protocol transmission_protocol = am::net::transmission::protocol::TCP, am::net::internet::protocol ip = am::net::internet::protocol::IPV4);
    am::net::transmission::protocol transmission_protocol;
    am::net::internet::protocol internet_protocol;
};

/**
 * Partially-abstract Socket class overridden by windows-specific or linux-specific sockets.
 */
class ISocket
{
public:
    /**
     * Create a socket using the given protocols.
     * @param descriptor - SocketDescriptor describing the desired socket
     */
    ISocket(SocketDescriptor descriptor);
    /**
     * Retrieve information about this socket.
     * @return - SocketDescriptor containing information about this socket
     */
    const SocketDescriptor& get_info() const;
    /**
     * Bind a receiving socket to a specific port. Should only be invoked on sockets expecting to receive data.
     * @param port - The port to bind to
     * @return - True if the bind was successful. False otherwise
     */
    virtual bool bind(unsigned int port) = 0;
    /**
     * Cause a bound receiving TCP socket to listen for incoming connections from clients.
     * @param maximum_queue_length - The maximum number of clients that can be queued at a time
     * @return - Whether the socket has successfully began listening for incoming connections
     */
    virtual bool listen(std::size_t maximum_queue_length) = 0;
    /**
     * Cause a bound sending TCP/UDP socket to connect to an existing server socket at the given address.
     * @param address - The destination address
     * @param port - The port that the destination socket is bound to
     * @return - True if a connection was made. False otherwise
     */
    virtual bool connect(const std::string& address, unsigned int port) = 0;
    /**
     * Accept the client at the front of the queue for a given listening TCP receiving socket.
     * @return - The address of the client. If there is no client, the optional is null
     */
    virtual std::optional<Address> accept() = 0;
    /**
     * Send data from a sending TCP/UDP socket to its connected server socket.
     * @param data - The string of bytes to be sent to the server
     * @return - True if the send was successful. False otherwise
     */
    virtual bool send(const std::string& data) = 0;
    /**
     * Causes a receiving socket to query the size of the data sent to it.
     * Available to:
     * - A bound TCP receiving socket with one accepted client.
     * - A bound UDP receiving socket
     * @return - Number of bytes sent from the accepted client to this receiving socket
     */
    virtual std::size_t peek_receive_size() = 0;
    /**
     * Causes a receiving socket to retrieve a portion of the data sent to it.
     * Available to:
     * - A bound TCP receiving socket with one accepted client.
     * - A bound UDP receiving socket
     * @param buffer_size
     * @return - The string of bytes sent to this server socket. If no data was sent, the optional is null
     */
    virtual std::optional<std::string> receive(std::size_t buffer_size) = 0;
    /**
     * Causes a receiving socket to retrieve ALL of the data sent to it.
     * Available to:
     * - A bound TCP receiving socket with one accepted client.
     * - A bound UDP receiving socket.
     * @return - The string of bytes sent to this server socket from its accepted client. If no data was sent, the optional is null
     */
    virtual std::optional<std::string> receive_boundless() = 0;
    /**
     * Unbind and release a given socket. This is called by the destructor, but can be invoked manually if a Socket needs to be retargeted.
     * @return - True if the socket was successfully unbound. False otherwise
     */
    virtual bool unbind() = 0;
protected:
    /// Structure containing information about this socket.
    SocketDescriptor descriptor;
    /// Whether the socket is a bound receiving socket.
    bool bound;
    /// The port of the receiving socket for both UDP and TCP, if it has been given one. Sending sockets will have this be null.
    std::optional<unsigned int> port;
    /// The address of the destination of this UDP sending socket, as defined in ISocket::connect(...).
    std::optional<Address> destination_address;
};

#ifdef AMETHYST_WINDOWS
    /**
     * Provides windows-specific Socket functionality.
     */
    class SocketWindows : public ISocket
    {
    public:
        /**
        * Create a socket using the given protocols.
        * @param descriptor - SocketDescriptor describing the desired socket
        */
        SocketWindows(SocketDescriptor descriptor);
        /// Automatically unbinds the socket.
        ~SocketWindows();
        /**
        * Bind a receiving socket to a specific port. Should only be invoked on sockets expecting to receive data.
        * @param port - The port to bind to
        * @return - True if the bind was successful. False otherwise
        */
        virtual bool bind(unsigned int port) override;
        /**
        * Cause a bound receiving TCP socket to listen for incoming connections from clients.
        * @param maximum_queue_length - The maximum number of clients that can be queued at a time
        * @return - Whether the socket has successfully began listening for incoming connections
        */
        virtual bool listen(std::size_t maximum_queue_length = SOMAXCONN) override;
        /**
        * Cause a bound sending TCP/UDP socket to connect to an existing server socket at the given address.
        * @param address - The destination address
        * @param port - The port that the destination socket is bound to
        * @return - True if a connection was made. False otherwise
        */
        virtual bool connect(const std::string& address, unsigned int port) override;
        /**
        * Accept the client at the front of the queue for a given listening TCP receiving socket.
        * @return - The address of the client. If there is no client, the optional is null
        */
        virtual std::optional<Address> accept() override;
        /**
        * Send data from a sending TCP/UDP socket to its connected server socket.
        * @param data - The string of bytes to be sent to the server
        * @return - True if the send was successful. False otherwise
        */
        virtual bool send(const std::string& data) override;
        /**
         * Causes a receiving socket to query the size of the data sent to it.
         * Available to:
         * - A bound TCP receiving socket with one accepted client.
         * - A bound UDP receiving socket
         * @return - Number of bytes sent from the accepted client to this receiving socket
         */
        virtual std::size_t peek_receive_size() override;
        /**
        * Causes a receiving socket to retrieve data sent to it.
        * Available to:
        * - A bound TCP receiving socket with one accepted client.
        * - A bound UDP receiving socket
        * @param buffer_size
        * @return - The string of bytes sent to this server socket. If no data was sent, the optional is null
        */
        virtual std::optional<std::string> receive(std::size_t buffer_size) override;
        /**
        * Causes a receiving socket to retrieve ALL of the data sent to it.
        * Available to:
        * - A bound TCP receiving socket with one accepted client.
        * - A bound UDP receiving socket.
        * @return - The string of bytes sent to this server socket from its accepted client. If no data was sent, the optional is null
        */
        virtual std::optional<std::string> receive_boundless() override;
        /**
        * Unbind and release a given socket. This is called by the destructor, but can be invoked manually if a Socket needs to be retargeted.
        * @return - True if the socket was successfully unbound. False otherwise
        */
        virtual bool unbind() override;
    private:
        /// Socket handle for this socket.
        SOCKET socket_handle;
        /// Socket handle for the sending socket connected to this receiving socket. Shall remain INVALID_SOCKET for a sending socket.
        SOCKET connection_handle;
    };
    /// Alias for SocketWindows.
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
    /// Alias for SocketUnix.
    using Socket = SocketUnix;
#endif

#include "socket.inl"
#endif //AMETHYST_SOCKET_HPP
