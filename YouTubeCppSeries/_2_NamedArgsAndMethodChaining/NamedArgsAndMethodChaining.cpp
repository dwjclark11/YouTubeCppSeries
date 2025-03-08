#include <iostream>
#include <string>

// ===================================================================================
// Named Arguments
// ===================================================================================
/*
* This function has a lot of parameters. 
* What if we need to add more parameters to the character?
* If we use this method, there will just be more and more parameters
*/
void CreateCharacter(const std::string& sName, int health, int mana, int level, bool bIsNPC)
{
	std::cout << "Creating Old Character " << sName << " with HP: " << health
		<< " and MP: " << mana << "\n";
}

void CallOldCreateCharacterExample()
{
	// Just looking at this code, we don't really know what these numbers mean.
	// They could be anything and if you are new to the code base, we need to spend
	// Extra time trying to determine what this does.
	CreateCharacter("Jadeite", 147, 56, 7, false);
}

/*
* Solution - Create a new struct with named fields
*/
struct CharacterParams
{
	std::string sName{ "" };
	int health{ 0 };
	int mana{ 0 };
	int level{ 1 };
	bool bIsNPC{ false };
};

// We can then make a cleaner function
void CreateCharacter(const CharacterParams& params)
{
	std::cout << "Creating New Character " << params.sName << " with HP: " << params.health
		<< " and MP: " << params.mana << "\n";
}

/*
* Now we can call the new function and it is more readable!
*/
void CallNewCreateCharacterExample()
{
	/*
	* Now it is more clean to read and write.
	* We only also have to set the values that we need.
	* The default values in the struct will handle the rest.
	*/
	CreateCharacter({ .sName = "Jadeite", .health = 147, .level = 10 });
}

// ===================================================================================
// Method Chaining
// ===================================================================================

// Character Builder class
class Character
{
private:
	std::string sName{ "" };
	int health{ 0 };
	int mana{ 0 };
	int level{ 1 };
	bool bIsNPC{ false };

public:
	Character& SetName(const std::string& name) { sName = name; return *this; }
	Character& SetHealth(int inHealth) { health = inHealth; return *this; }
	Character& SetMana(int inMana) { mana = inMana; return *this; }
	Character& SetLevel(int inLevel) { level = inLevel; return *this; }
	Character& SetAsNPC(bool npc) { bIsNPC = npc; return *this; }

	void Print() const {
		std::cout << "Character " << sName 
			<< " (Lvl " << level << ") HP: " << health << " Mana: " << mana << "\n";
	}
};

void CreateBuilderCharacter()
{
	/*
	* In this example, we create a new character 
	* and build it up one piece at a time.
	* only setting the values that we want to set.
	* 
	* It is more readable and elegant.
	*/
	Character hero{};
	hero.SetName("Jadeite")
		.SetHealth(100)
		.SetMana(50)
		.SetLevel(3);
}

/* 
* Here's another example of Method chaining that does not use a build pattern.
* Imagine we have a configuration system where we can set various options dynamically
*/

class Settings
{
private:
	bool bFullscreen{ false };
	int resolutionWidth{ 1920 };
	int resolutionHeight{ 1080 };
	int volume{ 50 };

public:
	Settings& SetFullScreenMode(bool fullscreen) { bFullscreen = fullscreen; return *this; }
	Settings& SetResolution(int width, int height) 
	{ 
		resolutionWidth = width; 
		resolutionHeight = height; 
		return *this; 
	}
	Settings& SetVolume(int inVolume) 
	{ 
		volume = inVolume; 
		return *this; 
	}

	void Apply() const
	{
		std::cout << "Applying Settings:\n"
			<< "Fullscreen: " << (bFullscreen ? "Enabled" : "Disabled") << "\n"
			<< "Resolution: " << resolutionWidth << " x " << resolutionHeight << "\n"
			<< "Volume: " << volume << "\n";
 	}
};

/* Configure settings Examples */
void ConfigureSettingsExamples()
{
	Settings config;
	config.SetFullScreenMode(true)
		.SetResolution(1920, 1080)
		.Apply();

	Settings otherConfig;
	otherConfig.SetVolume(75).Apply();


}

// ===================================================================================
// Combining Named Arguments with Method Chaining
// ===================================================================================
/*
* Alright Now let's combine the two
*/
class CharacterBuilder
{
private:
	Character character;

public:
	CharacterBuilder& name(const std::string& name) { character.SetName(name); return *this; }
	CharacterBuilder& health(int health) { character.SetHealth(health); return *this; }
	CharacterBuilder& mana(int mana) { character.SetMana(mana); return *this; }
	CharacterBuilder& level(int level) { character.SetLevel(level); return *this; }
	CharacterBuilder& npc(bool npc) { character.SetAsNPC(npc); return *this; }

	Character build() { return character; }
};

void CombinedNamedArgsAndMethodChaining()
{
	Character hero = CharacterBuilder().name("Jadeite").health(450).mana(12).build();
	hero.Print();
}

int main()
{
	ConfigureSettingsExamples();
	CombinedNamedArgsAndMethodChaining();
	return 0;
}
