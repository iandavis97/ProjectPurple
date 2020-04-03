// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectPurple.h"
#include "ProjectPurpleGameMode.h"
#include "Card.h"
#include <typeinfo>
#include "PurpleAIController.h"
//#include "CardActor.h"
#include "ParserObj.h"

// This heshbohn array is filled with heshbohn struct objects found in the header file, each element is a separate part that we use to determine the heshbohn event in blueprint
FHeshbon HESHBON[14] = {
	{ 0, 1, TEXT("Object's owner is found!"), EHeshbonType::LostAndFound },
	{ 1, 1, TEXT("Object's owner is not found."), EHeshbonType::LostAndFound },
	{ 0, 1, TEXT("No effect."), EHeshbonType::Transgression },
	{ 1, 1, TEXT("Take a completed Family \nResponsibility back to your hand."), EHeshbonType::Transgression },
	{ 2, 2, TEXT("You must pay restitution as with \nTeshuvah for this transgression OR \ntake a completed Family Responsibility \nback to your hand."), EHeshbonType::Transgression },
	{ 3, 1, TEXT("You must pay restitution as with \nTeshuvah for this transgression at a \ncost of +2 OR take a completed Family \nResponsibility back to your hand."), EHeshbonType::Transgression },
	{ 4, 1, TEXT("You must pay restitution as with \nTeshuvah for this transgression at a \ncost of +4 OR take a completed Family \nResponsibility back to your hand."), EHeshbonType::Transgression },
	{ 5, 1, TEXT("You must pay restitution as with \nTeshuvah for this transgression at a \ncost of +6 OR take a completed Family \nResponsibility back to your hand."), EHeshbonType::Transgression },
	{ 0, 1, TEXT("No effect."), EHeshbonType::BeyondTheLaw },
	{ 1, 1, TEXT("Complete a Family Responsibility in \nyour hand for free)."), EHeshbonType::BeyondTheLaw },
	{ 2, 2, TEXT("If you can complete a Family \nResponsibility in your hand with \nlegally owned resources now, you may \ndo so."), EHeshbonType::BeyondTheLaw },
	{ 3, 1, TEXT("If you can complete a Family \nResponsibility in your hand with \nlegally owned resources now at -2 cost, \nyou may do so."), EHeshbonType::BeyondTheLaw },
	{ 4, 1, TEXT("If you can complete a Family \nResponsibility in your hand with \nlegally owned resources now at -4 cost, \nyou may do so."), EHeshbonType::BeyondTheLaw },
	{ 5, 1, TEXT("If you can complete a Family \nResponsibility in your hand with \nlegally owned resources now at -6 cost, \nyou may do so."), EHeshbonType::BeyondTheLaw },
};

AProjectPurpleGameMode::AProjectPurpleGameMode(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//PrimaryActorTick.bCanEverTick = true;

	int64 DateInSeconds = FDateTime::Now().ToUnixTimestamp();
	FRandomStream SRand = FRandomStream();
	SRand.Initialize(DateInSeconds);

	//	static ConstructorHelpers::FClassFinder<AActor>CardBP(TEXT("/Game/Blueprint/CardBP.CardBP_C"));
	//
	//	if (CardBP.Class)
	//	{
	//		CardBlueprint = CardBP.Class;
	//	}
	//	else
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("Could not find CardBP.Uasset for card spawning in C++!"));
	//	}

}

//void AProjectPurpleGameMode::BeginPlay()
//{
//	// Loads and populates card lists
//	SetupCardLists();
//
//	//StartGame();
//}

// Input Functions
void AProjectPurpleGameMode::assignButtonChoice(int32 newValue)
{
	buttonChoice = newValue;
}

int32 AProjectPurpleGameMode::retrieveButtonChoice()
{
	return buttonChoice;
}

void AProjectPurpleGameMode::checkButtons()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Iteration");
	for (TActorIterator<ABoxButton> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ABoxButton *box = *ActorItr;
		if (box->isTouched == true)
		{
			assignButtonChoice(box->index);
		}
	}
}


// deprecated functions, kept for future reference and posterity

// Spawns and returns a visible CardBP that will be used for animation
//ACardActor* AProjectPurpleGameMode::SpawnCard(UCard* cardData, FVector location, FRotator rotation, float scalar)
//{
//	// spawn the physical card to the screen for animation
//	ACardActor* cardBP = World->SpawnActorDeferred<ACardActor>(CardBlueprint, FTransform(rotation, location, FVector(scalar)), nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
//	cardBP->CardData = cardData;
//	cardBP->FinishSpawning(FTransform(rotation, location, FVector(scalar)));
//	return cardBP;
//}

// Spawns a custom defined spline in the world and returns it
//ACustomSpline* AProjectPurpleGameMode::SpawnSplineWithPoints(const FVector& pointA, const FVector& pointB, const TArray<FVector>& additionalPoints, FVector splineLocation, FRotator splineRotation)
//{
//	UClass* splineClass = ACustomSpline::StaticClass();
//	const FTransform trans = FTransform(splineRotation, splineLocation);
//	ACustomSpline* spline = World->SpawnActorDeferred<ACustomSpline>(splineClass, trans, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
//	spline->createSplineWithPoints(pointA, pointB, additionalPoints);
//	spline->FinishSpawning(trans);
//	return spline;
//}

// Spawns a custom defined spline in the world and returns it
//ACustomSpline* AProjectPurpleGameMode::SpawnSpline(const FVector& pointA, const FVector& pointB, FVector splineLocation, FRotator splineRotation)
//{
//	UClass* splineClass = ACustomSpline::StaticClass();
//	const FTransform trans = FTransform(splineRotation, splineLocation);
//	ACustomSpline* spline = World->SpawnActorDeferred<ACustomSpline>(splineClass, trans, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
//	spline->createSpline(pointA, pointB);
//	spline->FinishSpawning(trans);
//	return spline;
//}

/////////////////////////////////////////////////////////////////

//Sets up the base card lists that will be managed in C++
void AProjectPurpleGameMode::SetupCardLists()
{
	UE_LOG(LogTemp, Warning, TEXT("begin play %s"), *StaticClass()->GetName());

	UParserObj* myParser = NewObject<UParserObj>();

	// Loads card from the json and adds the data into the arrays of parser obj class
	myParser->LoadCards();

	// We retrieve the data from the ararys of parser objs and make a copy into the game mode.
	eventsList.Empty();
	for (int i = 0; i < myParser->events.Num(); i++)
	{
		eventsList.Add(myParser->events[i]);
	}

	communalResponsibilities.Empty();
	for (int i = 0; i < myParser->communalResponsibilities.Num(); i++)
	{
		communalResponsibilities.Add(myParser->communalResponsibilities[i]);
	}

	familyResponsibilities1.Empty();
	for (int i = 0; i < myParser->familyResponsibilities1.Num(); i++)
	{
		familyResponsibilities1.Add(myParser->familyResponsibilities1[i]);
	}

	familyResponsibilities2.Empty();
	for (int i = 0; i < myParser->familyResponsibilities2.Num(); i++)
	{
		familyResponsibilities2.Add(myParser->familyResponsibilities2[i]);
	}

	familyResponsibilities3.Empty();
	for (int i = 0; i < myParser->familyResponsibilities3.Num(); i++)
	{
		familyResponsibilities3.Add(myParser->familyResponsibilities3[i]);
	}

	familyResponsibilities4.Empty();
	for (int i = 0; i < myParser->familyResponsibilities4.Num(); i++)
	{
		familyResponsibilities4.Add(myParser->familyResponsibilities4[i]);
	}

	familyResponsibilities5.Empty();
	for (int i = 0; i < myParser->familyResponsibilities5.Num(); i++)
	{
		familyResponsibilities5.Add(myParser->familyResponsibilities5[i]);
	}

	resourcesList.Empty();
	for (int i = 0; i < myParser->resources.Num(); i++)
	{
		resourcesList.Add(myParser->resources[i]);
	}

	lostResources.Empty();
	for (int i = 0; i < myParser->lostResources.Num(); i++)
	{
		lostResources.Add(myParser->lostResources[i]);
	}

	rolesList.Empty();
	for (int i = 0; i < myParser->roles.Num(); i++)
	{
		rolesList.Add(myParser->roles[i]);
	}

	startingResources.Empty();
	for (int i = 0; i < myParser->startingResources.Num(); i++)
	{
		startingResources.Add(myParser->startingResources[i]);
	}

	UE_LOG(LogTemp, Warning, TEXT("%s"), *StaticClass()->GetName());
}

