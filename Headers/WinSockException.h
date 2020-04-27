#pragma once

#include <exception>
#include <string>

class WinSockException : public std::exception
{
	public:
	explicit WinSockException(const std::string& message) :
		m_message{ message }
	{

	}

	virtual const char* what() const throw() override { return m_message.c_str(); }
	private:
	std::string m_message;
};