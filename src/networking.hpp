//
// Created by Harrand on 14/12/2018.
//

#ifndef AMETHYST_NETWORKING_HPP
#define AMETHYST_NETWORKING_HPP

namespace am::net
{
    namespace consts
    {
        constexpr std::size_t maximum_udp_packet_size = 65507;
        constexpr std::size_t maximum_tcp_packet_size = 65535;
    }
    namespace transmission
    {
        /// Transmission Protocol used for networking: TCP & UDP.
        enum class protocol
        {
            TCP, UDP
        };
    }

    namespace internet
    {
        /// Internet Protocol Version: IPV4 & IPV6.
        enum class protocol
        {
            IPV4, IPV6
        };
    }
}

#endif //AMETHYST_NETWORKING_HPP
