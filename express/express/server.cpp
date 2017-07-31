#include "server.h"

#include <sstream>
#include <thread>
#include <mstcpip.h>
#include <ws2tcpip.h>

void response(SOCKET ClientSocket, std::function<std::string(std::string)> & processor);

CExpress::Server::Server()
{
  int i_result;

  i_result = WSAStartup(MAKEWORD(2, 2), &wsa_data_);
  if (i_result != 0) {
    throw "wsastartup failed";
  }

  result_ = NULL;
  ptr_ = NULL;

  ZeroMemory(&hints_, sizeof(hints_));
  hints_.ai_family = AF_INET;
  hints_.ai_socktype = SOCK_STREAM;
  hints_.ai_protocol = IPPROTO_TCP;
  hints_.ai_flags = AI_PASSIVE;
}

void CExpress::Server::setPort(int port)
{
  int i_result;
  i_result = getaddrinfo(NULL, std::to_string(port).c_str(), &hints_, &result_);
  if (i_result != 0) {
    throw "getaddrinfo failed";
  }

  listen_socket_ = INVALID_SOCKET;
  listen_socket_ = socket(result_->ai_family, result_->ai_socktype, result_->ai_protocol);
  if (i_result != 0) {
    freeaddrinfo(result_);
    WSACleanup();
    throw "cannot create socket";
  }

  // binding a socket

  i_result = ::bind(listen_socket_, result_->ai_addr, (int)result_->ai_addrlen);
  if (i_result == SOCKET_ERROR) {
    freeaddrinfo(result_);
    closesocket(listen_socket_);
    WSACleanup();
    throw "binding a socket failed";
  }

  if (listen(listen_socket_, SOMAXCONN) == SOCKET_ERROR) {
    freeaddrinfo(result_);
    closesocket(listen_socket_);
    WSACleanup();
    throw "listen failed";
  }

  freeaddrinfo(result_);
}

void CExpress::Server::process(const std::function<std::string(std::string)>& processor)
{
  client_socket_ = INVALID_SOCKET;

  while (true) {
    DWORD one = 1;
    client_socket_ = accept(listen_socket_, NULL, NULL);

    BOOL bKeepAlive = TRUE;
    int nRet = setsockopt(client_socket_, SOL_SOCKET, SO_KEEPALIVE,
      (char*)&bKeepAlive, sizeof(bKeepAlive));
    if (nRet == SOCKET_ERROR)
    {
      throw "setsockopt failed";
    }
    // set KeepAlive parameter  
    tcp_keepalive alive_in;
    tcp_keepalive alive_out;
    alive_in.keepalivetime = 500;  // 0.5s  
    alive_in.keepaliveinterval = 1000; //1s  
    alive_in.onoff = TRUE;
    unsigned long ulBytesReturn = 0;
    nRet = WSAIoctl(client_socket_, SIO_KEEPALIVE_VALS, &alive_in, sizeof(alive_in),
      &alive_out, sizeof(alive_out), &ulBytesReturn, NULL, NULL);
    if (nRet == SOCKET_ERROR)
    {
      throw "WSAIoctl failed";
    }

    std::thread responseThread(&response, client_socket_, processor);
    responseThread.detach();
  }

  int i_result = shutdown(client_socket_, SD_SEND);
  if (i_result == SOCKET_ERROR) {
    closesocket(listen_socket_);
    closesocket(client_socket_);
    WSACleanup();
    throw "shutdown failed:\n";
  }


  closesocket(client_socket_);
  closesocket(listen_socket_);
  WSACleanup();
}


void response(SOCKET ClientSocket, std::function<std::string(std::string)> & processor) {

  const int DEFAULT_BUFLEN = 2048;

  char recvbuf[DEFAULT_BUFLEN];
  int i_result, iSendResult;
  int recvbuflen = DEFAULT_BUFLEN;

  while (true) {
    // receive until the peer shuts down the connection
    i_result = recv(ClientSocket, recvbuf, recvbuflen, 0);
    if (i_result > 0) {

      std::string incomingStr(std::string(recvbuf).substr(0, i_result));
      std::string result = processor(incomingStr);

      iSendResult = send(ClientSocket, result.c_str(), result.length(), 0);
      if (iSendResult == SOCKET_ERROR) {
        break;
        //throw "send failed";
      }
    }
    else if (i_result == 0) {
      break;
    }
    else {
      // throw "recv failed";
      break;
    }
  }

  closesocket(ClientSocket);
}
