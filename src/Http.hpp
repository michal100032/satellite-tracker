#pragma once

#include <string_view>

// #define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <WS2tcpip.h>

struct HttpResponse {
	static const int BUFFER_SIZE = 4096;

	uint8_t contents[BUFFER_SIZE];
	int length;
};

class Http {
public:
	Http();
	~Http();

	HttpResponse request(std::string_view url, uint16_t port = 80);
private:
	sockaddr_in getAddress(std::string_view host, uint16_t port);
	std::string_view getHost(std::string_view url);
	std::string_view getPath(std::string_view url);

	size_t findBody(std::string_view response);

};