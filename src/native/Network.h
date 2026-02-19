#ifndef NETWORK_H
#define NETWORK_H

#include "../interpreter/value/Value.h"
#include <iostream>
#include <vector>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#define SOCKET int
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#endif
inline void GWinitNetwork() {
  static bool networkInitialized = false;

  if (networkInitialized)
    return;
#ifdef _WIN32
  WSADATA wsaData;
  int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (iResult != 0) {
    throw std::runtime_error("[GW NATIVE] WSAStartup failed: " +
                             std::to_string(iResult));
  }
#endif
  networkInitialized = true;
}

inline Value __nativeSocketCreate(const std::vector<Value> &args) {
  if (args.size() != 0)
    throw std::runtime_error("[GW NATIVE] socketCreate() takes 0 arguments");

  GWinitNetwork();
  SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sock == INVALID_SOCKET) {
    throw std::runtime_error("[GW NATIVE] socket creation failed");
  }

  return Value((int)sock);
}

inline Value __nativeSocketClose(const std::vector<Value> &args) {
  if (args.size() != 1)
    throw std::runtime_error("[GW NATIVE] socketClose(id) takes 1 argument");
  SOCKET sock = (SOCKET)args[0].asInt();

#ifdef _WIN32
  if (closesocket(sock) == SOCKET_ERROR) {
  }
#else
  close(sock);
#endif
  return Value(true);
}

inline Value __nativeSocketConnect(const std::vector<Value> &args) {
  if (args.size() != 3)
    throw std::runtime_error(
        "[GW NATIVE] socketConnect(id, host, port) takes 3 arguments");

  SOCKET sock = (SOCKET)args[0].asInt();
  std::string host = args[1].asString();
  int port = args[2].asInt();

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);

  unsigned long ip = inet_addr(host.c_str());
  if (ip != INADDR_NONE) {
    addr.sin_addr.s_addr = ip;
  } else {
    struct hostent *he = gethostbyname(host.c_str());
    if (he == NULL) {
      return Value(false);
    }
    memcpy(&addr.sin_addr, he->h_addr_list[0], he->h_length);
  }

  if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR) {
    return Value(false);
  }
  return Value(true);
}

inline Value __nativeSocketSend(const std::vector<Value> &args) {
  if (args.size() != 2)
    throw std::runtime_error(
        "[GW NATIVE] socketSend(id, data) takes 2 arguments");

  SOCKET sock = (SOCKET)args[0].asInt();
  std::string data = args[1].asString();

  if (send(sock, data.c_str(), (int)data.length(), 0) == SOCKET_ERROR) {
    return Value(false);
  }
  return Value(true);
}

inline Value __nativeSocketReceive(const std::vector<Value> &args) {
  if (args.size() != 2)
    throw std::runtime_error(
        "[GW NATIVE] socketReceive(id, size) takes 2 arguments");

  SOCKET sock = (SOCKET)args[0].asInt();
  int size = args[1].asInt();
  if (size <= 0)
    return Value("");

  std::vector<char> buffer(size + 1);
  int bytesReceived = recv(sock, buffer.data(), size, 0);

  if (bytesReceived == SOCKET_ERROR || bytesReceived == 0) {
    return Value("");
  }

  buffer[bytesReceived] = '\0';
  return Value(std::string(buffer.data()));
}

inline Value __nativeSocketBind(const std::vector<Value> &args) {
  if (args.size() != 2)
    throw std::runtime_error(
        "[GW NATIVE] socketBind(id, port) takes 2 arguments");

  SOCKET sock = (SOCKET)args[0].asInt();
  int port = args[1].asInt();

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(port);

  if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR) {
    return Value(false);
  }
  return Value(true);
}

inline Value __nativeSocketListen(const std::vector<Value> &args) {
  if (args.size() != 1)
    throw std::runtime_error("[GW NATIVE] socketListen(id) takes 1 argument");

  SOCKET sock = (SOCKET)args[0].asInt();

  if (listen(sock, SOMAXCONN) == SOCKET_ERROR) {
    return Value(false);
  }
  return Value(true);
}

inline Value __nativeSocketAccept(const std::vector<Value> &args) {
  if (args.size() != 1)
    throw std::runtime_error("[GW NATIVE] socketAccept(id) takes 1 argument");

  SOCKET sock = (SOCKET)args[0].asInt();

  SOCKET clientSock = accept(sock, NULL, NULL);
  if (clientSock == INVALID_SOCKET) {
    throw std::runtime_error("[GW NATIVE] socket accept failed");
  }

  return Value((int)clientSock);
}

#endif // NETWORK_H