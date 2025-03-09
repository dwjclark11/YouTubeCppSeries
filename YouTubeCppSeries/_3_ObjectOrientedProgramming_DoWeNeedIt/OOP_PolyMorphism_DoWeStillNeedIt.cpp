#include <iostream>
#include <memory>
#include <vector>
#include <concepts>
#include <algorithm>
#include <variant>

/*
* Object Oriented Programming 
* - Four Main Principles
* 1) Encapsulation	- Bundling Data and Behavior inside a class.
* 2) Abstraction	- Hiding implementation details from the user.
* 3) Inheritance	- Creating a new class from an existing one.
* 4) Polymorphism	- Treating different classes through a common interface
*/

/*
* Classic Polymorphism with Virtual Functions
*/

class Animal
{
public:
	virtual ~Animal() = default;
	virtual void Speak() const = 0; // Pure virtual function
};

class Dog : public Animal
{
public:
	virtual void Speak() const override
	{
		std::cout << "Woof!\n";
	}
};

class Cat: public Animal
{
public:
	virtual void Speak() const override
	{
		std::cout << "Meow!\n";
	}
};

// Polymorphic Behavior
void MakeSound(const Animal& animal)
{
	animal.Speak();
}

void BasicOOPBehaviorAndPolyMorphism()
{
	// Using basic OOP and PolyMorphism
	std::vector<std::unique_ptr<Animal>> animals;
	animals.push_back(std::make_unique<Dog>());
	animals.push_back(std::make_unique<Cat>());
	// ... Add more animals if desired
	
	std::cout << "\n==========================================\n";
	std::cout << "Calling Speak for each animal.\n";
	std::cout << "==========================================\n";

	for (const auto& animal : animals)
	{
		// Calls the derived class method
		animal->Speak();
	}

	std::cout << "\n==========================================\n";
	std::cout << "Calling MakeSound using Ranges.\n";
	std::cout << "==========================================\n";
	std::ranges::for_each(animals, [](auto& animal) {MakeSound(*animal); });

	/*
	* How This Works:
	* - The base class (Animal) has a pure virtual function (Speak).
	* - Derived classes (Dog, Cat) override it.
	* - Dynamic dispatch determines which function to call at runtime.
	*/
}

/*
* Polymorphism using std::variant.
* - We can create new Dog and Cat classes, without using the Animal Base class
*/
class Doggy
{
public:
	void Speak() const 
	{
		std::cout << "Woof!\n";
	}
};

class Kitty
{
public:
	void Speak() const
	{
		std::cout << "Meow!\n";
	}
};

/*
* From here we can create an alias to an std::variant
* for Cats and Dogs
*/
using VarAnimal = std::variant<Doggy, Kitty>;

void MakeSound(const VarAnimal& animal)
{
	std::visit([](const auto& a) {a.Speak(); }, animal);
}

void OOPBehaviorUsingVariant()
{
	VarAnimal myDog = Doggy{};
	VarAnimal myCat = Kitty{};

	std::cout << "\n==========================================\n";
	std::cout << "Calling Speak for each VarAnimal.\n";
	std::cout << "This is using std::variant and std::visit.\n";
	std::cout << "==========================================\n";
	MakeSound(myDog);
	MakeSound(myCat);
}

/*
* So why use std::variant?
* - No virtual functions	-> No vtable overhead
* - Memory-efficient		-> Stores only one type at a time.
* - Compile-time safety		-> Catches missing cases at compile-time
* 
* Downsides?
* - Less flexible			-> You must explicitly list all possible types in std::variant
*/

/*
* Polymorphism using concepts
*/

// Example: templates without using concepts
template <typename T>
void MakeSoundTemplate(const T& animal)
{
	// This will work only if T has speak
	animal.Speak();
}

/*
* Pros: Works with any type that has a Speak() method
* Cons: If T doesn't have Speak(), the compiler error can be cryptic and hard to debug.
*/


/*
* Let's improve this with concepts
*/
template <typename T>
concept AnimalConcept = requires(T a)
{
	{ a.Speak() } ->std::same_as<void>;
};

// Now, any type that we use must have the Speak() func
template <AnimalConcept T>
void MakeSoundConcept(const T& animal)
{
	// Compile-Time Polymorphism!!
	animal.Speak();
}

class Car 
{  
public:
	void Honk() const { std::cout << "Beep!\n"; }
};

void OOPUsingConcepts()
{
	std::cout << "\n==========================================\n";
	std::cout << "Calling Speak for each animal.\n";
	std::cout << "This is using the template function with concepts.\n";
	std::cout << "==========================================\n";
	MakeSoundConcept(Dog{});
	MakeSoundConcept(Cat{});

	// This will instantly get a compiler error!
	// Car does not have Speak()
	
	// MakeSoundConcept(Car{});

	// This does not have an error; however, it will not compile.
	// MakeSoundTemplate(Car{});
	// The error is actually pretty good, 'Speak' is not a member of Car
}

int main()
{
	BasicOOPBehaviorAndPolyMorphism();
	OOPBehaviorUsingVariant();
	OOPUsingConcepts();

	return 0;
}