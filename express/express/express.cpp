#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <iostream>
#include <thread>
#include <Mstcpip.h>
#include "express.h"

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

#define DEFAULT_PORT "27015"

string responseStr = "HTTP/1.1 200 OK\n\
Content-Type: text/html; charset=utf-8\n\
Content-Length: 2\n\
\n\
hi";

void response(SOCKET ClientSocket);

//int main() {
//	WSADATA wsaData;
//
//	int iResult;
//
//	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
//	if (iResult != 0) {
//		cout << "wsastartup failed" << endl;
//		return 1;
//	}
//
//	struct addrinfo
//		* result = NULL,
//		*ptr = NULL,
//		hints;
//
//	ZeroMemory(&hints, sizeof(hints));
//	hints.ai_family = AF_INET;
//	hints.ai_socktype = SOCK_STREAM;
//	hints.ai_protocol = IPPROTO_TCP;
//	hints.ai_flags = AI_PASSIVE;
//
//	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
//	if (iResult != 0) {
//		cout << "getaddrinfo failed" << endl;
//		return 1;
//	}
//
//	SOCKET ListenSocket = INVALID_SOCKET;
//	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
//	if (iResult != 0) {
//		cout << "cannot create socket" << endl;
//		freeaddrinfo(result);
//		WSACleanup();
//		return 1;
//	}
//
//	// binding a socket
//
//	iResult = ::bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
//	if (iResult == SOCKET_ERROR) {
//		cout << "binding a socket failed" << endl;
//		freeaddrinfo(result);
//		closesocket(ListenSocket);
//		WSACleanup();
//		return 1;
//	}
//
//	if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
//		cout << "listen failed" << endl;
//		freeaddrinfo(result);
//		closesocket(ListenSocket);
//		WSACleanup();
//		return 1;
//	}
//
//	SOCKET ClientSocket = INVALID_SOCKET;
//
//	while (true) {
//		DWORD one = 1;
//		
//		
//
//		ClientSocket = accept(ListenSocket, NULL, NULL);
//
//		BOOL bKeepAlive = TRUE;
//		int nRet = setsockopt(ClientSocket, SOL_SOCKET, SO_KEEPALIVE,
//			(char*)&bKeepAlive, sizeof(bKeepAlive));
//		if (nRet == SOCKET_ERROR)
//		{
//			//TRACE(L"setsockopt failed: %d/n", WSAGetLastError());
//			return FALSE;
//		}
//		// set KeepAlive parameter  
//		tcp_keepalive alive_in;
//		tcp_keepalive alive_out;
//		alive_in.keepalivetime = 500;  // 0.5s  
//		alive_in.keepaliveinterval = 1000; //1s  
//		alive_in.onoff = TRUE;
//		unsigned long ulBytesReturn = 0;
//		nRet = WSAIoctl(ClientSocket, SIO_KEEPALIVE_VALS, &alive_in, sizeof(alive_in),
//			&alive_out, sizeof(alive_out), &ulBytesReturn, NULL, NULL);
//		if (nRet == SOCKET_ERROR)
//		{
//			//TRACE(L"WSAIoctl failed: %d/n", WSAGetLastError());
//			return FALSE;
//		}
//
//
//		//setsockopt(ListenSocket, SOL_SOCKET, SO_KEEPALIVE, (char *)&one, sizeof(one));
//		//setsockopt(ClientSocket, SOL_SOCKET, SO_KEEPALIVE, (char *)&one, sizeof(one));
//
//
//		//if (ClientSocket == INVALID_SOCKET) {
//		//	cout << "accept failed" << endl;
//		//	freeaddrinfo(result);
//		//	closesocket(ListenSocket);
//		//	WSACleanup();
//		//	return 1;
//		//}
//		
//		//response(ClientSocket);
//		thread responseThread(response, ClientSocket);
//		responseThread.detach();
//	}
//	
//	iResult = shutdown(ClientSocket, SD_SEND);
//	//if (iResult == SOCKET_ERROR) {
//	//	printf("shutdown failed: %d\n", WSAGetLastError());
//	//	freeaddrinfo(result);
//	//	closesocket(ClientSocket);
//	//	WSACleanup();
//	//	return 1;
//	//}
//
//	//freeaddrinfo(result);
//	//closesocket(ClientSocket);
//	//closesocket(ListenSocket);
//	//WSACleanup();
//	
//	return 0;
//}


void CExpress::Express::use(std::function<void(CExpress::Request&, CExpress::Response&)>& func)
{
  // check if the function is callable, if not then return immediatly.
  if (!func)
    throw ("Function warpper without target is not allowed for the use function");

  _middleWares.push_back(func);
}

void CExpress::Express::route(std::string, std::string, std::function<void(CExpress::Request&, CExpress::Response&)>)
{
}

void CExpress::Express::onerror(std::string, std::function<void(CExpress::Request&, CExpress::Response&)>)
{
}

CExpress::Express::Express()
{
}

void CExpress::Express::start(int port)
{
  _server = Server();
  _server.setPort(port);
  _server.process([=](string incomingStr) {
    // process incoming str

    // TODO:
    // create a request and a response
    // go through middleware
    // find controller
    // call the controller
    // fallback try find the file
    // otherwise, find error handlers
    // otherwise, return default error page

    return responseStr;
  });
}
