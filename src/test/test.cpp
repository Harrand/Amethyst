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
        socketB.send("hi my name is harrand and this is a message whose size i cannot be bothered to calculate. i hope that the receiving socket is able to determine the correct size of this message so that they allocate enough size to see the true message within: ducks.");
        std::cout << "socket A data received size = " << socketA.peek_receive_size() << "\n";
        std::string received = socketA.receive_boundless().value_or("invalid data");
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
        socketB.send("hello. this is some data, except that this data is being sent via TCP. pretty cool eh?");
        std::optional<Address> possible_connection = socketA.accept();
        if(possible_connection.has_value())
        {
            std::cout << "found connection!\n";
            std::string received = socketA.receive_boundless().value_or("invalid data");
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

