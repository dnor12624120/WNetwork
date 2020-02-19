#pragma once

#include "WinSockException.h"
#include "IPv4Address.h"

#if defined(WIN32) || defined(_WIN32)
	#include <WinSock2.h>

	#define WNetworkGetErrorNumber() WSAGetLastError()
#elif defined(__linux__)
	#include <unistd.h>
	#include <sys/socket.h>
  	#include <errno.h>

  	#define WNetworkGetErrorNumber() errno
	#define closesocket(socketHandle) close(socketHandle)

  	#define INVALID_SOCKET -1
  	#define SOCKET_ERROR -1
#endif

class Socket
{
	public:
		Socket() = default;
		Socket(int16_t family, int32_t type, int32_t protocol) :
			m_family{ family },
			m_type{ type },
			m_protocol{ protocol }
		{
			m_handle = socket(m_family, m_type, m_protocol);
			if (m_handle == INVALID_SOCKET)
			{
				std::string errorCode(std::to_string(WNetworkGetErrorNumber()));
				throw WinSockException(std::string("Error creating socket! (" + errorCode + ")").c_str());
			}
		}

		Socket(SOCKET handle, int16_t family, int32_t type, int32_t protocol) :
			m_handle{ handle },
			m_family{ family },
			m_type{ type },
			m_protocol{ protocol }
		{

		}

		Socket(const Socket& socket) = delete;
		Socket& operator=(const Socket& socket) = delete;

		Socket(Socket&& socket) :
			m_handle{ socket.m_handle },
			m_family{ socket.m_family },
			m_type{ socket.m_type },
			m_protocol{ socket.m_protocol }
		{
			socket.m_handle = INVALID_SOCKET;
		}

		Socket& operator=(Socket&& socket)
		{
			m_handle = socket.m_handle;
			m_family = socket.m_family;
			m_type = socket.m_type;
			m_protocol = socket.m_protocol;
			socket.m_handle = INVALID_SOCKET;
			return *this;
		}

		~Socket() throw()
		{
			if (m_handle != INVALID_SOCKET && closesocket(m_handle) == SOCKET_ERROR)
			{
				std::string errorCode(std::to_string(WNetworkGetErrorNumber()));
				throw WinSockException(std::string("Error closing socket! (" + errorCode + ")").c_str());
			}
		}

		void Bind(const IPv4Address& address)
		{
			if (bind(m_handle, (sockaddr*)&address.GetSocketAddress().GetStructure(), sizeof(sockaddr)) != 0)
			{
				std::string errorCode(std::to_string(WNetworkGetErrorNumber()));
				throw WinSockException(std::string("Error binding socket! (" + errorCode + ")").c_str());
			}
		}

		void Listen(int32_t backlog)
		{
			if (listen(m_handle, backlog) != 0)
			{
				std::string errorCode(std::to_string(WNetworkGetErrorNumber()));
				throw WinSockException(std::string("Error trying to listen to incoming connections! (" + errorCode + ")").c_str());
			}
		}


		void Accept(Socket& clientSocket, IPv4Address& clientAddress)
		{
			sockaddr_in address;
			socklen_t addressSize = sizeof(sockaddr_in);
			SOCKET clientHandle = accept(m_handle, (sockaddr*)&address, &addressSize);
			if (clientHandle == INVALID_SOCKET)
			{
				std::string errorCode(std::to_string(WNetworkGetErrorNumber()));
				throw WinSockException(std::string("Error accepting connection! (" + errorCode + ")").c_str());
			}
			#if defined(WIN32) || defined(_WIN32)
				clientAddress = SocketAddress(address.sin_family, address.sin_port, address.sin_addr.S_un.S_addr);
			#elif defined(__linux__)
				clientAddress = SocketAddress(address.sin_family, address.sin_port, address.sin_addr.s_addr);
			#endif
			clientSocket = Socket(clientHandle, AF_INET, SOCK_STREAM, IPPROTO_TCP);
		}

		void Connect(const IPv4Address& address)
		{
			int32_t result = connect(m_handle, (sockaddr*)&address.GetSocketAddress().GetStructure(), sizeof(sockaddr));
			if (result != 0)
			{
				std::string errorCode(std::to_string(WNetworkGetErrorNumber()));
				throw WinSockException(std::string("Error accepting connection! (" + errorCode + ")").c_str());
			}
		}

		void Send(const void* data, int32_t size)
		{
			int32_t currentNumBytes{ 0 };
			while (currentNumBytes < size)
			{
				int32_t numBytesSent = SendImplementation((const char*)data + currentNumBytes, size - currentNumBytes);
				currentNumBytes += numBytesSent;
			}
		}

		void Receive(void* data, int32_t size)
		{
			int32_t currentNumBytes{ 0 };
			while (currentNumBytes != size)
			{
				int numBytesReceived = ReceiveImplementation((char*)data + currentNumBytes, size - currentNumBytes);
				currentNumBytes += numBytesReceived;
				if (currentNumBytes == 0)
				{
					throw WinSockException("Client disconnected!\n");
				}
			}
		}

		void ReceiveAll(void* data, int32_t& size)
		{
			int32_t currentNumBytes{ 0 };
			int32_t numBytesReceived{ 0 };
			do
			{
				numBytesReceived = ReceiveImplementation((char*)data + currentNumBytes, Socket::DefaultReceiveSize);
				currentNumBytes += numBytesReceived;
			}
			while (numBytesReceived != 0);
			size = currentNumBytes;
		}
	private:
		int32_t SendImplementation(const void* data, int32_t size)
		{
			int32_t numBytesSent = send(m_handle, (const char*)data, size, 0);
			if (numBytesSent == SOCKET_ERROR)
			{
				std::string errorCode(std::to_string(WNetworkGetErrorNumber()));
				throw WinSockException(std::string("Error sending data! (" + errorCode + ")").c_str());
			}
			return numBytesSent;
		}

		int32_t ReceiveImplementation(void* data, int32_t size)
		{
			int32_t numBytesReceived = recv(m_handle, (char*)data, size, 0);
			if (numBytesReceived == SOCKET_ERROR)
			{
				std::string errorCode(std::to_string(WNetworkGetErrorNumber()));
				throw WinSockException(std::string("Error recieving data! (" + errorCode + ")").c_str());
			}
			return numBytesReceived;
		}

		SOCKET m_handle;
		int16_t m_family;
		int32_t m_type;
		int32_t m_protocol;
		static const int32_t DefaultReceiveSize{ 128 };
};
