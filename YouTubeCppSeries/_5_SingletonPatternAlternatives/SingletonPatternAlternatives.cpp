#include <memory>
#include <mutex>
#include <vector>
#include <thread>
#include <iostream>

#include <fmt/format.h>

/*
* Singleton Pattern
* - Ensures only one instance of a class exists
* - Global acces to that instance
*/
class Logger
{
public:
	static Logger& GetInstance()
	{
		static Logger instance;
		return instance;
	}

	void Log(const std::string& message)
	{
		fmt::print("[LOG]: {}\n", message);
	}

private:
	Logger() = default;
	~Logger() = default;
	Logger(const Logger&) = delete;
	Logger& operator=(const Logger&) = delete;
};

void RunSingletonLogger()
{
	Logger::GetInstance().Log("Singlton Logger in action!");
}

/*
* CONS of the Singleton:
* - Global State = Hidden dependencies
* - Hard to test (Mocks? etc)
* - Tight coupling
*/

/*
* The Monostate Pattern
* 
* - Looks like a regular class, but under the hood, it shares state between instances
* - Here, every logger object shares the same state. But unlike singletons, Monostate
* can be copied and moved.
* 
* Pros:
* - Easier to test 
* - Still a single global state
* - No need for GetInstance()
* 
* Cons:
* - Still hidden dependencies
* - Global state is still global
*/
class MonoLogger
{
public:
	void Log(const std::string& message)
	{
		std::lock_guard lock{ mutex };
		fmt::print("[LOG]: {}\n", message);
	}
private:
	// Shared state
	inline static std::mutex mutex;
};

void RunMonostateLogger()
{
	MonoLogger log1{};
	log1.Log("MonoLogger 1 used.");

	MonoLogger log2{};
	log2.Log("MonoLogger 2 used.");
}

void RunMonoFromThread(int threadIndex)
{
	MonoLogger logger{};

	for (int i = 0; i < 5; ++i)
	{
		logger.Log(fmt::format("Message: {} from thread {}", i, threadIndex));
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

void CreateMonoThreads()
{
	constexpr int threadCount = 4;

	std::vector<std::thread> threads;

	for (int i = 0; i < threadCount; ++i)
	{
		threads.emplace_back(RunMonoFromThread, i);
	}

	for (auto& t : threads)
	{
		t.join();
	}
}

/*
* Dependency Injection
* We can pass in any dependencies that we need.
* Pros:
* - Testable(Pass a fake logger)
* - No global state!
* - More control
* 
* Cons:
* - More boilerplate
* - You have to pass dependencies manually.
* 
*/
class DILogger
{
public:
	DILogger() = default;
	void Log(const std::string& message)
	{
		fmt::print("{}{}\n", sPrefix, message);
	}
private:
	std::string sPrefix{ "[LOG]: " };
};

/*
* Now we just pass in the logger to another class
* as a dependency.
*/
class Service
{
public:
	Service(DILogger& logger) : logger{logger} {}

	void DoSomething()
	{
		logger.Log("Dependency Injection in action!");
	}

private:
	DILogger& logger;
};

void RunDependencyInjection()
{
	DILogger logger{};
	Service service{ logger };
	service.DoSomething();
}

/*
* What if we don't want to pass along the dependencies manually?
*/

/*
* Service Locator
* - Hides the complexity of dependency injection while avoiding the issues 
* of a singleton.
* 
* Pros:
* - Flexible!
* - Can swap services easily.
* - Less boilerplate than pure DI
* 
* Cons:
* - Still kinda global
* - Still some hidden dependencies.
* - 
*/

class ServiceLocator
{
public:
	template <typename T>
	static void Provide(std::shared_ptr<T> service)
	{
		instance<T>() = std::move(service);
	}

	template <typename T>
	static T& Get()
	{
		return *instance<T>();
	}

private:
	template <typename T>
	static std::shared_ptr<T>& instance()
	{
		static std::shared_ptr<T> serviceInstance;
		return serviceInstance;
	}
};

/*
* Now we can register services dynamically
*/

void RunServiceLocator()
{
	/* We provide the logger to the service locator. */
	ServiceLocator::Provide<DILogger>(std::make_shared<DILogger>());

	/* Get the logger when we need it. */
	auto& logger = ServiceLocator::Get<DILogger>();
	logger.Log("Service locator in Action!");
}

/*
* Which one should you use? 
* 
* - Singleton -> When you really need one instance (Like a window manager)
* - Monostate -> When you want shared state without singleton restrictions.
* - Dependency Injection -> When testability and flexibility matter.
* - Service Locator -> When you need global access but want some more flexibility.
* 
* No pattern is inherently bad, it depends on your needs!
*/

int main()
{
	RunSingletonLogger();
	RunMonostateLogger();
	RunDependencyInjection();
	RunServiceLocator();
	return 0;
}