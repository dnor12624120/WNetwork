#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#if defined(WIN32) || defined(_WIN32)
#include "WSAManager.h"
#elif defined(__linux__)
#define SOCKET int
#endif

#include "IPv4Address.h"
#include "Socket.h"
#include "SocketAddress.h"
#include "FileDescriptorSet.h"
#include "WinSockException.h"