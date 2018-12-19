//
// Created by Harrand on 14/12/2018.
//

#ifndef AMETHYST_ADDRESS_HPP
#define AMETHYST_ADDRESS_HPP
#include "utility/platform.hpp"
#include "networking.hpp"
#include <string>

/**
 * Contains the following information about an address:
 * - Internet Protocol Version (IPV4 or IPV6)
 */
struct AddressDescriptor
{
    /**
     * Construct an AddressDescriptor from all specifications.
     * @param internet_protocol - The IP version describing the address
     */
    AddressDescriptor(am::net::internet::protocol internet_protocol = am::net::internet::protocol::IPV4);
    am::net::internet::protocol internet_protocol;
};

/**
 * Stores either an IPV4 or IPV6 address.
 */
class Address
{
public:
    /**
     * Construct an Address from a descriptor and IP address string.
     * @param descriptor - Descriptor about the address
     * @param address - IP address represented as a string
     */
    Address(AddressDescriptor descriptor, std::string address);
    /**
     * Retrieve the address descriptor
     * @return - Description of this address
     */
    const AddressDescriptor& get_info() const;
    /**
     * Retrieve the IP address as a string.
     * Examples:
     * "127.0.0.1" (IPV4)
     * "FE80:CD00:0000:0CDE:1257:0000:211E:729C" (IPV6)
     * @return - This address, represented as a string
     */
    const std::string& as_string() const;
protected:
    /// Contains information about this address
    AddressDescriptor descriptor;
    /// Contains this address expressed as a string
    std::string address;
};

#ifdef AMETHYST_WINDOWS
    /**
    * Stores either an IPV4 or IPV6 address. Windows-specific implementation.
    */
    class AddressWindows : public Address
    {
    public:
        /**
        * Construct an AddressWindows from a descriptor and IP address string.
        * @param descriptor - Descriptor about the address
        * @param address - IP address represented as a string
        */
        AddressWindows(AddressDescriptor descriptor, std::string address);
        /**
         * Construct an AddressWindows from a less-concrete address.
         * @param address - Address to create a windows-specific copy from
         */
        AddressWindows(const Address& address);
        /**
         * Construct an AddressWindows from an existing WinSock address. (IPV4)
         * @param windows_address - The given WinSock IPV4 address
         */
        AddressWindows(const sockaddr_in& windows_address);
        /**
         * Construct an AddressWindows from an existing WinSock address. (IPV6)
         * @param windows_address - The given WinSock IPV6 address
         */
        AddressWindows(const sockaddr_in6& windows_address);
        /**
         * Retrieve a corresponding WinSock address based off of this address. (IPV4)
         * @param port - The desired port bound through this address
         * @return - The resultant WinSock IPV4 address
         */
        sockaddr_in get_winsock_address_ipv4(unsigned int port) const;
        /**
         * Retrieve a corresponding WinSock address based off of this address. (IPV6)
         * @param port - The desired port bound through this address
         * @return - The resultant WinSock IPV6 address
         */
        sockaddr_in6 get_winsock_address_ipv6(unsigned int port) const;
    };
#elif AMETHYST_UNIX
    class AddressUnix : public IAddress
    {

    };
#endif


#endif //AMETHYST_ADDRESS_HPP