// Function for shuffling cards, pass in the deck, starting and ending indices of the deck for shuffling
TArray<class UCard*> AProjectPurpleGameMode::ShuffleCards(TArray<class UCard*> deck)
{
	// this is using the fisher yates algorithm, because the earlier solution was only giving partial pseudo random shuffling
	for (int32 i = 0; i < deck.Num() - 2; i++)
	{
		int32 j = FMath::FloorToInt(FMath::RandRange(i, deck.Num() - 1));

		// xcode fix
		if (j < 0)
		{
			j *= -1;
		}

		if (j != i)
		{
			UCard* temp = deck[i];
			deck[i] = deck[j];
			deck[j] = temp;
		}
	}

	return deck;
}

// Shuffle the events according to season and put it in an new array.
TArray<class UCard*> AProjectPurpleGameMode::ShuffleEvents(TArray<class UCard*> events)
{
	TArray<class UCard*> shuffled;

	TArray<class UCard*> winter;
	TArray<class UCard*> spring;
	TArray<class UCard*> summer;
	TArray<class UCard*> fall;

	// need to separate cards in deck by season, using property
	// then shuffle each section of deck and assemble correctly
	// winter, spring, summer, fall
	for (int32 i = events.Num() - 1; i >= 0; i--)
	{

		ESeason s = events[i]->GetSeasonProperty();

		// Assigns multiple copies and puts them into different decks if the card is present in more than one season

		if ((s & ESeason::Winter) != ESeason::None)
		{
			winter.Add(events[i]);
		}

		if ((s & ESeason::Spring) != ESeason::None)
		{
			spring.Add(events[i]);
		}

		if ((s & ESeason::Summer) != ESeason::None)
		{
			summer.Add(events[i]);
		}

		if ((s & ESeason::Autumn) != ESeason::None)
		{
			fall.Add(events[i]);
		}
	}

	// Shuffle the seasosn indvidually
	winter = ShuffleCards(winter);

	spring = ShuffleCards(spring);

	summer = ShuffleCards(summer);

	fall = ShuffleCards(fall);


	// Will eventually have to change this, to where festivals will be placed at the top of each deck after shuffling, random for now

	// Add the cards to the new deck
	// last season first since unreal pop takes off the back of an array
	shuffled.Append(fall);
	shuffled.Append(summer);
	shuffled.Append(spring);
	shuffled.Append(winter);

	return shuffled;
}

// Gives the cards to players when the game starts.
void AProjectPurpleGameMode::GiveStartingHands()
{
	// loop through players checking their role against the owner of the starting resource cards, then adds to hand
	for (int i = 0; i < players.Num(); i++)
	{
		for (int j = 0; j < startingResources.Num(); j++)
		{
			if (players[i]->playerRole->name == startingResources[j]->GetStringProperty("owner"))
			{
				players[i]->playerHand.Add(startingResources[j]);
				// startingResources.RemoveAt(j);
			}
		}
	}

	TArray<TArray<UCard*>> familyDecks;

	familyResponsibilities1 = ShuffleCards(familyResponsibilities1);
	familyDecks.Add(familyResponsibilities1);

	familyResponsibilities2 = ShuffleCards(familyResponsibilities2);
	familyDecks.Add(familyResponsibilities2);

	familyResponsibilities3 = ShuffleCards(familyResponsibilities3);
	familyDecks.Add(familyResponsibilities3);

	familyResponsibilities4 = ShuffleCards(familyResponsibilities4);
	familyDecks.Add(familyResponsibilities4);

	familyResponsibilities5 = ShuffleCards(familyResponsibilities5);
	familyDecks.Add(familyResponsibilities5);

	for (int i = 0; i < players.Num(); i++)
	{
		players[i]->playerFamilyResponsibilities = familyDecks[i];  // Check if array has assignment overloads. ****It does!, the = operator preforms a deep copy

		UCard* firstFamily = players[i]->playerFamilyResponsibilities.Pop();
		players[i]->playerActiveFamilyResponsibilities.Add(firstFamily);
	}
}

// sets the enum to the next phase for event firing
void AProjectPurpleGameMode::AdvanceTurnPhase()
{
	if (turnPhase == ETurnPhase::Discard)
	{
		turnPhase = ETurnPhase::DrawResource;
		activePlayerIndex += 1;

		if (activePlayerIndex > players.Num() - 1)
		{
			activePlayerIndex = 0;
		}

		activePlayer = players[activePlayerIndex];
	}
	else {
		turnPhase = ETurnPhase(int(turnPhase) + 1);
	}

	OnBeginTurnPhase(turnPhase);
	//OnTest().Broadcast();
}

// returns the current state of the enum
ETurnPhase AProjectPurpleGameMode::GetTurnPhase()
{
	return turnPhase;
}

// sets the turn phase to specified phase if you need to send straight to discard etc
void AProjectPurpleGameMode::SetTurnPhase(ETurnPhase phase)
{
	turnPhase = phase;
	OnBeginTurnPhase(turnPhase);
}

// spawn 1 player and 4 ai controllers, only spawns player for now
void AProjectPurpleGameMode::SpawnPlayers()
{
	World = GetWorld();
	// spawn player's controller --- This works! Do same action for 4 ai controllers once we have an ai controller class
	if (World)
	{
		APurpleController* player = GetWorld()->SpawnActor<APurpleController>();
		player->initController();
		players.Add(player);



		//// Should be spawning 4 ai controllers (ai coming later)
		for (int i = 1; i < initialNumPlayers; i++)
		{
			APurpleAIController* AIplayer = World->SpawnActor<APurpleAIController>();
			//AIplayer->initController();
			players.Add(AIplayer);
		}

		int male = 0;
		int female = 0;

		for (int i = 0; i < players.Num(); i++)
		{
			if (players[i]->gender == "Male")
			{
				male += 1;
			}
			else
			{
				female += 1;
			}
		}

		if (female == 0)
		{
			players[players.Num() - 1]->gender = "Female";
			players[1]->gender = "Female";
		}
		else if (female == 1)
		{
			for (int i = 1; i < players.Num(); i++)
			{
				if (players[i]->gender == "Male")
				{
					players[i]->gender == "Female";
					break;
				}
			}
		}
	}

}

void AProjectPurpleGameMode::NeedToReshuffleResources()
{
	AProjectPurpleGameMode* currentGameMode = static_cast<AProjectPurpleGameMode*>(GetWorld()->GetAuthGameMode());

	if (currentGameMode->resourcesList.Num() <= 2)
	{
		currentGameMode->ShuffleCards(currentGameMode->resourcesListDiscard);
		currentGameMode->resourcesList.Append(currentGameMode->resourcesListDiscard);
		currentGameMode->resourcesListDiscard.Empty();
	}
}

void AProjectPurpleGameMode::AdjustResponsibilityValue(UCard* resp)
{
	UCard* bonus = completedCommunalResponsibilities.Last();
	float newCost = resp->GetDoubleProperty("cost");

	if (resp->type == "Communal Responsibility")
	{
		if (bonus->GetStringProperty("bonus_to_communal") == resp->GetStringProperty("category"))
		{
			newCost -= resp->GetDoubleProperty("bonus");
		}
	}
	else if (resp->type == "Family Responsibility")
	{
		if (bonus->GetStringProperty("bonus_to_family") == resp->name)
		{
			newCost -= resp->GetDoubleProperty("bonus");
		}
		else if (resp->name == "Redemption of Firstborn")
		{
			newCost -= resp->GetDoubleProperty("bonus");
		}
	}

	resp->SetDoubleProperty("cost", newCost);
}

