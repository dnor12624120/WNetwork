#include "NetworkManager.h"
#include "Exception.h"

NetworkManager::NetworkManager()
{

}

std::shared_ptr<NetworkManager> NetworkManager::GetInstance()
{
	static std::shared_ptr<NetworkManager> manager(new NetworkManager());
	return manager;
}

void NetworkManager::Initialize(int version, int subversion)
{
	if (WSAStartup(MAKEWORD(version, subversion), &m_data) != 0)
	{
		throw WNManagerException("Error initializing WSA.");
	}
	m_version = version;
	m_subversion = subversion;
}

void NetworkManager::Cleanup()
{
	WSACleanup();
}