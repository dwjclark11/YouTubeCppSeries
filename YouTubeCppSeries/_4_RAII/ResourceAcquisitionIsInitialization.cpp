#include <iostream>
#include <fstream>
#include <memory>
#include <thread>
#include <mutex>
#include <fmt/format.h>

/*
* RAII or Resource Acquisition Is Initialization, is a fundemental technique
* that makes memory management, file handling, and even thread management safer
* and easier.
* 
* What is RAII - RAII is a design pattern where resource allocation and deallocation 
* are tied to object lifetime.
* 
* This means when an object is created, it acquires a resource, and when it goes out
* of scope, it releases the resource automatically.
* 
*/

class FileHandler
{
public:
	FileHandler(const std::string& sFilename)
		: sFilename{ sFilename }
	{
		file.open(sFilename);
		if (!file.is_open())
		{
			throw std::runtime_error(fmt::format("Failed to open file [{}]", sFilename));
		}

		fmt::print("File opened: {}\n", sFilename);
	}

	~FileHandler()
	{
		if (file.is_open())
			file.close();

		fmt::print("File [{}] was closed.\n", sFilename);
	}

private:
	std::ofstream file;
	std::string sFilename;
};

void RAIIFileHandlerTest()
{
	try
	{
		FileHandler fHandler{ "test.txt" };

	}
	catch (const std::exception& ex)
	{
		fmt::report_error(ex.what());
	}
}

class Resource
{
public:
	Resource() { fmt::print("Resource Acquired.\n"); }
	~Resource() { fmt::print("Resource Released.\n"); }
};

/*
* The std::unique_ptr also uses RAII, it automatically releases the 
* allocated memory/resource as soon as it goes out of scope.
* The Resource class would also work if we just used a local stack 
* variable.
*/
void RAIIUseUniqueResource()
{
	auto pResource = std::make_unique<Resource>();
	fmt::print("Using Resource...\n");
} // Resouce should be automatically released.

std::mutex gMtx;

void ThreadSafeFunction()
{
	/*
	* The lock_guard is using RAII. 
	* Once the lock goes out of scope, it will automatically
	* unlock it self, therefore no need for manual unlock calls
	*/
	std::lock_guard lock(gMtx);
	fmt::print("Thread-safe operation!!\n");
} // The lock guard is automatically unlocked here

/*
* Let's create a new class that Allocates something upon creation and than
* deallocates upon destruction
*/
class Allocator
{
public:
	Allocator() 
	{ 
		pResource = new Resource(); 
		fmt::print("Allocator -- Allocated a new resource.\n");
	}

	~Allocator() 
	{ 
		if (pResource) 
		{
			delete pResource;
			fmt::print("Allocator -- deallocated the resource.\n");
		}
	}

private:
	Resource* pResource{ nullptr };
};

void RunAllocatorTest()
{
	auto pAllocator = std::make_unique<Allocator>();
}

int main()
{
	RAIIFileHandlerTest();
	RAIIUseUniqueResource();
	
	std::thread t1{ ThreadSafeFunction };
	std::thread t2{ ThreadSafeFunction };

	t1.join();
	t2.join();

	//while(true)
	//{
	//	RunAllocatorTest();
	//}

	return 0;
}