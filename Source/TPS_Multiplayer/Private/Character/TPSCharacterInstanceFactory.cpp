// (C) ToasterCat Studios 2025

#include "Character/TPSCharacterInstanceFactory.h"

// TODO: [PC-228] Replace with JSON list and provide via Blueprint
static const TArray<FString> _FirstNames_Male = {
	"Karl",
	"Steve",
	"Bob",
	"Casey",
	"Dirk",
	"Peter",
	"Conor",
	"Connor",
	"Kieth",
	"Kevin",
	"Albert",
	"Sam",
	"David",
	"Gustavo",
	"Raul",
	"Luis",
	"Louis",
	"James",
	"Jake",
	"Michael",
	"Mike",
	"Tom",
	"Thomas",
	"Timothy",
	"Tim",
	"Robert",
	"Mark",
	"Luke",
	"John",
	"Jon",
	"Jesus",
	"Jorge",
	"Geoffery",
	"Jeff",
	"Ned",
	"Olaf",
	"Frederic",
	"Fred",
	"Sean",
	"Shawn",
	"Kristof",
	"Dante",
	"Daniel",
	"Wayne",
	"Marlon",
	"James"
};

// TODO: [PC-228] Replace with JSON list and provide via Blueprint
static const TArray<FString> _FirstNames_Female = {
	"Casey",
	"Gloria",
	"Francis",
	"Susan",
	"Colleen",
	"Pamela",
	"Cheryl",
	"Sally",
	"Sarah",
	"Sara",
	"Jennifer",
	"Janet",
	"Madeline",
	"Christine",
	"Allison",
	"Alexandra",
	"Sheila",
	"Margaret",
	"Anna",
	"Ana",
	"Eva",
	"Ava",
	"Dianne",
	"Diana",
	"Chris",
	"Heidi"
};

// TODO: [PC-228] Replace with JSON list and provide via Blueprint
static const TArray<FString> _LastNames = {
	"Lance",
	"Wilson",
	"Walker",
	"O'Shea",
	"Kavanaugh",
	"Barrett",
	"O'Shannahan",
	"MacDonald",
	"King",
	"Ojibwe",
	"Dakar",
	"Rice",
	"Baker",
	"Smith",
	"Hunter",
	"Tanner",
	"Johnson",
	"Robertson",
	"Ragnarsson",
	"Borgia",
	"Tudor",
	"Windsor",
	"Williams",
	"Grant",
	"Forest",
	"Forrester",
	"Bush",
	"Hetfield",
	"Keenan",
	"McCoy",
	"Schumacher",
	"Klein",
	"Schwartz",
	"Fitzgerald",
	"Gunn",
	"Nemitz",
	"Halsey",
	"Hayley",
	"Rogers",
	"Daniels",
	"Lee",
	"Li",
	"Nazir",
	"Markstrom",
	"Matthews",
	"Smalls",
	"Hammersmith",
	"Black",
	"Mack",
	"John"
};

// TODO: [PC-228] Replace with JSON list and provide via Blueprint
static const TArray<FString> _Callsigns = {
	"Boomer",
	"Mad Dog",
	"Good Boy",
	"Iceman",
	"Snowball",
	"8-Ball",
	"Joker",
	"Cap",
	"Tex",
	"Gunny",
	"Wedge",
	"Tracer",
	"Hangman",
	"Merlin",
	"Trout",
	"Warlock",
	"Chaser",
	"Banshee",
	"Siren",
	"Hammer",
	"Ali",
	"Screwball",
	"Butterfingers",
	"Pyro",
	"Hobbie",
	"DeadEye",
	"Rocket",
	"Widow",
	"Grimm",
	"Preacher",
	"Shalom",
	"Trick",
	"Treat",
	"Cowboy",
	"Snake",
	"Biggs",
	"Big Shot",
	"Ocelot",
	"Idaho",
	"Wash",
	"Grunt",
	"Mal",
	"Scotty",
	"Tycho",
	"Drummer",
	"Cash"
};

static FString _randNameInList(TArray<FString> list)
{
	int max = list.Num() - 1;
	int i = FMath::RandRange(0, max);
	return list[i];
}

UTPSCharacterInstanceFactory::UTPSCharacterInstanceFactory()
{
	
}

UTPSCharacterInstance* UTPSCharacterInstanceFactory::ConfigureCharacterInstanceForUnitAndRole(UTPSCharacterInstance* instance, FTPSUnitID unit, FTPSUnitRoleDefinitionData roleDefinition)
{
	instance->Identity.UnitID = unit;

	// Coin flip for gender
	bool isMale = FMath::RandRange(0, 1) > 0;
	if (isMale) {
		instance->Identity.CharacterBodyType = ETPSCharacterBodyType::Male;
		instance->Identity.FirstName = _randNameInList(_FirstNames_Male);
	} else {
		instance->Identity.CharacterBodyType = ETPSCharacterBodyType::Female;
		instance->Identity.FirstName = _randNameInList(_FirstNames_Female);
	}
	instance->Identity.LastName = _randNameInList(_LastNames);
	instance->Identity.Callsign = _randNameInList(_Callsigns);


	instance->Identity.SquadRole = roleDefinition.Role;
	instance->Identity.Rank = roleDefinition.Rank;

	return instance;
}