void AProjectPurpleGameMode::AdjustResponsibilityOnDraw(UCard * resp)
{
	TArray<UCard*> bonus = completedCommunalResponsibilities;
	float newCost = resp->GetDoubleProperty("cost");

	if (resp->type == "Communal Responsibility")
	{
		for (UCard* i : bonus)
		{
			if (i->GetStringProperty("bonus_to_communal") == resp->GetStringProperty("category"))
			{
				newCost -= resp->GetDoubleProperty("bonus");
			}
		}
	}
	else if (resp->type == "Family Responsibility")
	{
		for (UCard* i : bonus)
		{
			if (i->GetStringProperty("bonus_to_family") == resp->name)
			{
				newCost -= resp->GetDoubleProperty("bonus");
			}
			else if (resp->name == "Redemption of Firstborn")
			{
				newCost -= resp->GetDoubleProperty("bonus");
			}
		}
	}

	resp->SetDoubleProperty("cost", newCost);
}

TArray<ASuperController*> AProjectPurpleGameMode::DisasterPlayerList()
{
	TArray<ASuperController*> playersList;

	playersList.Add(activePlayer);

	int counter = 0;
	int playerIndex = activePlayerIndex + 1;

	while (counter < players.Num() - 1)
	{
		if (playerIndex > players.Num() - 1)
		{
			playerIndex = 0;
		}

		playersList.Add(players[playerIndex]);

		playerIndex++;
		counter++;
	}

	return playersList;
}

//Sort the roles into male and female
void AProjectPurpleGameMode::PopulateRolesList()
{
	// sort roles into male and female lists
	for (int i = 0; i < rolesList.Num(); i++)
	{
		if (i % 2 == 1)
		{
			maleRolesList.Add(rolesList[i]);
		}
		else if (i % 2 == 0)
		{
			femaleRolesList.Add(rolesList[i]);
		}
	}
}

// does all necessary pre game operations / initialization
void AProjectPurpleGameMode::StartGame()
{


	for (TActorIterator<ACustomSpline> ActorItr(World); ActorItr; ++ActorItr)
	{
		ACustomSpline* spline = *ActorItr;

		if (spline->GetName() == "CommunalSpline")
		{
			communalSpline = spline;
		}
		else if (spline->GetName() == "HandSpline")
		{
			handSpline = spline;
		}
		else if (spline->GetName() == "RestingHandSpline")
		{
			restingHandSpline = spline;
		}
		else if (spline->GetName() == "SwipeSpline")
		{
			swipeSpline = spline;
		}
		else if (spline->GetName() == "CompletedCommunalSpline")
		{
			completedCommunalSpline = spline;
		}
		else if (spline->GetName() == "CompletedFamilySpline")
		{
			completedFamilySpline = spline;
		}
	}

	//Hard coded
	//communalSpline = SpawnSpline(FVector(0, 0, 0), FVector(0, -800, 0), FVector(157.792969, 414.735413, 43.563995), FRotator(0, 0, 0));
	//handSpline = SpawnSpline(FVector(0, 0, 0), FVector(0, -800, 0), FVector(157.792969, 414.735413, 43.563995), FRotator(0, 0, 0));

	// make players
	//SpawnPlayers();

	// suhffle all of the decks in the game for everyone
	communalResponsibilities = ShuffleCards(communalResponsibilities);
	resourcesList = ShuffleCards(resourcesList);
	lostResources = ShuffleCards(lostResources);
	eventsList = ShuffleEvents(eventsList);



	//	for (int i = 0; i < players.Num(); i++)
	//	{
	//		// this is random for now, will set role based on click in bp. need to add param in setrole when this happens, need role cards in game still
	//        if (!static_cast<APurpleController*>(players[i]))
	//        {
	//            players[i]->SetRole();
	//        }
	//		
	//	}

		// set 3 active communals for beginning of game
	for (int i = 0; i < 3; i++)
	{
		UCard* card = communalResponsibilities.Pop(true);

		activeCommunalResponsibilities.Add(card);
	}

	//Double checks that players have been initialized
	if (players.Num() > 0)
	{
		// give each player their respective starting resources and their first family responsibility
		GiveStartingHands();

		//DistributeOnSpline(communalSpline, displayedCommunalResponsibilities);

		// set the starting player for the game
		activePlayerIndex = 0;
		activePlayer = players[activePlayerIndex];
	}
}

