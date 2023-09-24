#include "Http.hpp"

#include <string>
#include <iostream>
#include <charconv>

HttpResponse::HttpResponse() :
	body(nullptr), length(0) {
}

HttpResponse::HttpResponse(size_t length) :
	body(new char[length + 1]), length(length) {

	body[length] = '\0';
}

HttpResponse::HttpResponse(const HttpResponse& other) :
	body(new char[other.length + 1]), length(other.length) {

	body[length] = '\0';
}

HttpResponse::HttpResponse(HttpResponse&& other) noexcept :
	body(other.body), length(other.length) {

	other.body = nullptr;
	other.length = 0;
}

HttpResponse::~HttpResponse() {
	delete[] body;
}

HttpResponse& HttpResponse::operator=(const HttpResponse& other) {
	body = other.body;
	length = other.length;

	return *this;
}

HttpResponse& HttpResponse::operator=(HttpResponse&& other) noexcept {
	body = other.body;
	length = other.length;

	other.body = nullptr;
	other.length = 0;

	return *this;
}

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

HttpResponse Http::get(std::string_view url, uint16_t port)
{
	std::string host(getHost(url));
	std::string path(getPath(url));

	SOCKET sock = connectSocket(host, port);
	sendGet(sock, path, host);

	HttpResponse response = receive(sock);

	return response;
}

SOCKET Http::connectSocket(std::string_view host, uint16_t port) {
	sockaddr_in addr = getAddress(host, port);

	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET) {
		printf("Invalid socket!");
	}

	if (connect(sock, (const sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
		int error = WSAGetLastError();
		printf("Connection error %d", error);

	}

	return sock;
}

void Http::sendGet(SOCKET sock, const std::string& path, const std::string& host) {
	std::string request = "GET " + path + " HTTP/1.1\r\nHost: " + host + "\r\n\r\n";
	if (send(sock, request.c_str(), request.size(), 0) == SOCKET_ERROR) {
		int error = WSAGetLastError();
		printf("Sending error %d", error);
	}
}

HttpResponse Http::receive(SOCKET sock) {

	char headerBuffer[1024];

	int receivedLength = recv(sock, (char*)&headerBuffer, sizeof(headerBuffer), 0);

	std::string_view bufferView(headerBuffer, receivedLength);

	int bodyStart = findBody(bufferView);
	if (bodyStart == 0 || bodyStart > receivedLength) {
		closesocket(sock);
		return HttpResponse();
	}

	int contentLength = getContentLength(bufferView);
	if (contentLength == 0) {
		closesocket(sock);
		return HttpResponse();
	}

	HttpResponse response(contentLength);

	int bodyReceiveOffset = receivedLength - bodyStart;

	memset(response.body, 0, contentLength);
	memcpy(response.body, headerBuffer + bodyStart, bodyReceiveOffset);

	while (contentLength + bodyStart > receivedLength) {
		receivedLength += recv(sock, response.body + bodyReceiveOffset, contentLength - bodyReceiveOffset, 0);
		bodyReceiveOffset = receivedLength - bodyStart;
	}

	closesocket(sock);

	return response;

}

int Http::getContentLength(std::string_view headers) {
	std::string_view contLenStr = "Content-Length: ";

	size_t contLenPos = headers.find(contLenStr);
	if (contLenPos == std::string_view::npos) {
		std::cout << "No content length header!" << std::endl;
		return 0;
	}
	contLenPos += contLenStr.size();

	size_t contLenLen = headers.find_first_of("\r\n", contLenPos) - contLenPos;
	std::string_view contLenInt = headers.substr(contLenPos, contLenLen);

	size_t length;
	if (std::from_chars(contLenInt.data(), contLenInt.data() + contLenInt.size(), length).ec == std::errc()) {
		return length;
	}
	return 0;
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
		printf("getaddrinfo failed with error: %d\n", retVal);
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
	size_t position = response.find("\r\n\r\n");
	if (position == std::string_view::npos) {
		position = response.find("\n\n");
		if (position == std::string_view::npos) {
			position = response.find("\r\r");
			if (position == std::string_view::npos) {
				return 0;
			}
			else position += 2;
		}
		else position += 2;
	}
	else position += 4;
	return position;
}