#include "server.h"
#include <sstream>
#include <thread>
#include <Mstcpip.h>
#include <ws2tcpip.h>

void response(SOCKET ClientSocket, std::function<std::string(std::string)> & processor);

CExpress::Server::Server()
{
  int iResult;

  iResult = WSAStartup(MAKEWORD(2, 2), &wsaData_);
  if (iResult != 0) {
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
  int iResult;
  iResult = getaddrinfo(NULL, std::to_string(port).c_str(), &hints_, &result_);
  if (iResult != 0) {
    throw "getaddrinfo failed";
  }

  ListenSocket_ = INVALID_SOCKET;
  ListenSocket_ = socket(result_->ai_family, result_->ai_socktype, result_->ai_protocol);
  if (iResult != 0) {
    freeaddrinfo(result_);
    WSACleanup();
    throw "cannot create socket";
  }

  // binding a socket

  iResult = ::bind(ListenSocket_, result_->ai_addr, (int)result_->ai_addrlen);
  if (iResult == SOCKET_ERROR) {
    freeaddrinfo(result_);
    closesocket(ListenSocket_);
    WSACleanup();
    throw "binding a socket failed";
  }

  if (listen(ListenSocket_, SOMAXCONN) == SOCKET_ERROR) {
    freeaddrinfo(result_);
    closesocket(ListenSocket_);
    WSACleanup();
    throw "listen failed";
  }

  freeaddrinfo(result_);
}

void CExpress::Server::process(std::function<std::string(std::string)> processor)
{
  ClientSocket_ = INVALID_SOCKET;

  while (true) {
    DWORD one = 1;
    ClientSocket_ = accept(ListenSocket_, NULL, NULL);

    BOOL bKeepAlive = TRUE;
    int nRet = setsockopt(ClientSocket_, SOL_SOCKET, SO_KEEPALIVE,
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
    nRet = WSAIoctl(ClientSocket_, SIO_KEEPALIVE_VALS, &alive_in, sizeof(alive_in),
      &alive_out, sizeof(alive_out), &ulBytesReturn, NULL, NULL);
    if (nRet == SOCKET_ERROR)
    {
      throw "WSAIoctl failed";
    }

    std::thread responseThread(&response, ClientSocket_, processor);
    responseThread.detach();
  }

  int iResult = shutdown(ClientSocket_, SD_SEND);
  if (iResult == SOCKET_ERROR) {
    closesocket(ListenSocket_);
    closesocket(ClientSocket_);
    WSACleanup();
    throw "shutdown failed:\n";
  }


  closesocket(ClientSocket_);
  closesocket(ListenSocket_);
  WSACleanup();
}


void response(SOCKET ClientSocket, std::function<std::string(std::string)> & processor) {

  const int DEFAULT_BUFLEN = 2048;

  char recvbuf[DEFAULT_BUFLEN];
  int iResult, iSendResult;
  int recvbuflen = DEFAULT_BUFLEN;

  while (true) {
    // receive until the peer shuts down the connection
    iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
    if (iResult > 0) {

      std::string incomingStr(recvbuf);
      std::string result = processor(incomingStr);

      iSendResult = send(ClientSocket, result.c_str(), result.length(), 0);
      if (iSendResult == SOCKET_ERROR) {
        break;
        //throw "send failed";
      }
    }
    else if (iResult == 0) {
      break;
    }
    else {
      // throw "recv failed";
      break;
    }
  }

  closesocket(ClientSocket);
}