// called to check for end game state
bool AProjectPurpleGameMode::IsGameOver()
{
	if (eventsList.Num() <= 0 || destitution == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}

EEndGameTypes AProjectPurpleGameMode::ReturnGameOverEnum()
{
	if (eventsList.Num() <= 0)
	{
		return EEndGameTypes::NoMoreEvents;
	}
	else if (destitution == true)
	{
		return EEndGameTypes::Destitution;
	}
	else
	{
		return EEndGameTypes::None;
	}
}

// will return the appropriate enum based on the name and type of card for gui
EEventOptions AProjectPurpleGameMode::ReturnEventEnum()
{
	if (currentEvent->type == "Individual - Found")
	{
		// cards with no options
		if (currentEvent->name == "Found an Object" || currentEvent->name == "Bumper Crop" ||
			currentEvent->name == "Scholarly" || currentEvent->name == "Celebrate Birth" ||
			currentEvent->name == "Money in the River" || currentEvent->name == "Breeding")
		{
			return EEventOptions::NoOptionAcquire;
		}

		// keep, return, and ignore cards
		else if (currentEvent->name == "Abandoned Donkey" || currentEvent->name == "Abandoned Cow" ||
			currentEvent->name == "Abandoned Sheep")
		{
			return EEventOptions::KeepReturnAndIgnore;
		}

		// money purse
		else if (currentEvent->name == "Money-Purse in River")
		{
			return EEventOptions::MoneyPurseInRiver;
		}

		// money in dust
		else if (currentEvent->name == "Money in the Dust")
		{
			return EEventOptions::MoneyInDust;
		}

		// found in river
		else if (currentEvent->name == "Found Object in the River")
		{
			return EEventOptions::FoundObjectInRiver;
		}

		// need for compilation
		else
		{
			return EEventOptions::None;
		}
	}

	else if (currentEvent->type == "Individual - Lost")
	{
		// no option cards
		if (currentEvent->name == "Object Lost" || currentEvent->name == "Animal Lost")
		{
			return EEventOptions::NoOptionLoss;
		}

		// ignore and return 
		else if (currentEvent->name == "Animal Runs Away")
		{
			return EEventOptions::AnimalRunsAway;
		}

		// empty vessel honey
		else if (currentEvent->name == "Honey Jar Cracks" || currentEvent->name == "Honey Jar cracks")
		{
			return EEventOptions::HoneyJarCracks;
		}

		// need for compilation
		else
		{
			return EEventOptions::None;
		}
	}

	else if (currentEvent->type == "Community - Disaster")
	{
		return EEventOptions::Disaster;
	}

	else if (currentEvent->type == "Community - Crisis")
	{
		return EEventOptions::Crisis;
	}

	else if (currentEvent->type == "Community - Festival")
	{
		return EEventOptions::Festival;
	}

	// need for compilation
	else
	{
		return EEventOptions::None;
	}
}

// these events just happen and need no option input from the player ---- Do not need input from implementables
UCard* AProjectPurpleGameMode::NoOptionAcquireEvents()
{
	UCard* card = nullptr;

	if (currentEvent->name == "Found an Object")
	{
		int foIndex = activePlayerIndex + 1;

		// loops through players to find first in turn order with < 3 resources
		for (int i = 0; i < players.Num() - 1; i++)
		{
			if (foIndex > players.Num() - 1)
			{
				foIndex = 0;
			}

			if (players[foIndex]->playerHand.Num() < 3)
			{
				// adds a lost resource to the player hand --- not legally theirs
				card = players[foIndex]->DrawLostResource();
				break;
			}

			++foIndex;
		}
	}

	else if (currentEvent->name == "Bumper Crop")
	{
		// player with least family done in turn order
		int bcIndex = getPlayerWithLowestFamily();
		card = players[bcIndex]->DrawResource();
	}

	else if (currentEvent->name == "Celebrate Birth")
	{
		// add the event to the hand since it has value just like resources
		card = currentEvent;
		players[activePlayerIndex]->playerHand.Add(card);
		activePlayer->handLimit += 1;
	}

	else if (currentEvent->name == "Breeding")
	{
		TArray<UCard*> legals;

		int bIndex = activePlayerIndex + 1;

		// need to find the next player in turn order with legal animal
		for (int i = 0; i < players.Num() - 1; i++)
		{
			if (bIndex > players.Num() - 1)
			{
				bIndex = 0;
			}

			legals = players[bIndex]->GetLegalResources();

			for (int j = 0; j < legals.Num(); j++)
			{
				if (legals[j]->name == "Cow" || legals[j]->name == "Sheep" || legals[j]->name == "Donkey")
				{
					// add the event to the hand, as it has value of 6
					card = currentEvent;
					players[bIndex]->playerHand.Add(card);
					activePlayer->handLimit += 1;
					players[bIndex]->playerOwnedEvents.Add(card);
					break;
				}
			}

			if (players.Num() > 1)
			{
				++bIndex;
			}
		}
	}

	else if (currentEvent->name == "Money in the River")
	{
		// add the event to the hand since it has value just like resources
		card = currentEvent;

		activePlayer->playerHand.Add(card);
		activePlayer->handLimit += 1;
	}

	else if (currentEvent->name == "Scholarly")
	{
		// player with lowest family keeps the scholarly card
		int sIndex = getPlayerWithLowestFamily();

		card = currentEvent;

		players[sIndex]->playerHand.Add(card);
		activePlayer->handLimit += 1;
	}

	return card;
}

UCard* AProjectPurpleGameMode::NoOptionLossEvents()
{
	UCard* card = nullptr;

	if (currentEvent->name == "Object Lost")
	{
		TArray<UCard*> legalResources;

		// the player with most family done in turn order
		int olIndex = getPlayerWithHighestFamily();

		// get players' legally owned resources and pick one to lose
		legalResources = players[olIndex]->GetLegalResources();

		if (legalResources.Num() > 0)
		{
			int cardIndToRemove = FMath::RandRange(0, legalResources.Num() - 1);

			card = legalResources[cardIndToRemove];

			if (card->name == "Money in the River")
			{
				activePlayer->handLimit -= 1;
			}

			// remove the card from player hand and add to lost pile
			players[olIndex]->playerHand.RemoveSingle(card);
			players[olIndex]->playerFoundObjects.RemoveSingle(card);
			players[olIndex]->playerLostObjects.Add(card);
			lostUsedIndex = olIndex;
		}
	}

	else if (currentEvent->name == "Animal Lost")
	{
		TArray<UCard*> legals;
		TArray<UCard*> animals;

		int alIndex = activePlayerIndex + 1;

		// need to find the next player in turn order with legal animal
		for (int i = 0; i < players.Num() - 1; i++)
		{
			if (alIndex > players.Num() - 1)
			{
				alIndex = 0;
			}

			legals = players[alIndex]->GetLegalResources();

			for (int j = 0; j < legals.Num(); j++)
			{
				if (legals[j]->name == "Cow" || legals[j]->name == "Sheep" || legals[j]->name == "Donkey")
				{
					// add to possible animals to take
					animals.Add(legals[j]);
				}
			}

			if (players.Num() > 1 && animals.Num() == 0)
			{
				alIndex++;
			}
		}

		// any of the players meet condition
		if (animals.Num() > 0)
		{
			int cardIndToRemove = FMath::RandRange(0, animals.Num() - 1);

			card = animals[cardIndToRemove];

			players[alIndex]->playerHand.RemoveSingle(card);
			players[alIndex]->playerFoundObjects.RemoveSingle(card);
			players[alIndex]->playerLostObjects.Add(card);
			lostUsedIndex = alIndex;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("No players with animals"));
		}
	}

	return card;
}

// these events will take input from an implementable event and resolve accordingly ---- Do not need input from implementables
void AProjectPurpleGameMode::KeepReturnAndIgnoreEvents()
{
	if (activePlayer->IsA(APurpleController::StaticClass()))
	{
		buttonChoice = retrieveButtonChoice();
	}
	else
	{
		int standing = CheckPlayerStanding();

		// generous
		if (standing > 0)
		{
			buttonChoice = 1;
		}
		// neutral
		else if (standing == 0)
		{
			buttonChoice = 2;
		}
		// greedy
		else
		{
			buttonChoice = 0;
		}
	}

	if (currentEvent->name == "Abandoned Donkey")
	{
		switch (buttonChoice)
		{
			// case 0 = Keep, transgression, card goes to hand
		case 0:
			// subtract from standing for not
			if (activePlayer->IsA(APurpleController::StaticClass()))
			{
				static_cast<APurpleController*>(activePlayer)->SubtractFromPlayerStanding(1);
			}
			activePlayer->playerHand.Add(currentEvent);
			activePlayer->handLimit += 1;
			activePlayer->Transgressions.Add(currentEvent);
			break;
			// case 1 = Return, beyond the law, make player go straight to discard
		case 1:
			// add to standing for helping
			if (activePlayer->IsA(APurpleController::StaticClass()))
			{
				static_cast<APurpleController*>(activePlayer)->AddToPlayerStanding(1);
			}
			activePlayer->BeyondTheLaws.Add(currentEvent);
			break;
			// case 2 = Ignore, nothing
		case 2:
			break;
		}
	}
	else if (currentEvent->name == "Abandoned Sheep")
	{
		switch (buttonChoice)
		{
			// case 0 = Keep, transgression, card goes to hand
		case 0:
			// subtract from standing for not
			if (activePlayer->IsA(APurpleController::StaticClass()))
			{
				static_cast<APurpleController*>(activePlayer)->SubtractFromPlayerStanding(1);
			}
			activePlayer->playerHand.Add(currentEvent);
			activePlayer->handLimit += 1;
			activePlayer->Transgressions.Add(currentEvent);
			break;
			// case 1 = Return, beyond the law, make player go straight to discard
		case 1:
			// add to standing for helping
			if (activePlayer->IsA(APurpleController::StaticClass()))
			{
				static_cast<APurpleController*>(activePlayer)->AddToPlayerStanding(1);
			}
			activePlayer->BeyondTheLaws.Add(currentEvent);
			break;
			// case 2 = Ignore, nothing
		case 2:
			break;
		}
	}
	else if (currentEvent->name == "Abandoned Cow")
	{
		switch (buttonChoice)
		{
			// case 0 = Keep, transgression, card goes to hand
		case 0:
			// subtract from standing for not
			if (activePlayer->IsA(APurpleController::StaticClass()))
			{
				static_cast<APurpleController*>(activePlayer)->SubtractFromPlayerStanding(1);
			}
			activePlayer->playerHand.Add(currentEvent);
			activePlayer->handLimit += 1;
			activePlayer->Transgressions.Add(currentEvent);
			break;
			// case 1 = Return, beyond the law,make player go straight to discard
		case 1:
			// add to standing for helping
			if (activePlayer->IsA(APurpleController::StaticClass()))
			{
				static_cast<APurpleController*>(activePlayer)->AddToPlayerStanding(1);
			}
			activePlayer->BeyondTheLaws.Add(currentEvent);
			break;
			// case 2 = Ignore, nothing
		case 2:
			break;
		}
	}
}

