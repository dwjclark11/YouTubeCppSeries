#include <iostream>
#include <memory>

void RawPointerExamples()
{
	int val = 42;
	int* ptr = &val;
	std::cout << "Stack Address: " << ptr << std::endl;
	std::cout << "Val: " << *ptr << std::endl;

	int* ptr2 = new int(42);
	std::cout << "Heap Address: " << ptr2 << std::endl;
	std::cout << "Val: " << *ptr2 << std::endl;
	delete ptr2;

	/* This function will leak memory. */
	auto badFunction = [] 
	{ 
		int* ptr = new int(69); 
		std::cout << "Leaked Address: " << ptr << std::endl;
		return *ptr; 
	};
	int val2 = badFunction();
	std::cout << "Leaked value: "<< val2 << std::endl;
}

void UniquePtrExamples()
{
	auto ptr1 = std::make_unique<int>(89);
	std::cout << "Address: " << ptr1 << std::endl;
	std::cout << "Val: " << *ptr1 << std::endl;;

	auto ptr2 = std::move(ptr1);
	std::cout << "Address2: " << ptr2 << std::endl;
	std::cout << "Val2: " << *ptr2 << std::endl;

	if (!ptr1)
	{
		std::cout << "ptr1 is no longer valid. The value has been moved" << std::endl;
	}
}

void SharedPtrExamples()
{
	auto sharedPtr1 = std::make_shared<int>(9874);
	std::cout << "Ref Count: " << sharedPtr1.use_count() << std::endl;
	std::cout << "Ptr1 Address: " << sharedPtr1 << std::endl;
	std::cout << "Ptr1 Val: " << *sharedPtr1 << std::endl;
	{
		auto sharedPtr2 = sharedPtr1;
		std::cout << "Ref Count: " << sharedPtr1.use_count() << std::endl;
		std::cout << "Ptr2 Address: " << sharedPtr2 << std::endl;
		std::cout << "Ptr2 Val: " << *sharedPtr2 << std::endl;
		{
			auto sharedPtr3 = sharedPtr2;
			std::cout << "Ref Count: " << sharedPtr1.use_count() << std::endl;
			std::cout << "Ptr3 Address: " << sharedPtr3 << std::endl;
			std::cout << "Ptr3 Val: " << *sharedPtr3 << std::endl;
		}
		std::cout << "Ref Count: " << sharedPtr1.use_count() << std::endl;
	}

	std::cout << "Final Ref Count: " << sharedPtr1.use_count() << std::endl;

	auto newPtr = sharedPtr1;

	/* Shared Ptrs can also be moved. We can transfer ownership. */
	auto transferedPtr = std::move(sharedPtr1);
	std::cout << "Transfered Address: " << transferedPtr << std::endl;
	std::cout << "Transfered Val: " << *transferedPtr<< std::endl;

	if (!sharedPtr1)
	{
		std::cout << "Shared Pointer ownership has been transferred." << std::endl;
	}

	std::cout << "Transferred Count: " << transferedPtr.use_count() << std::endl;
}

struct B;

struct A
{
	std::shared_ptr<B> bPtr;
	~A() { std::cout << "A Destroyed\n"; }
};

struct B
{
	std::weak_ptr<A> aPtr;
	~B() { std::cout << "B Destroyed\n"; }
};

void WeakPtrExamples()
{
	std::shared_ptr<A> a = std::make_shared<A>();
	std::shared_ptr<B> b = std::make_shared<B>();

	// Set the two underlying pointers to point at each other
	a->bPtr = b;
	b->aPtr = a; // The weak ptr in b, prevents the reference cycle

	// Both Objects will be properly destroyed
}

void LegacyFunction(int* rawPtr)
{
	// Change te value
	*rawPtr += 47;
	std::cout << "Legacy Function takes a raw ptr: " << *rawPtr << std::endl;
}

int main()
{
	// This example has a memory leak -- Try running it in a while loop.
	// Watch your memory explode!!
	//RawPointerExamples();
	std::cout << "\n=============================\n";
	std::cout << "Unique Ptr Examples\n";
	UniquePtrExamples();
	std::cout << "\n=============================\n";
	std::cout << "Shared Ptr Examples\n";
	SharedPtrExamples();
	std::cout << "\n=============================\n";
	std::cout << "Weak Ptr Examples\n";
	WeakPtrExamples();
	std::cout << "\n=============================\n";
	std::cout << "Legacy Function Example\n";
	auto ptr1 = std::make_unique<int>(74);
	std::cout << "Old Value: " << *ptr1 << "\n";
	LegacyFunction(ptr1.get());
	std::cout << "Value after legacy function: " << *ptr1 << std::endl;

	return 0;
}