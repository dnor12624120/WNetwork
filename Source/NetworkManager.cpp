#include "NetworkManager.h"
#include "Exception.h"

WNetwork::NetworkManager::NetworkManager()
{

}

std::shared_ptr<WNetwork::NetworkManager> WNetwork::NetworkManager::GetInstance()
{
	static std::shared_ptr<NetworkManager> manager(new NetworkManager());
	return manager;
}

void WNetwork::NetworkManager::Initialize(int version, int subversion)
{
	if (WSAStartup(MAKEWORD(version, subversion), &m_data) != 0)
	{
		throw WNManagerException("Error initializing WSA.");
	}
	m_version = version;
	m_subversion = subversion;
}

void WNetwork::NetworkManager::Cleanup()
{
	WSACleanup();
}
