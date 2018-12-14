//
// Created by Harrand on 14/12/2018.
//

#ifndef AMETHYST_AMETHYST_HPP
#define AMETHYST_AMETHYST_HPP
#include "utility/platform.hpp"

namespace am
{
    inline void initialise();
    inline void terminate();

#ifdef AMETHYST_WINDOWS
    namespace windows
        {
            inline void initialise();
            inline void terminate();
        }
#elif AMETHYST_UNIX
    namespace unix
        {
            inline void initialise();
            inline void terminate();
        }
#endif
    namespace detail
    {
        extern bool is_initialised;
    }
}

#include "amethyst.inl"

#endif //AMETHYST_AMETHYST_HPP
