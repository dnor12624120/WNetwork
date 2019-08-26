#include "Socket.h"
#include "IPv4Address.h"
#include "Exception.h"

#include <exception>
#include <iostream>

Socket::Socket(SocketAddressFamily family, SocketType type, SocketProtocol protocol) :
	m_family(family),
	m_type(type),
	m_protocol(protocol),
	m_backlog(DEFAUKLT_BACKLOG)
{

}

Socket::Socket(SocketHandle handle, SocketAddressFamily family, SocketType type, SocketProtocol protocol):
	m_handle(handle),
	m_family(family),
	m_type(type),
	m_protocol(protocol),
	m_backlog(DEFAUKLT_BACKLOG)
{

}

void Socket::Create()
{
	m_handle = socket(m_family, m_type, m_protocol);
	if (m_handle == INVALID_SOCKET)
	{
		throw WNInvalidOperationException("Error creating socket!");
	}
}

void Socket::Close()
{
	if (m_handle == INVALID_SOCKET)
	{
		throw WNInvalidOperationException("Trying to close an invalid socket!");
	}
	if (closesocket(m_handle) == SOCKET_ERROR)
	{
		throw WNInvalidOperationException("Error closing socket!");
	}
}

void Socket::ToggleNagle(bool on)
{
	BOOL value = on ? TRUE : FALSE;
	if (setsockopt(m_handle, Protocol_TCP, TCP_NODELAY, (const char*)&value, sizeof(value)) != 0)
	{
		std::string errorCode(std::to_string(WSAGetLastError()));
		throw WNInvalidOperationException(std::string("Error toggling Nagle's algorithm! (" + errorCode + ")").c_str());
	}
}

void Socket::Bind(const IPv4Address& address)
{
	if (bind(m_handle, (sockaddr*)&address.GetSocketAddress(), sizeof(sockaddr_in)) != 0)
	{
		std::string errorCode(std::to_string(WSAGetLastError()));
		throw WNInvalidOperationException(std::string("Error binding socket! (" + errorCode + ")").c_str());
	}
}

void Socket::Listen()
{
	if (listen(m_handle, m_backlog) != 0)
	{
		std::string errorCode(std::to_string(WSAGetLastError()));
		throw WNInvalidOperationException(std::string("Error listening to socket! (" + errorCode + ")").c_str());
	}
}

void Socket::Accept(Socket& recievedSocket, IPv4Address& socketAddress)
{
	InSocketAddress address = {};
	int size = sizeof(InSocketAddress);
	int handle = accept(m_handle, (SocketAddress*)&address, &size);
	
	if (handle == INVALID_SOCKET)
	{
		std::string errorCode(std::to_string(WSAGetLastError()));
		throw WNInvalidOperationException(std::string("Error accepting connection! (" + errorCode + ")").c_str());
	}
	socketAddress = IPv4Address((SocketAddress*)&address);
	recievedSocket = Socket(handle, Family_IPv4, SocketType_Stream, Protocol_TCP);
}

void Socket::Connect(const IPv4Address& address)
{
	int result = connect(m_handle, (SocketAddress*)&address.GetSocketAddress(), sizeof(SocketAddress));
	if (result != 0)
	{
		std::string errorCode(std::to_string(WSAGetLastError()));
		throw WNInvalidOperationException(std::string("Error accepting connection! (" + errorCode + ")").c_str());
	}
}

void Socket::SendAll(const void* data, int size)
{
	int currentSize = 0;
	while (currentSize < size)
	{
		int sentBytes = Send((const char*)data + currentSize, size - currentSize);
		currentSize += sentBytes;
	}
}

void Socket::RecieveAll(void* data, int size)
{
	int currentSize = 0;
	while (currentSize != size)
	{
		int recievedBytes = Recieve((char*)data + currentSize, size - currentSize);
		currentSize += recievedBytes;
	}
}

int Socket::Send(const void* data, int size)
{
	int bytesSent = send(m_handle, (const char*)data, size, 0);
	if (bytesSent == SOCKET_ERROR)
	{
		std::string errorCode(std::to_string(WSAGetLastError()));
		throw WNInvalidOperationException(std::string("Error sending data! (" + errorCode + ")").c_str());
	}
	return bytesSent;
}

int Socket::Recieve(void* data, int size)
{
	int bytesSent = recv(m_handle, (char*)data, size, 0);
	if (bytesSent == 0)
	{
		throw WNDisconnectException("Client disconnected!\n");
	}
	if (bytesSent == SOCKET_ERROR)
	{
		std::string errorCode(std::to_string(WSAGetLastError()));
		throw WNInvalidOperationException(std::string("Error recieving data! (" + errorCode + ")").c_str());
	}
	return bytesSent;
}