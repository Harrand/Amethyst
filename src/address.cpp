//
// Created by Harrand on 14/12/2018.
//

#include "address.hpp"

AddressDescriptor::AddressDescriptor(am::net::internet::protocol internet_protocol): internet_protocol(internet_protocol){}

IAddress::IAddress(AddressDescriptor descriptor, std::string address): descriptor(descriptor), address(address){}

const AddressDescriptor& IAddress::get_info() const
{
    return this->descriptor;
}

const std::string& IAddress::as_string() const
{
    return this->address;
}

#ifdef AMETHYST_WINDOWS
    AddressWindows::AddressWindows(AddressDescriptor descriptor, std::string address): IAddress(descriptor, address){}

    AddressWindows::AddressWindows(const sockaddr_in& windows_address): IAddress(AddressDescriptor{am::net::internet::protocol::IPV4}, inet_ntoa(windows_address.sin_addr)){}

    AddressWindows::AddressWindows(const sockaddr_in6& windows_address): IAddress(AddressDescriptor{am::net::internet::protocol::IPV6}, "")
    {
        // 39 characters should appropriate to store an ipv6 address.
        this->address.resize(39);
        DWORD required_size;
        WSAAddressToStringA((sockaddr*)&windows_address, sizeof(&windows_address), nullptr, this->address.data(), &required_size);
    }
#elif AMETHYST_UNIX

#endif
