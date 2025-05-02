#include "_6_PIMPL/pimpl_classes.hpp"
#include <thread>
#include <vector>
#include <format>

int main()
{
	pimplTests::Person person{ "Dustin", 40 };
	person.Introduce();
	
	std::vector<std::thread> threads;
	for (int i = 0; i < 5; i++)
	{
		threads.emplace_back(
			[i] 
			{
				for (int j = 0; j < 5; j++)
				{
					pimplTests::Logger::GetInstance().Log(
						std::format("Thread {} - Message {}", i, j)
					);
				}
			}
		);
	}

	for (auto& thread : threads)
		thread.join();

	pimplTests::Logger::GetInstance().Log("All threads are finished!");

	return 0;
}