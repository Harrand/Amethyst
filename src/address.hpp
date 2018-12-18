//
// Created by Harrand on 14/12/2018.
//

#ifndef AMETHYST_ADDRESS_HPP
#define AMETHYST_ADDRESS_HPP
#include "utility/platform.hpp"
#include "networking.hpp"
#include <string>

struct AddressDescriptor
{
    AddressDescriptor(am::net::internet::protocol internet_protocol = am::net::internet::protocol::IPV4);
    am::net::internet::protocol internet_protocol;
};

class Address
{
public:
    Address(AddressDescriptor descriptor, std::string address);
    const AddressDescriptor& get_info() const;
    const std::string& as_string() const;
protected:
    AddressDescriptor descriptor;
    std::string address;
};

#ifdef AMETHYST_WINDOWS
    class AddressWindows : public Address
    {
    public:
        AddressWindows(AddressDescriptor descriptor, std::string address);
        AddressWindows(const Address& address);
        AddressWindows(const sockaddr_in& windows_address);
        AddressWindows(const sockaddr_in6& windows_address);
        sockaddr_in get_winsock_address_ipv4(unsigned int port) const;
        sockaddr_in6 get_winsock_address_ipv6(unsigned int port) const;
    };
#elif AMETHYST_UNIX
    class AddressUnix : public IAddress
    {

    };
#endif


#endif //AMETHYST_ADDRESS_HPP
