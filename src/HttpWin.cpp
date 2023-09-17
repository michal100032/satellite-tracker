#include "Http.hpp"

#include <string>
#include <iostream>

Http::Http() {
	WSAData wsaData;
	WORD dllVersion = MAKEWORD(2, 2);
	if (WSAStartup(dllVersion, &wsaData) != 0) {
		std::cout << "WSA failure" << std::endl;
	}
}

Http::~Http() {
	WSACleanup();
}

HttpResponse Http::request(std::string_view url, uint16_t port)
{
	std::string host(getHost(url));
	sockaddr_in addr = getAddress(host, port);

	std::string path(getPath(url));

	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET) {
		std::cout << "Invalid socket!" << std::endl;
	}

	std::string request = "GET " + path + " HTTP/1.1\r\nHost: " + host + "\r\n\r\n";

	if (connect(sock, (const sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
		int error = WSAGetLastError();
		std::cout << "Connection error " << error << std::endl;

	}
	if (send(sock, request.c_str(), request.size(), 0) == SOCKET_ERROR) {
		int error = WSAGetLastError();
		std::cout << "Sending error " << error << std::endl;
	}

	char buffer[HttpResponse::BUFFER_SIZE];
	int len = recv(sock, (char*)&buffer, sizeof(buffer), 0);

	HttpResponse response;
	memset(&response.contents, 0, sizeof(response.contents));

	size_t position = findBody(buffer);
	if (!position || position > len) {
		return response;
	}
	response.length = len - position;
	memcpy(&response.contents, buffer + position, response.length);

	return response;
}

sockaddr_in Http::getAddress(std::string_view host, uint16_t port) {
	addrinfo hints;
	memset(&hints, 0, sizeof(hints));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	addrinfo* infoBegin = nullptr;

	sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);

	DWORD retVal = getaddrinfo(host.data(), NULL, &hints, &infoBegin);
	if (retVal != 0) {
		std::cout << "getaddrinfo failed with error: " << retVal << std::endl;
		return addr;
	}

	for (addrinfo* info = infoBegin; info != nullptr; info = info->ai_next) {
		if (info->ai_family == AF_INET) {
			addr.sin_addr.S_un = ((sockaddr_in*)info->ai_addr)->sin_addr.S_un;

			return addr;
		}
	}
	freeaddrinfo(infoBegin);

	return addr;
}

std::string_view Http::getHost(std::string_view url) {
	size_t position = url.find("/");
	if (position == std::string_view::npos) {
		return url;
	}
	return url.substr(0, position);
}

std::string_view Http::getPath(std::string_view url) {
	size_t position = url.find("/");
	if (position == std::string_view::npos) {
		return "/";
	}
	return url.substr(position);
}

size_t Http::findBody(std::string_view response) {
	size_t position = response.find("\n\n");
	if (position == std::string_view::npos) {
		position = response.find("\r\n\r\n");
		if (position == std::string_view::npos) {
			position = response.find("\r\r");
			if (position == std::string_view::npos) {
				return 0;
			}
			else position += 2;
		}
		else position += 4;
	}
	else position += 2;
	return position;
}