#pragma once

#include "SocketAddress.h"
#include "WinSockException.h"

#include <string>
#include <vector>

#if defined(WIN32) || defined(_WIN32)
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WS2tcpip.h>
#elif defined(__linux__)
#include <arpa/inet.h>
#endif

class IPv4Address
{
	public:
	IPv4Address() = default;
	IPv4Address(const std::string& IPAddress, short port)
	{
		m_address.SetAddress(inet_addr(IPAddress.c_str()));
		m_address.SetFamily(AF_INET);
		m_address.SetPort(port);
	}

	IPv4Address(const SocketAddress& socketAddress) :
		m_address{ socketAddress }
	{

	}

	inline const SocketAddress& GetSocketAddress() const { return m_address; }

	std::string GetAddressString()
	{
		std::string stringAddress;
		inet_ntop(AF_INET, &m_address.GetAddress(), &stringAddress[0], IPv4Address::IPStringSize);
		return stringAddress;
	}
	private:
	SocketAddress m_address;
	static const int32_t IPStringSize{ 16 };
};