#pragma once

#define WIN32_LEAN_AND_MEAN //Define to skip a large number of useless windows-related includes.

#include "Types.h"
#include <memory>
#include <winsock2.h>

/* Singleton manager class for initializing and closing WSA. */

namespace WNetwork
{
	class NetworkManager
	{
		public:
			NetworkManager(const NetworkManager&) = delete;
			NetworkManager& operator=(const NetworkManager&) = delete;

			static std::shared_ptr<NetworkManager> GetInstance();

			void Initialize(int version, int subversion);
			void Cleanup();

			inline const int GetVersion() const { return m_version; }
			inline const int GetSubversion() const { return m_subversion; }
		private:
			NetworkManager();

			WNetwork::Types::WSAData m_data;
			int m_version;
			int m_subversion;
	};
}
