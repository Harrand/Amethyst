//
// Created by Harrand on 14/12/2018.
//

#ifndef AMETHYST_NETWORKING_HPP
#define AMETHYST_NETWORKING_HPP

namespace am::net
{
    namespace transmission
    {
        enum class protocol
        {
            TCP, UDP
        };
    }

    namespace internet
    {
        enum class protocol
        {
            IPV4, IPV6
        };
    }
}

#endif //AMETHYST_NETWORKING_HPP
