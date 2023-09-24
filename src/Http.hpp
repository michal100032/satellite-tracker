#pragma once

#include <string_view>

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <WS2tcpip.h>

struct HttpResponse {
	char* body;
	size_t length;

	HttpResponse();
	HttpResponse(size_t length);

	HttpResponse(const HttpResponse&);
	HttpResponse(HttpResponse&&) noexcept;

	~HttpResponse();

	HttpResponse& operator=(const HttpResponse&);
	HttpResponse& operator=(HttpResponse&&) noexcept;
};

class Http {
public:
	Http();
	~Http();

	HttpResponse get(std::string_view url, uint16_t port = 80);
private:
	SOCKET connectSocket(std::string_view host, uint16_t port);
	void sendGet(SOCKET sock, const std::string& path, const std::string& host);
	HttpResponse receive(SOCKET sock);

	int getContentLength(std::string_view headers);
	sockaddr_in getAddress(std::string_view host, uint16_t port);
	std::string_view getHost(std::string_view url);
	std::string_view getPath(std::string_view url);

	size_t findBody(std::string_view response);

};