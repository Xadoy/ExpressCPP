#pragma once

#ifdef _WIN32
  #ifndef WIN32_LEAN_AND_MEAN
  #define WIN32_LEAN_AND_MEAN
  #endif
#endif

#ifdef _WIN32
  #include <windows.h>
  #include <winsock2.h>
#else
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <unistd.h>
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
#endif

#include <functional>
#include <string>

#ifdef _WIN32
#pragma comment(lib, "Ws2_32.lib")
#endif

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
#ifdef _WIN32
    WSADATA wsa_data_;
    SOCKET listen_socket_;
    SOCKET client_socket_;
    struct addrinfo *result_, *ptr_, hints_;
#else
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    
    struct sockaddr_in serv_addr, cli_addr;
    
    
    void error(const char *msg);
#endif
  };
}
