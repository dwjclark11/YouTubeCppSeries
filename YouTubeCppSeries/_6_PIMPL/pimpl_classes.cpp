#include "pimpl_classes.hpp"
#include <iostream>
#include <fstream>
#include <mutex>

namespace pimplTests 
{
// Now we want to define the inner declared class
class Person::Impl
{
public:
	Impl(const std::string name, int age)
		: m_Name{ name }, m_Age{ age }
	{

	}

	void Introduce() const
	{
		std::cout << "Hello, I'm " << m_Name << " and I'm " << m_Age << " years old.\n";
	}

private:
	std::string m_Name;
	int m_Age;
};

// Forward constructor calls to Impl
Person::Person(const std::string& name, int age)
	: m_pImpl{ std::make_unique<Person::Impl>(name, age)}
{
}

Person::~Person() = default;

void Person::Introduce() const
{
	m_pImpl->Introduce();
}


class Logger::Impl
{
public:
	Impl()
	{
		m_LogFile.open("log.txt", std::ios::app);
	}
	~Impl()
	{
		if (m_LogFile.is_open())
		{
			m_LogFile.close();
		}
	}

	void Log(const std::string& message)
	{
		std::lock_guard lock{ m_Mutex };
		std::cout << "[LOG]: " << message << std::endl;
		if (m_LogFile.is_open())
		{
			m_LogFile << message << std::endl;
		}
	}

private:
	std::ofstream m_LogFile;
	std::mutex m_Mutex;
};

Logger& Logger::GetInstance()
{
	static Logger instance{};
	return instance;
}

Logger::Logger() : m_pImpl{ std::make_unique<Logger::Impl>() }
{

}
Logger::~Logger() = default;

void Logger::Log(const std::string& message)
{
	m_pImpl->Log(message);
}

} 