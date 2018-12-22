//
// Created by Harrand on 14/12/2018.
//

#ifndef AMETHYST_UTILITY_HPP
#define AMETHYST_UTILITY_HPP

#include <iostream>
#include <sstream>
#include <utility>

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

namespace am::debug
{
    template<typename T>
    inline std::string to_string(T&& obj)
    {
        if constexpr(std::is_convertible_v<T, std::string>)
            return std::forward<T>(obj);
        else if constexpr(std::is_same_v<T, int> || std::is_same_v<T, float> || std::is_same_v<T, double>)
            return std::to_string(obj);
        else
        {
            std::ostringstream oss;
            oss << std::forward<T>(obj);
            return oss.str();
        }
    }

    inline void print(){}

    template<typename FirstArg, typename... Args>
    inline void print(FirstArg&& arg, Args&&... args)
    {
        #ifndef AMETHYST_DEBUG
            return;
        #endif
        if constexpr(std::is_same_v<FirstArg, std::string>)
            std::cout << arg;
        else
            std::cout << am::debug::to_string(arg);
        am::debug::print(std::forward<Args>(args)...);
    }
}

#endif //AMETHYST_UTILITY_HPP