// these events will take input from an implementable event and resolve accordingly
void AProjectPurpleGameMode::MoneyPurseInRiver()
{
	if (activePlayer->IsA(APurpleController::StaticClass()))
	{
		buttonChoice = retrieveButtonChoice();
	}
	else
	{
		int standing = CheckPlayerStanding();

		// generous
		if (standing >= 0)
		{
			buttonChoice = 1;
		}
		// greedy
		else
		{
			buttonChoice = 0;
		}
	}

	switch (buttonChoice)
	{
		// case 0 = Keep
	case 0:
		// subtract from standing for not
		if (activePlayer->IsA(APurpleController::StaticClass()))
		{
			static_cast<APurpleController*>(activePlayer)->SubtractFromPlayerStanding(1);
		}
		activePlayer->playerHand.Add(currentEvent);
		activePlayer->handLimit += 1;
		break;
		// case 1 = return, beyond the law
	case 1:
		// add to standing for helping
		if (activePlayer->IsA(APurpleController::StaticClass()))
		{
			static_cast<APurpleController*>(activePlayer)->AddToPlayerStanding(1);
		}
		activePlayer->BeyondTheLaws.Add(currentEvent);
		break;
	}
}

// these events will take input from an implementable event and resolve accordingly
UCard* AProjectPurpleGameMode::FoundObjectInRiverStageOne()
{
	UCard* card = nullptr;

	if (lostResources.Num() > 0)
	{
		card = lostResources.Pop();
	}

	return card;
}

void AProjectPurpleGameMode::FoundObjectInRiverStageTwo(ACardActor* card)
{
	if (activePlayer->IsA(APurpleController::StaticClass()))
	{
		buttonChoice = retrieveButtonChoice();
	}
	else
	{
		int standing = CheckPlayerStanding();

		// generous
		if (standing >= 0)
		{
			buttonChoice = 0;
		}
		// greedy
		else
		{
			buttonChoice = 1;
		}
	}

	switch (buttonChoice)
	{
		// case 0 = Keep
	case 0:
		// add to standing for helping
		if (activePlayer->IsA(APurpleController::StaticClass()))
		{
			static_cast<APurpleController*>(activePlayer)->AddToPlayerStanding(1);
		}
		card->CardData->currentOwner = activePlayer->playerRole->name;
		activePlayer->playerHand.Add(card->CardData);
		break;
		// case 1 = Discard, beyond the law
	case 1:
		// subtract from standing for not
		if (activePlayer->IsA(APurpleController::StaticClass()))
		{
			static_cast<APurpleController*>(activePlayer)->SubtractFromPlayerStanding(1);
		}
		lostResourcesDiscard.Add(card->CardData);
		activePlayer->BeyondTheLaws.Add(card->CardData);
		break;
	}
}

// these events will take input from an implementable event and resolve accordingly
bool AProjectPurpleGameMode::MoneyInDustStageOne()
{
	// next player in turn order with any dinarim cards
	int mdIndex = activePlayerIndex + 1;
	TArray<UCard*> dinarim;

	for (int i = 0; i < players.Num() - 1; i++)
	{
		if (mdIndex > players.Num() - 1)
		{
			mdIndex = 0;
		}

		// loop through hand of player in order to find if they have any dinarim cards
		for (int j = 0; j < players[mdIndex]->playerHand.Num(); j++)
		{
			if (players[mdIndex]->playerHand[j]->name.Contains("Dinar"))
			{
				return true;
			}
		}

		if (players.Num() > 1 && dinarim.Num() == 0)
		{
			mdIndex++;
		}
	}

	return false;
}

UCard* AProjectPurpleGameMode::MoneyInDustStageTwo()
{
	UCard* card = nullptr;

	// next player in turn order with any dinarim cards
	int mdIndex = activePlayerIndex + 1;
	TArray<UCard*> dinarim;

	while (dinarim.Num() == 0)
	{
		if (mdIndex > players.Num() - 1)
		{
			mdIndex = 0;
		}

		// loop through hand of player in order to find if they have any dinarim cards
		for (int j = 0; j < players[mdIndex]->playerHand.Num(); j++)
		{
			if (players[mdIndex]->playerHand[j]->name.Contains("Dinar"))
			{
				dinarim.Add(players[mdIndex]->playerHand[j]);
			}
		}

		if (players.Num() > 1 && dinarim.Num() == 0)
		{
			mdIndex++;
		}
	}

	moneyInDustIndex = mdIndex;

	int cardIndToRemove = FMath::RandRange(0, dinarim.Num() - 1);

	card = dinarim[cardIndToRemove];

	players[mdIndex]->playerHand.RemoveSingle(card);

	return card;
}

void AProjectPurpleGameMode::MoneyInDustStageThree(ACardActor* card)
{
	if (activePlayer->IsA(APurpleController::StaticClass()))
	{
		buttonChoice = retrieveButtonChoice();
	}
	else
	{
		int standing = CheckPlayerStanding();

		// generous
		if (standing >= 0)
		{
			buttonChoice = 0;
		}
		// greedy
		else
		{
			buttonChoice = 1;
		}
	}

	switch (buttonChoice)
	{
		// case 0 = return
	case 0:
		// add to standing for helping
		if (activePlayer->IsA(APurpleController::StaticClass()))
		{
			static_cast<APurpleController*>(activePlayer)->AddToPlayerStanding(1);
		}
		players[moneyInDustIndex]->playerHand.Add(card->CardData);
		activePlayer->BeyondTheLaws.Add(currentEvent);
		break;
		// case 1 = keep
	case 1:
		// subtract from standing for not
		if (activePlayer->IsA(APurpleController::StaticClass()))
		{
			static_cast<APurpleController*>(activePlayer)->SubtractFromPlayerStanding(1);
		}
		activePlayer->playerHand.Add(card->CardData);
		break;
	}
}

// these events will take input from an implementable event and resolve accordingly
bool AProjectPurpleGameMode::AnimalRunsAwayStageOne()
{
	TArray<UCard*> animals;
	int araIndex = activePlayerIndex + 1;

	// need to find the next player in turn order with animal
	for (int i = 0; i < players.Num() - 1; i++)
	{
		if (araIndex > players.Num() - 1)
		{
			araIndex = 0;
		}

		for (int j = 0; j < players[araIndex]->playerHand.Num(); j++)
		{
			if (players[araIndex]->playerHand[j]->name == "Cow" || players[araIndex]->playerHand[j]->name == "Sheep" || players[araIndex]->playerHand[j]->name == "Donkey")
			{
				// add the animal to list to choose from
				// animals.Add(players[araIndex]->playerHand[j]);
				return true;
			}
		}

		if (players.Num() > 1 && animals.Num() == 0)
		{
			++araIndex;
		}
	}

	return false;
}

UCard* AProjectPurpleGameMode::AnimalRunsAwayStageTwo()
{
	UCard* card = nullptr;

	TArray<UCard*> animals;
	int araIndex = activePlayerIndex + 1;

	// need to find the next player in turn order with animal
	while (animals.Num() == 0)
	{
		if (araIndex > players.Num() - 1)
		{
			araIndex = 0;
		}

		for (int j = 0; j < players[araIndex]->playerHand.Num(); j++)
		{
			if (players[araIndex]->playerHand[j]->name == "Cow" || players[araIndex]->playerHand[j]->name == "Sheep" || players[araIndex]->playerHand[j]->name == "Donkey")
			{
				// add the animal to list to choose from
				animals.Add(players[araIndex]->playerHand[j]);
			}
		}

		if (players.Num() > 1 && animals.Num() == 0)
		{
			++araIndex;
		}
	}

	animalRunsIndex = araIndex;

	int cardIndToRemove = FMath::RandRange(0, animals.Num() - 1);

	card = animals[cardIndToRemove];

	players[araIndex]->playerHand.RemoveSingle(card);

	return card;
}

