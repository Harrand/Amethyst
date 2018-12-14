//
// Created by Harrand on 14/12/2018.
//

#ifndef AMETHYST_UTILITY_HPP
#define AMETHYST_UTILITY_HPP

#ifdef _WIN32
    #define AMETHYST_WINDOWS
    #include <ws2tcpip.h>
    #include <windows.h>
#elif __unix__ || __APPLE__ || __linux__
    #define AMETHYST_UNIX
    #include <sys/socket.h>
    #include <unistd.h>
    #include <errno.h>
#else
    static_assert(false, "Amethyst does not detect to be running under Windows nor Linux. Thus, compilation must abort.");
#endif

#ifdef AMETHYST_DEBUG
#define AMETHYST_DEBUG_PRINT(x) printf("%s", x)
#else
#define AMETHYST_DEBUG_PRINT(x)
#endif

#endif //AMETHYST_UTILITY_HPP
