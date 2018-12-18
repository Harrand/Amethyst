//
// Created by Harrand on 14/12/2018.
//
#include "socket.hpp"
#include "amethyst.hpp"
#include <iostream>
#include <windows.h>

int main()
{
    am::initialise();
    using namespace am::net;
    // UDP Example
    Socket socketA{{transmission::protocol::UDP, internet::protocol::IPV4}};
    socketA.bind(80);
    Socket socketB{{transmission::protocol::UDP, internet::protocol::IPV4}};
    socketB.connect("127.0.0.1", 80);
    while(1)
    {
        socketB.send("some_data");
        std::string received = socketA.receive(9).value_or("invalid data");
        std::cout << received;
        if(received != "invalid data")
            break;
    }
    socketA.unbind();
    socketB.unbind();
    // End UDP Example

    // TCP Example
    /*
    Socket socketA{{transmission::protocol::TCP, internet::protocol::IPV4}};
    socketA.bind(80);
    socketA.listen();
    Socket socketB{{transmission::protocol::TCP, internet::protocol::IPV4}};
    while(1)
    {
        socketB.connect("127.0.0.1", 80);
        socketB.send("some_data");
        std::optional<Address> possible_connection = socketA.accept();
        if(possible_connection.has_value())
        {
            std::cout << "found connection!\n";
            std::string received = socketA.receive(9).value_or("invalid data");
            std::cout << "data received: " << received << "\n";
            break;
        }
    }
    socketA.unbind();
    socketB.unbind();
     */
    // End TCP Example
    am::terminate();
}

