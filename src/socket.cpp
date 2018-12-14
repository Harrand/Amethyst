//
// Created by Harrand on 14/12/2018.
//

#include "socket.hpp"
#include "amethyst.hpp"
#include <iostream>

ISocket::ISocket(SocketDescriptor descriptor): descriptor(descriptor)
{
    if(!am::detail::is_initialised)
        std::cerr << "ISocket::ISocket(...): Amethyst not initialised.\n";
}

#ifdef AMETHYST_WINDOWS
    SocketWindows::SocketWindows(SocketDescriptor descriptor): ISocket(descriptor){}
#elif AMETHYST_UNIX
    SocketUnix::SocketUnix(SocketDescriptor descriptor): ISocket(descriptor){}
#endif
