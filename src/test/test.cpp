//
// Created by Harrand on 14/12/2018.
//
#include "socket.hpp"
#include "amethyst.hpp"
#include <iostream>

int main()
{
    am::initialise();
    using namespace am::net;
    Socket socket{SocketDescriptor{transmission::protocol::TCP, internet::protocol::IPV4}};
    socket.bind(80);
    socket.listen();
    while(1)
    {
        std::optional<IAddress> possible_connection = socket.accept();
        if(possible_connection.has_value())
        {
            std::cout << "found connection!\n";
            std::cout << "ip address is " << possible_connection.value().as_string() << "\n";
            std::cout << "data read: " << socket.receive(1024).c_str() << "\n";
            break;
        }
    }
    am::terminate();
}