void AProjectPurpleGameMode::AnimalRunsAwayStageThree(ACardActor* card)
{
	int32 standing = 0;

	if (activePlayer->IsA(APurpleController::StaticClass()))
	{
		buttonChoice = retrieveButtonChoice();
	}
	else
	{
		standing = CheckPlayerStanding();

		// generous
		if (standing >= 0)
		{
			buttonChoice = 1;
		}
		// greedy
		else
		{
			buttonChoice = 0;
		}
	}

	switch (buttonChoice)
	{
		// case 0 = ignore, transgression to active player
	case 0:
		// subtract from standing for not
		if (activePlayer->IsA(APurpleController::StaticClass()))
		{
			static_cast<APurpleController*>(activePlayer)->SubtractFromPlayerStanding(1);
		}
		activePlayer->Transgressions.Add(card->CardData);
		break;
		// case 1 = return
	case 1:
		// add to standing for helping
		if (activePlayer->IsA(APurpleController::StaticClass()))
		{
			static_cast<APurpleController*>(activePlayer)->AddToPlayerStanding(1);
		}
		players[animalRunsIndex]->playerHand.Add(card->CardData);
		//SetTurnPhase(ETurnPhase::Discard);
		break;
	}
}

// these events will take input from an implementable event and resolve accordingly
bool AProjectPurpleGameMode::HoneyJarCracksStageOne()
{
	bool result = false;

	bool hJ = false;
	int playersChecked = 0;
	int hjIndex = activePlayerIndex + 1;

	// loop through all players to find the next in turn order with a honey jar
	while (hJ == false)
	{

		if (hjIndex > players.Num() - 1)
		{
			hjIndex = 0;
		}

		// checks each players hand for honey, breaks if found
		for (int j = 0; j < players[hjIndex]->playerHand.Num(); j++)
		{
			if (players[hjIndex]->playerHand[j]->name == "Jar of Honey")
			{
				jarOfHoney = players[hjIndex]->playerHand[j];
				hJ = true;
				break;
			}
		}

		if (hJ == true)
		{
			break;
		}
		else if (playersChecked == players.Num() - 1)
		{
			return result;
		}
		else if (hJ == false)
		{
			++hjIndex;
			++playersChecked;
		}
	}

	//For Wine
	bool pW = false;
	int pWIndex = hjIndex + 1;

	// For Empty Vessel
	bool eV = false;
	int evIndex = hjIndex + 1;

	// need to loop through after player with honey, to find next with a legal empty vessel or wine, depending

	if (currentEvent->name.Contains(TEXT("Honey Jar Cracks"), ESearchCase::CaseSensitive))
	{
		while (eV == false)
		{
			if (evIndex > players.Num() - 1)
			{
				evIndex = 0;
			}

			TArray<UCard*> legals = players[evIndex]->GetLegalResources();

			// checks each players hand for honey, breaks if found
			for (int j = 0; j < legals.Num(); j++)
			{
				if (legals[j]->name == "Empty Vessel")
				{
					emptyVessel = legals[j];
					eV = true;
					players[evIndex]->playerHand.RemoveSingle(emptyVessel);
					break;
				}
			}

			if (eV == true)
			{
				break;
			}
			else if (playersChecked == players.Num() - 1)
			{
				return result;
			}
			else
			{
				++playersChecked;
				++evIndex;
			}

		}

		if (hJ == true && eV == true)
		{
			honeyJarIndex = hjIndex;
			vesselIndex = evIndex;
			result = true;
		}
		else
		{
			result = false;
		}
	}
	else if (currentEvent->name.Contains(TEXT("Honey Jar cracks"), ESearchCase::CaseSensitive))
	{
		for (int i = 0; i < players.Num(); i++)
		{

			if (pWIndex > players.Num() - 1)
			{
				pWIndex = 0;
			}

			TArray<UCard*> legals = players[pWIndex]->GetLegalResources();

			// checks each players hand for honey, breaks if found
			for (int j = 0; j < legals.Num(); j++)
			{
				if (legals[j]->name == "Pitcher of Wine")
				{
					pitcherOfWine = legals[j];
					pW = true;
					players[pWIndex]->playerHand.RemoveSingle(pitcherOfWine);
					break;
				}
			}

			if (pW == true)
			{
				break;
			}
			else if (playersChecked == players.Num() - 1)
			{
				return result;
			}
			else
			{
				++playersChecked;
				++pWIndex;
			}

		}

		if (hJ == true && pW == true)
		{
			honeyJarIndex = hjIndex;
			vesselIndex = pWIndex;
			result = true;
		}
		else
		{
			result = false;
		}
	}

	return result;
}

void AProjectPurpleGameMode::HoneyJarCracksStageTwoNegotiatePass(TArray<class ACardActor*> cards)
{
	if (currentEvent->name.Contains(TEXT("Honey Jar Cracks"), ESearchCase::CaseSensitive))
	{
		// send cards passed in from swipe of honey jar player to the empty vessel player as agreed
		for (int i = cards.Num() - 1; i >= 0; i--)
		{
			players[honeyJarIndex]->playerHand.RemoveSingle(cards[i]->CardData);
			players[vesselIndex]->playerHand.Add(cards[i]->CardData);
		}

		// remove the empty vessel from hand
		players[vesselIndex]->playerHand.RemoveSingle(emptyVessel);
	}

	else if (currentEvent->name.Contains(TEXT("Honey Jar cracks"), ESearchCase::CaseSensitive))
	{
		// send cards passed in from swipe of honey jar player to the empty vessel player as agreed
		for (int i = cards.Num() - 1; i >= 0; i--)
		{
			players[honeyJarIndex]->playerHand.RemoveSingle(cards[i]->CardData);
			players[vesselIndex]->playerHand.Add(cards[i]->CardData);
		}

		// remove the empty vessel from hand
		players[vesselIndex]->playerHand.RemoveSingle(emptyVessel);
	}
}

void AProjectPurpleGameMode::HoneyJarCracksStageTwoNegotiateFail()
{
	// will act the same as ignore, empty vessel player recieves the honey jar from first player in transgressions
	if (currentEvent->name.Contains(TEXT("Honey Jar Cracks"), ESearchCase::CaseSensitive))
	{
		players[honeyJarIndex]->playerHand.RemoveSingle(jarOfHoney);
		players[vesselIndex]->Transgressions.Add(jarOfHoney);
		players[vesselIndex]->playerHand.Add(emptyVessel);
	}

	else if (currentEvent->name.Contains(TEXT("Honey Jar cracks"), ESearchCase::CaseSensitive))
	{
		players[honeyJarIndex]->playerHand.RemoveSingle(jarOfHoney);
		players[vesselIndex]->Transgressions.Add(jarOfHoney);
		players[vesselIndex]->playerHand.Add(pitcherOfWine);
	}
}

void AProjectPurpleGameMode::HoneyJarCracksStageTwoHelp()
{
	// empty vessel player loses vessel, but gains beyond the law
	if (currentEvent->name.Contains(TEXT("Honey Jar Cracks"), ESearchCase::CaseSensitive))
	{
		players[vesselIndex]->BeyondTheLaws.Add(emptyVessel);
	}

	else if (currentEvent->name.Contains(TEXT("Honey Jar cracks"), ESearchCase::CaseSensitive))
	{
		players[vesselIndex]->BeyondTheLaws.Add(pitcherOfWine);
	}
}

