#pragma once
#include <memory>
#include <string>

namespace pimplTests
{

	class Person
	{
	public:
		Person(const std::string& name, int age);
		~Person(); // Required for a unique_ptr with an incomplete type

		void Introduce() const;

	private:
		class Impl;
		std::unique_ptr<Impl> m_pImpl;
	};

	class Logger
	{
	public:
		static Logger& GetInstance();
		void Log(const std::string& message);

	private:
		Logger(); // Private Ctor for singleton
		~Logger();
		Logger(const Logger&) = delete;
		Logger& operator=(const Logger&) = delete;

		class Impl;
		std::unique_ptr<Impl> m_pImpl;
	};
}
