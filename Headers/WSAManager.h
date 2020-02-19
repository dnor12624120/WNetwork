#pragma once

#include <iostream>
#include <memory>
#include <WinSock2.h>

#include "WinSockException.h"

class WSAManager
{
	public:
		WSAManager(const WSAManager&) = delete;
		WSAManager& operator=(const WSAManager&) = delete;
		~WSAManager()
		{
			WSACleanup();
		}

		static std::shared_ptr<WSAManager> GetInstance()
		{
			static std::shared_ptr<WSAManager> instance{ new WSAManager() };
			return instance;
		}

	private:
		WSAManager()
		{
			if (WSAStartup(MAKEWORD(2, 2), &m_startupData) != 0)
			{
				throw WinSockException("Failed to initialize WSA.");
			}
		}

		WSADATA m_startupData;
};
