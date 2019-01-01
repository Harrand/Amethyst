# Amethyst
## About
### What is Amethyst?
Amethyst is a low-level socket-based networking library written in C++17. It supports simple socket-based networking interactions. It serves as a wrapper for OS-specific socket functionality; I've gone through the WinSock API and created a wrapper so you don't have to.
### What is Amethyst NOT?
Amethyst is NOT a high-level networking library. If you're looking for a networking library for your game, then you may wish to consider a higher-level networking library, such as [boost::asio](http://boost.org/libs/asio).
### Supports
Protocols:
 - UDP (Complete)
 - TCP (Complete)
 - IPv4 (Complete)
 - IPv6 (Upcoming)

Platforms:
 - Windows (Complete)
 - Linux (Upcoming)

### License

This project is licensed under the MIT license - see the [LICENSE.md](LICENSE.md) file for details.

## Installation
### Prerequisites

* Machine running on either Windows or Linux
* C++17-compliant compiler
* CMake for building the project
### Compilation
##### CMake:
See `CMakeLists.txt` in the root directory.

## Sample Code
See the [test file](https://github.com/Harrand/Amethyst/blob/master/src/test/test.cpp) included in the repository.

## Documentation
See the [wiki](https://github.com/Harrand/Amethyst/wiki).