void AProjectPurpleGameMode::HoneyJarCracksStageTwoIgnore()
{
	// empty vessel player recieves the honey jar from first player in transgressions
	if (currentEvent->name.Contains(TEXT("Honey Jar Cracks"), ESearchCase::CaseSensitive))
	{
		players[honeyJarIndex]->playerHand.RemoveSingle(jarOfHoney);
		players[vesselIndex]->Transgressions.Add(jarOfHoney);
		players[vesselIndex]->playerHand.Add(emptyVessel);
	}

	else if (currentEvent->name.Contains(TEXT("Honey Jar cracks"), ESearchCase::CaseSensitive))
	{
		players[honeyJarIndex]->playerHand.RemoveSingle(jarOfHoney);
		players[vesselIndex]->Transgressions.Add(jarOfHoney);
		players[vesselIndex]->playerHand.Add(pitcherOfWine);
	}
}

// these events will take input from an implementable event and resolve accordingly
bool AProjectPurpleGameMode::DisasterPassed()
{
	bool result = true;

	if (currentEvent->name == "Khamsin (Windstorm)")
	{
		if (currentEvent->GetDoubleProperty("col_cost") <= 0)
		{
			// congrats kid, you did it
		}
		else
		{
			// game over
			result = false;
		}
	}
	else if (currentEvent->name == "Fire")
	{
		if (currentEvent->GetDoubleProperty("col_cost") <= 0)
		{
			// congrats kid, you did it
		}
		else
		{
			// game over
			result = false;
		}
	}
	else if (currentEvent->name == "Flood")
	{
		if (currentEvent->GetDoubleProperty("col_cost") <= 0)
		{
			// congrats kid, you did it
		}
		else
		{
			// game over
			result = false;
		}
	}

	return result;
}

ECrisisFailed AProjectPurpleGameMode::CrisisFail()
{
	for (int i = 0; i < activeCrises.Num(); i++)
	{
		if (activeCrises[i]->GetDoubleProperty("turns_active") == players.Num())
		{
			// check for which of the active crises has failed, will not be current event
			if (activeCrises[i]->name == "Plague")
			{
				activeCrises.RemoveSingle(activeCrises[i]);
				return ECrisisFailed::Plague;
			}
			else if (activeCrises[i]->name == "Monsoon")
			{
				activeCrises.RemoveSingle(activeCrises[i]);
				return ECrisisFailed::Monsoon;
			}
			else if (activeCrises[i]->name == "Locusts")
			{
				activeCrises.RemoveSingle(activeCrises[i]);

				// all players discard all of their hands
				for (int j = 0; j < players.Num(); j++)
				{
					for (int k = players[j]->playerHand.Num() - 1; k >= 0; k--)
					{
						// call individual player discard helper to send all of the cards in their hand to their respective locations, which will also add to transgressions if necessary
						players[j]->DiscardHelper(players[j]->playerHand[k]);
					}
				}

				return ECrisisFailed::Locusts;
			}
		}
	}

	return ECrisisFailed::None;
}

// these events will take input from an implementable event and resolve accordingly
void AProjectPurpleGameMode::AddToActiveCrises(UCard* crisis)
{
	if (crisis->name == "Plague")
	{
		activeCrises.Add(crisis);

		// need animation call here to recieve the cards to contribute from player
		//players[activePlayerIndex]->ContributeToCrisis();
	}
	else if (crisis->name == "Monsoon")
	{
		// cost is 2 times the amount of players in the game
		// just use the property setter to set col_cost to players.Num * 2
		crisis->SetDoubleProperty("col_cost", 2 * players.Num());

		activeCrises.Add(crisis);

		// need animation call here to recieve the cards to contribute from player
		//players[activePlayerIndex]->ContributeToCrisis();
	}
	else if (crisis->name == "Locusts")
	{
		activeCrises.Add(crisis);

		// need animation call here to recieve the cards to contribute from player
		//players[activePlayerIndex]->ContributeToCrisis();
	}
}

// all players draw a new family and communal responsibility
void AProjectPurpleGameMode::FestivalFinalStage()
{
	for (int i = 0; i < players.Num(); i++)
	{
		if (players[i]->playerFamilyResponsibilities.Num() > 0)
		{
			UCard* newFam = players[i]->playerFamilyResponsibilities.Pop();

			AdjustResponsibilityOnDraw(newFam);

			players[i]->playerActiveFamilyResponsibilities.Add(newFam);
		}
	}

	if (communalResponsibilities.Num() > 0)
	{
		UCard* newCom = communalResponsibilities.Pop();

		AdjustResponsibilityOnDraw(newCom);

		activeCommunalResponsibilities.Add(newCom);
	}
}

// helper functions for the game modes events
FHeshbon AProjectPurpleGameMode::GetHeshbonByType(EHeshbonType type)
{
	int maxWeight = 0;
	FHeshbon candidates[sizeof(HESHBON) / sizeof(FHeshbon)] = { { 0 } };

	// accumulated weight of all heshbon of the desired type.
	int candidatesCount = 0;
	for (auto h : HESHBON) {
		if (h.type == type) {
			maxWeight += h.weight;
			for (int32 i = 0; i < h.weight; i++)
			{
				candidates[candidatesCount++] = h;
			}
		}
	}

	/*
	static int rnd = 0;
	rnd += 1;
	if (rnd > maxWeight) rnd = 1;
	/*/
	// random number inside the accumulated weight range.
	int rnd = FMath::RandRange(1, maxWeight);
	//*/

	return candidates[rnd - 1];
}

// this is used in trading with another player and will swap the cards from both players' choices
void AProjectPurpleGameMode::SwapCards(const TArray<UCard*>& firstCards, const TArray<UCard*>& otherPlayerCards, ASuperController* firstPlayer, ASuperController* otherPlayer)
{
	// remove the card references from each players' hand
	for (int i = 0; i < firstCards.Num(); i++)
	{
		firstPlayer->playerHand.RemoveSingle(firstCards[i]);
	}

	for (int i = 0; i < otherPlayerCards.Num(); i++)
	{
		otherPlayer->playerHand.RemoveSingle(otherPlayerCards[i]);
	}



	// add the respective cards from each player into the other player's hand
	for (int i = 0; i < firstCards.Num(); i++)
	{
		//card->SetStringProperty("owner", otherPlayer->playerRole->name);
		otherPlayer->playerHand.Add(firstCards[i]);
	}

	for (int i = 0; i < otherPlayerCards.Num(); i++)
	{
		//card->SetStringProperty("owner", playerRole->name);
		firstPlayer->playerHand.Add(otherPlayerCards[i]);
	}
}

