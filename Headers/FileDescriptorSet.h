#pragma once

#include <WinSock2.h>
#include "Types.h"

class Socket;

namespace WNetwork
{
	class FileDescriptorSet
	{
		public:
			FileDescriptorSet() = default;

			void Clear();
			void AddSocket(Socket& socket);
			void RemoveSocket(Socket& socket);
			void Cleanup();
			void SetTimeout(int seconds, int miliseconds);

			int Select();

			inline FD_SET& GetSet() { return m_set; }
			inline WNetwork::Types::SocketHandle* GetArray() { return m_set.fd_array; }
		private:
			FD_SET m_set;
			WNetwork::Types::TimeValue m_timeout;
	};
}