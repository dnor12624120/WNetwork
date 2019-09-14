#include "FileDescriptorSet.h"
#include "Socket.h"

using namespace WNetwork::Types;

void WNetwork::FileDescriptorSet::Clear()
{
	FD_ZERO(&m_set);
}

void WNetwork::FileDescriptorSet::AddSocket(Socket& socket)
{
	FD_SET(socket.GetHandle(), &m_set);
}

void WNetwork::FileDescriptorSet::RemoveSocket(Socket& socket)
{
	FD_CLR(socket.GetHandle(), &m_set);
}

int WNetwork::FileDescriptorSet::Select()
{
	return select(0, &m_set, NULL, NULL, &m_timeout);
}

void WNetwork::FileDescriptorSet::Cleanup()
{
	while (m_set.fd_count)
	{
		Socket socket(m_set.fd_array[0], Family_IPv4, SocketType_Stream, Protocol_TCP);
		RemoveSocket(socket);
		socket.Close();
	}
}

void WNetwork::FileDescriptorSet::SetTimeout(int seconds, int miliseconds)
{
	m_timeout.tv_sec = seconds;
	m_timeout.tv_usec = miliseconds;
}
