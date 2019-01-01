//
// Created by Harry on 01/01/2019.
//
#include "socket.hpp"
#include "amethyst.hpp"

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
    am::terminate();
}

