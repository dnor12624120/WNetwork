#pragma once

#if defined(WIN32) || defined(_WIN32)
    #include <WinSock2.h>
#endif

#include "Socket.h"

class Socket;

class FileDescriptorSet
{
	public:
		FileDescriptorSet() = default;
		FileDescriptorSet(const FileDescriptorSet& fds) = default;

		void Zero()
		{
			FD_ZERO(&m_set);
		}

		void AddSocket(Socket& socket)
		{
			FD_SET(socket.GetHandle(), &m_set);
		}

		void SetTimeout(int seconds, int miliseconds)
		{
			m_timeout.tv_sec = seconds;
			m_timeout.tv_usec = miliseconds;
		}

		int Select()
		{
			return select(FD_SETSIZE, &m_set, NULL, NULL, &m_timeout);
		}

		inline fd_set& GetSet() { return m_set; }
		inline SOCKET* GetArray() { return m_set.fd_array; }
	private:
	    fd_set m_set;
	    timeval m_timeout;
};