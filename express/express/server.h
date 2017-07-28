#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <string>
#include <windows.h>
#include <winsock2.h>
#include <functional>

#pragma comment(lib, "Ws2_32.lib")

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
    void process(const std::function<std::string(std::string)>&);

  private:
    WSADATA wsa_data_;
    SOCKET listen_socket_;
    SOCKET client_socket_;
    struct addrinfo *result_, *ptr_, hints_;
  };
}
