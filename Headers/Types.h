#pragma once

#include <WinSock2.h>

namespace WNetwork
{
	namespace Types
	{
		const int Family_Unspecified = AF_UNSPEC;
		const int Family_IPv4 = AF_INET;
		const int Family_IPX = AF_IPX;
		const int Family_AppleTalk = AF_APPLETALK;
		const int Family_NetBIOS = AF_NETBIOS;
		const int Family_IPv6 = AF_INET6;
		const int Family_Infrared = AF_IRDA;
		const int Family_Bluetooth = AF_BTH;

		const int SocketType_Stream = SOCK_STREAM;
		const int SocketType_Datagram = SOCK_DGRAM;
		const int SocketType_Raw = SOCK_RAW;
		const int SocketType_ReliableDatagram = SOCK_RDM;
		const int SocketType_SeqPacket = SOCK_SEQPACKET;

		const int Protocol_ICMP = IPPROTO_ICMP;
		const int Protocol_IGMP = IPPROTO_IGMP;
		const int Protocol_TCP = IPPROTO_TCP;
		const int Protocol_UDP = IPPROTO_UDP;
		const int Protocol_ICMPv6 = IPPROTO_ICMPV6;

		using SocketAddressFamily = int;
		using SocketType = int;
		using SocketProtocol = int;
		using SocketHandle = SOCKET;
		using InSocketAddress = sockaddr_in;
		using InAddress = in_addr;
		using AddressInfo = addrinfo;
		using SocketAddress = sockaddr;
		using TimeValue = timeval;
		using WSAData = WSADATA;
	}
}