bool AProjectPurpleGameMode::AIWillingToTrade(ASuperController * humanPlayer, ASuperController * AIPlayer)
{
	TArray<UCard*> legals = AIPlayer->GetLegalResources();

	int32 standing = 0;

	if (legals.Num() > 0)
	{
		if (humanPlayer->IsA(APurpleController::StaticClass()))
		{
			standing = static_cast<APurpleController*>(activePlayer)->GetPlayerStanding();
		}

		if (standing >= 0)
		{
			return true;
		}
		else if (standing < 0)
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	return false;
}

// this is going to be used in the case where a human player initiates a trade with an AI player
TArray<UCard*> AProjectPurpleGameMode::TradeFromHumanToAI(const TArray<UCard*>& humanCards, ASuperController* humanPlayer, ASuperController * AIPlayer)
{
	TArray<UCard*> AIOffer;
	TArray<UCard*> AILegals;

	int32 sum = 0;
	int32 potterSum = 0;
	for (int i = 0; i < humanCards.Num(); i++)
	{
		//if AI is Potter, consider relevant cards to be of +1 value
		if (AIPlayer->roleString == "POTTER")
		{
			//checking for relevant cards
			if (humanCards[i]->name == "EMPTY VESSEL" || humanCards[i]->name == "JAR OF MILK"
				|| humanCards[i]->name == "PITCHER OF WINE" || humanCards[i]->name == "JAR OF HONEY")
			{
				sum += humanCards[i]->GetDoubleProperty("value")+1;
				potterSum+= humanCards[i]->GetDoubleProperty("value") + 1;
			}
		}
		else
			sum += humanCards[i]->GetDoubleProperty("value");
	}

	if (AIPlayer->IsA(APurpleAIController::StaticClass()))
	{
		AILegals = static_cast<APurpleAIController*>(AIPlayer)->SortCards(AIPlayer->GetLegalResources());
	}

	int32 legalSum = 0;

	int32 standing = 0;

	if (humanPlayer->IsA(APurpleController::StaticClass()))
	{
		standing = static_cast<APurpleController*>(activePlayer)->GetPlayerStanding();
	}

	if (AILegals.Num() > 0)
	{
		// check player standing in order to determine whether the ai will give a generous offer back or not
		if (standing >= 0)
		{
			// will give an ok offer
			for (int i = AILegals.Num() - 1; i >= 0; i--)
			{
				//checks if human is Potter to consider giving them relevant cards
				if (humanPlayer->roleString == "POTTER")
				{
					if (AILegals[i]->name == "EMPTY VESSEL" || AILegals[i]->name == "JAR OF MILK"
						|| AILegals[i]->name == "PITCHER OF WINE" || AILegals[i]->name == "JAR OF HONEY")
					{
						int oldValue = AILegals[i]->GetDoubleProperty("value");
						AILegals[i]->SetDoubleProperty("value", oldValue + 1);
					}
				}
				legalSum += AILegals[i]->GetDoubleProperty("value");
				AIOffer.Add(AILegals[i]);

				if (legalSum >= (sum - 1))
				{
					break;
				}
			}
		}
		else
		{
			// will propose a lower offer than received
			for (int i = AILegals.Num() - 1; i >= 0; i--)
			{
				//checks if human is Potter to consider giving them relevant cards
				if (humanPlayer->roleString == "POTTER")
				{
					if (AILegals[i]->name == "EMPTY VESSEL" || AILegals[i]->name == "JAR OF MILK"
						|| AILegals[i]->name == "PITCHER OF WINE" || AILegals[i]->name == "JAR OF HONEY")
					{
						int oldValue = AILegals[i]->GetDoubleProperty("value");
						AILegals[i]->SetDoubleProperty("value", oldValue + 1);
					}
				}
				legalSum += AILegals[i]->GetDoubleProperty("value");
				AIOffer.Add(AILegals[i]);

				if (legalSum >= (sum - 3))
				{
					break;
				}
			}
		}
	}
	return AIOffer;
}

// used for changing the current owner property on a card, most often when trading a legally owned resource to another player
void AProjectPurpleGameMode::ChangeOwnerOnCard(UCard * card, ASuperController * player)
{
	/*if (card->ContainsProperty("owner"))
	{
	card->SetStringProperty("owner", FString(player->playerRole->name));
	}*/
	FString playName = player->playerRole->name;

	if (player->trade == true)
	{
		card->SetDoubleProperty("value", (card->GetDoubleProperty("value") + 1));
	}
	card->currentOwner = playName;
}

// tallies the player standing amongst all of the human players at the table by ai to gague generosity or selfishness
int32 AProjectPurpleGameMode::CheckPlayerStanding()
{
	int sum = 0;

	for (int i = 0; i < players.Num(); i++)
	{
		if (players[i]->IsA(APurpleController::StaticClass()))
		{
			APurpleController* humanPlayer = static_cast<APurpleController*>(players[i]);
			sum += humanPlayer->GetPlayerStanding();
		}
	}

	return sum;
}

// returns the index for player with least family responsibilities completed in turn order
int32 AProjectPurpleGameMode::getPlayerWithLowestFamily()
{
	int lfIndex = activePlayerIndex;
	int minFam = players[lfIndex]->playerCompletedFamilyResponsibilities.Num();

	// loops through players to find lowest family # completed
	for (int i = 0; i < players.Num(); i++)
	{
		if (lfIndex > players.Num() - 1)
		{
			lfIndex = 0;
		}

		// finding the lowest number first, since there may be ties among players

		if (players[lfIndex]->playerCompletedFamilyResponsibilities.Num() < minFam)
		{
			minFam = players[lfIndex]->playerCompletedFamilyResponsibilities.Num();
			break;
		}

		++lfIndex;
	}

	lfIndex = activePlayerIndex;
	while (true)
	{
		if (lfIndex > players.Num() - 1)
		{
			lfIndex = 0;
		}

		// finding the lowest in turn order
		if (players[lfIndex]->playerCompletedFamilyResponsibilities.Num() == minFam)
		{
			break;
		}

		++lfIndex;
	}

	return lfIndex;
}

// returns the index for player with most family responsibilities completed in turn order
int32 AProjectPurpleGameMode::getPlayerWithHighestFamily()
{
	int hfIndex = activePlayerIndex;
	int maxFam = players[hfIndex]->playerCompletedFamilyResponsibilities.Num();

	// loops through players to find highest family # completed
	for (int i = 0; i < players.Num(); i++)
	{
		if (hfIndex > players.Num() - 1)
		{
			hfIndex = 0;
		}

		// finding the highest number first, since there may be ties among players

		if (players[hfIndex]->playerCompletedFamilyResponsibilities.Num() > maxFam)
		{
			maxFam = players[hfIndex]->playerCompletedFamilyResponsibilities.Num();
			break;
		}

		++hfIndex;
	}

	hfIndex = activePlayerIndex;
	while (true)
	{
		if (hfIndex > players.Num() - 1)
		{
			hfIndex = 0;
		}

		// finding the highest in turn order
		if (players[hfIndex]->playerCompletedFamilyResponsibilities.Num() == maxFam)
		{
			break;
		}

		++hfIndex;
	}

	return hfIndex;
}

// Sorts array so that all the owners are consecutive
FArrayOfCardArrays AProjectPurpleGameMode::SortArrayByOwner(TArray<class UCard*> cards)
{
	FArrayOfCardArrays sortedCards;

	TArray<UCard*> dateFarmersCards;
	TArray<UCard*> strangersCards;
	TArray<UCard*> vintnersCards;
	TArray<UCard*> potterCards;
	TArray<UCard*> cowherdsCards;
	TArray<UCard*> shepherdCards;
	TArray<UCard*> anyCards;

	for (int i = 0; i < cards.Num(); i++)
	{
		if (cards[i]->currentOwner == "Date Farmer")
		{
			dateFarmersCards.Add(cards[i]);
		}
		else if (cards[i]->currentOwner == "Vintner")
		{
			vintnersCards.Add(cards[i]);
		}
		else if (cards[i]->currentOwner == "Stranger")
		{
			strangersCards.Add(cards[i]);
		}
		else if (cards[i]->currentOwner == "Potter")
		{
			potterCards.Add(cards[i]);
		}
		else if (cards[i]->currentOwner == "Cowherd")
		{
			cowherdsCards.Add(cards[i]);
		}
		else if (cards[i]->currentOwner == "Shepherd")
		{
			shepherdCards.Add(cards[i]);
		}
		else if (cards[i]->currentOwner == "Any")
		{
			shepherdCards.Add(cards[i]);
		}
	}

	sortedCards.dateFarmersCards = dateFarmersCards;
	sortedCards.strangersCards = strangersCards;
	sortedCards.vintnersCards = vintnersCards;
	sortedCards.potterCards = potterCards;
	sortedCards.cowherdsCards = cowherdsCards;
	sortedCards.shepherdCards = shepherdCards;
	sortedCards.anyCards = anyCards;

	//    sortedCards.Append(dateFarmersCards);
	//    sortedCards.Append(strangersCards);
	//    sortedCards.Append(vintnersCards);
	//    sortedCards.Append(potterCards);
	//    sortedCards.Append(cowherdsCards);
	//    sortedCards.Append(shepherdCards);
	//    sortedCards.Append(anyCards);






	return sortedCards;
}


