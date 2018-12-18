//
// Created by Harrand on 14/12/2018.
//

#include "address.hpp"

AddressDescriptor::AddressDescriptor(am::net::internet::protocol internet_protocol): internet_protocol(internet_protocol){}

Address::Address(AddressDescriptor descriptor, std::string address): descriptor(descriptor), address(address){}

const AddressDescriptor& Address::get_info() const
{
    return this->descriptor;
}

const std::string& Address::as_string() const
{
    return this->address;
}

#ifdef AMETHYST_WINDOWS
    AddressWindows::AddressWindows(AddressDescriptor descriptor, std::string address): Address(descriptor, address){}

    AddressWindows::AddressWindows(const Address& address): Address(address.get_info(), address.as_string()){}

    AddressWindows::AddressWindows(const sockaddr_in& windows_address): Address(AddressDescriptor{am::net::internet::protocol::IPV4}, inet_ntoa(windows_address.sin_addr)){}

    AddressWindows::AddressWindows(const sockaddr_in6& windows_address): Address(AddressDescriptor{am::net::internet::protocol::IPV6}, "")
    {
        // 39 characters should appropriate to store an ipv6 address.
        this->address.resize(39);
        DWORD required_size;
        WSAAddressToStringA((sockaddr*)&windows_address, sizeof(&windows_address), nullptr, this->address.data(), &required_size);
    }

    sockaddr_in AddressWindows::get_winsock_address_ipv4(unsigned int port) const
    {
        sockaddr_in result;
        memset(&result, 0, sizeof(result));
        result.sin_family = AF_INET;
        result.sin_port = htons(port);
        result.sin_addr.s_addr = inet_addr(this->as_string().c_str());
        return result;
    }

    sockaddr_in6 AddressWindows::get_winsock_address_ipv6(unsigned int port) const
    {
        sockaddr_in6 result;
        return result;
    }
#elif AMETHYST_UNIX

#endif
