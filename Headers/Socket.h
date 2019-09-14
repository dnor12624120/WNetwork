#pragma once

#define WIN32_LEAN_AND_MEAN

#include "Types.h"
#include "IPv4Address.h"
#include <WinSock2.h>

class Socket
{
	public:
		Socket() = default;
		Socket(WNetwork::Types::SocketAddressFamily family, WNetwork::Types::SocketType type, WNetwork::Types::SocketProtocol protocol);
		Socket(WNetwork::Types::SocketHandle handle, WNetwork::Types::SocketAddressFamily family,
			   WNetwork::Types::SocketType type, WNetwork::Types::SocketProtocol protocol);

		void Create();
		void Close();
		void ToggleNagle(bool on);
		void Bind(const WNetwork::IPv4Address& address);
		void Listen();
		void Accept(Socket& recievedSocket, WNetwork::IPv4Address& socketAddress);
		void Connect(const WNetwork::IPv4Address& address);
		void Send(const void* data, int size);
		void Receive(void* data, int size);
		void ReceiveAll(void* data, int& size);

		inline void SetBacklog(int backlog) { m_backlog = backlog; }
		inline const WNetwork::Types::SocketHandle GetHandle() const { return m_handle; }
		inline const WNetwork::Types::SocketAddressFamily GetFamily() const { return m_family; }
		inline const WNetwork::Types::SocketType GetType() const { return m_type; }
		inline const WNetwork::Types::SocketProtocol GetProtocol() const { return m_protocol; }

		inline const bool operator==(const Socket& other) { return m_handle == other.m_handle; }
	private:
		int SendImplementation(const void* data, int size);
		int ReceiveImplementation(void* data, int size);

		WNetwork::Types::SocketHandle m_handle;
		WNetwork::Types::SocketAddressFamily m_family;
		WNetwork::Types::SocketType m_type;
		WNetwork::Types::SocketProtocol m_protocol;

		int m_backlog;

		static const int DefaultReceiveSize{ 1024 };
		static const int DefaultBacklogValue{ 8 };
};
