#include "IPv4Address.h"

#include <WS2tcpip.h>
#include <cstring>
#include <exception>

IPv4Address::IPv4Address(const std::string& ip, unsigned short port):
	m_hostname(ip),
	m_ip(ip),
	m_port(port)
{
	InAddress address;
	if (inet_pton(Family_IPv4, m_ip.c_str(), &address) == 1)
	{
		if (address.S_un.S_addr != INADDR_NONE)
		{
			m_ipBytes.resize(sizeof(unsigned long));
			memcpy(&m_ipBytes[0], &address.S_un.S_addr, sizeof(unsigned long));
			return;
		}
	}
	AddressInfo hints = {};
	hints.ai_family = Family_IPv4;
	AddressInfo* info = NULL;
	if (getaddrinfo(ip.c_str(), NULL, &hints, &info) == 0)
	{
		InSocketAddress* socketAddress = (InSocketAddress*)info->ai_addr;
		m_ip.resize(16);
		inet_ntop(Family_IPv4, &socketAddress->sin_addr, (PSTR)&m_ip[0], 16);
		m_ipBytes.resize(sizeof(unsigned long));
		memcpy(&m_ipBytes[0], &(socketAddress->sin_addr.S_un.S_addr), sizeof(unsigned long));
		return;
	}
}

IPv4Address::IPv4Address(SocketAddress* address)
{
	InSocketAddress* inAddress = (InSocketAddress*)address;
	m_port = ntohs(inAddress->sin_port);
	m_ipBytes.resize(sizeof(unsigned long));
	memcpy(&m_ipBytes[0], &inAddress->sin_addr, sizeof(unsigned long));
	m_ip.resize(16);
	inet_ntop(Family_IPv4, &inAddress->sin_addr, (PSTR)&m_ip[0], 16);
	m_hostname = m_ip;
}

InSocketAddress IPv4Address::GetSocketAddress() const
{
	InSocketAddress addr = {};
	memcpy(&addr.sin_addr, &m_ipBytes[0], sizeof(unsigned long));
	addr.sin_port = htons(m_port);
	addr.sin_family = Family_IPv4;
	return addr;
}