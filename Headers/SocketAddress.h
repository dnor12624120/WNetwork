#pragma once

#if defined(WIN32) || defined(_WIN32)
	#include <WS2tcpip.h>
#elif defined(__linux__)
	#include <netinet/in.h>
	#define sockaddr_in struct sockaddr_in
#endif

#if defined(SetPort)
	#undef SetPort
#endif

class SocketAddress
{
	public:
		SocketAddress() = default;

		SocketAddress(unsigned short familyType, unsigned short port, unsigned long address)
		{
			m_address.sin_family = familyType;
			m_address.sin_port = port;
			#if defined(WIN32) || defined(_WIN32)
				m_address.sin_addr.S_un.S_addr = address;
			#elif defined(__linux__)
				m_address.sin_addr.s_addr = address;
			#endif
		}

		inline uint16_t GetFamilyType() const { return m_address.sin_family; }
		inline uint16_t GetPort() { return m_address.sin_port; }

		#if defined(WIN32) || defined(_WIN32)
			inline ULONG& GetAddress()
			{
				return m_address.sin_addr.S_un.S_addr;
			}
		#elif defined(__linux__)
			inline uint32_t& GetAddress()
			{
				return m_address.sin_addr.s_addr;
			}
		#endif

		inline const sockaddr_in& GetStructure() const { return m_address; }

		inline void SetFamily(unsigned short familyType) { m_address.sin_family = familyType; }
		inline void SetPort(unsigned short port) { m_address.sin_port = port; }
		inline void SetAddress(unsigned long address)
		{
			#if defined(WIN32) || defined(_WIN32)
				m_address.sin_addr.S_un.S_addr = address;
			#elif defined(__linux__)
				m_address.sin_addr.s_addr = address;
			#endif
		}
	private:
		sockaddr_in m_address;
};

