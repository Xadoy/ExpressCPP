#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

#define DEFAULT_PORT "27015"

string response = "HTTP/1.1 200 OK\n\
Content-Type: text/html; charset=utf-8\n\
Server: CPPServer\n\
Connection: close\n\
Content-Length: 2\n\
\n\
hi";

int main() {
	WSADATA wsaData;

	int iResult;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		cout << "wsastartup failed" << endl;
		return 1;
	}

	struct addrinfo
		* result = NULL,
		*ptr = NULL,
		hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		cout << "getaddrinfo failed" << endl;
		return 1;
	}

	SOCKET ListenSocket = INVALID_SOCKET;
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (iResult != 0) {
		cout << "cannot create socket" << endl;
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	// https://msdn.microsoft.com/en-us/library/windows/desktop/bb530742(v=vs.85).aspx

	return 0;
}
