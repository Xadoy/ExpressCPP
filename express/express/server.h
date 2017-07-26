#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <string>
#include <windows.h>
#include <winsock2.h>
#include <functional>

namespace CExpress {
  class Server {
  public:
    /* default constructor*/
    Server();
    /* set port to server and open tcp ip*/
    void setPort(int);
    /*
    listen on port and use the given lambda to process
    the incoming connection and bytes
    */
    void process(std::function<std::string(std::string)>);

  private:
    WSADATA wsaData_;
    SOCKET ListenSocket_;
    SOCKET ClientSocket_;
    struct addrinfo *result_, *ptr_, hints_;
  };
}
