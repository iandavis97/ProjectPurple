// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectPurple.h"
#include "PurpleAIController.h"
#include "Math.h"
#include "CardActor.h"
#include <algorithm>
#include <typeinfo>
#include "ProjectPurpleGameMode.h"


// Sets default values
APurpleAIController::APurpleAIController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	responsibilitiesAbleToContribute = 1;
	numToDraw = 2;
	handLimit = 3;
	marriage = false;
	swim = false;
	trade = false;
	//gender = "Male";
	int32 genderDeterminant = FMath::RandRange(0, 10);
	if (genderDeterminant > 5)
	{
		gender = "Male";
	}
	else
	{
		gender = "Female";
	}

}




// Is called when the player wants to draw resources- This also adds the cards to the hand of the player, variable based on responsibilities, which change numtodraw
TArray<UCard*> APurpleAIController::DrawResources()
{
	TArray<UCard*> newCards;

	AProjectPurpleGameMode* currentGameMode = static_cast<AProjectPurpleGameMode*>(GetWorld()->GetAuthGameMode());

	for (int i = 0; i < numToDraw; i++)
	{
		if (currentGameMode->resourcesList.Num() <= 0)
		{
			currentGameMode->ShuffleCards(currentGameMode->resourcesListDiscard);
			currentGameMode->resourcesList.Append(currentGameMode->resourcesListDiscard);
			currentGameMode->resourcesListDiscard.Empty();
		}

		UCard* draw = currentGameMode->resourcesList.Pop(true);

		if (trade == true && draw->currentOwner == playerRole->name)
		{
			draw->SetDoubleProperty("value", (draw->GetDoubleProperty("value") + 1));
		}

		if (draw->currentOwner != playerRole->name && draw->currentOwner != "Any")
		{
			playerFoundObjects.Add(draw);
		}

		playerHand.Add(draw);
		newCards.Add(draw);
	}

	return newCards;
}

// draws a single lost resource, used for events
UCard* APurpleAIController::DrawLostResource()
{
	AProjectPurpleGameMode* currentGameMode = static_cast<AProjectPurpleGameMode*>(GetWorld()->GetAuthGameMode());

	UCard* draw = currentGameMode->lostResources.Pop(true);

	playerHand.Add(draw);

	playerFoundObjects.Add(draw);

	return draw;
}

// draws a single resource, for events mostly
UCard* APurpleAIController::DrawResource()
{
	AProjectPurpleGameMode* currentGameMode = static_cast<AProjectPurpleGameMode*>(GetWorld()->GetAuthGameMode());

	if (currentGameMode->resourcesList.Num() <= 0)
	{
		currentGameMode->ShuffleCards(currentGameMode->resourcesListDiscard);
		currentGameMode->resourcesList.Append(currentGameMode->resourcesListDiscard);
	}

	UCard* draw = currentGameMode->resourcesList.Pop(true);

	if (trade == true && draw->currentOwner == playerRole->name)
	{
		draw->SetDoubleProperty("value", (draw->GetDoubleProperty("value") + 1));
	}

	if (draw->currentOwner != playerRole->name && draw->currentOwner != "Any")
	{
		playerFoundObjects.Add(draw);
	}

	playerHand.Add(draw);

	return draw;
}

UCard* APurpleAIController::DrawResourceDiscard()
{
	AProjectPurpleGameMode* currentGameMode = static_cast<AProjectPurpleGameMode*>(GetWorld()->GetAuthGameMode());

	int range = currentGameMode->resourcesListDiscard.Num() - 0 + 1;
	int num = rand() % range + 0;

	UCard* randDraw = currentGameMode->resourcesListDiscard[num];
	currentGameMode->resourcesListDiscard.RemoveAt(num);


	if (trade == true && randDraw->currentOwner == playerRole->name)
	{
		randDraw->SetDoubleProperty("value", (randDraw->GetDoubleProperty("value") + 1));
	}

	if (randDraw->currentOwner != playerRole->name && randDraw->currentOwner != "Any")
	{
		playerFoundObjects.Add(randDraw);
	}

	playerHand.Add(randDraw);

	return randDraw;
}

// Is called when we want to draw events. This only draws the cards, it does not execute the resolution of the events.
void APurpleAIController::DrawEvent()
{
	AProjectPurpleGameMode* currentGameMode = static_cast<AProjectPurpleGameMode*>(GetWorld()->GetAuthGameMode());
	UCard* draw = currentGameMode->eventsList.Pop(true);
	currentGameMode->currentEvent = draw;

	if (currentGameMode->currentEvent->type == "Community - Disaster" && swim)
	{
		currentGameMode->currentEvent->SetDoubleProperty("col_cost", (currentGameMode->currentEvent->GetDoubleProperty("col_cost") - 4));
	}
}

// Is called to set the player roles
void APurpleAIController::SetRole()
{
	int32 ind;

	if (gender == "Male")
	{
		// receive the game mode in order to get list of players and check the roles given to each
		AProjectPurpleGameMode* currentGameMode = static_cast<AProjectPurpleGameMode*>(GetWorld()->GetAuthGameMode());

		if (currentGameMode->maleRolesList.Num() > 1)
		{
			ind = FMath::RandRange(0, (currentGameMode->maleRolesList.Num() - 1));
		}
		else
		{
			ind = 0;
		}

		playerRole = currentGameMode->maleRolesList[ind];
		roleString = FString(playerRole->name);
		currentGameMode->maleRolesList.RemoveAt(ind);
		currentGameMode->femaleRolesList.RemoveAt(ind);
	}
	else
	{
		// receive the game mode in order to get list of players and check the roles given to each
		AProjectPurpleGameMode* currentGameMode = static_cast<AProjectPurpleGameMode*>(GetWorld()->GetAuthGameMode());

		if (currentGameMode->femaleRolesList.Num() > 1)
		{
			ind = FMath::RandRange(0, (currentGameMode->femaleRolesList.Num() - 1));
		}
		else
		{
			ind = 0;
		}

		playerRole = currentGameMode->femaleRolesList[ind];
		currentGameMode->femaleRolesList.RemoveAt(ind);
		currentGameMode->maleRolesList.RemoveAt(ind);
	}
}

// called when players owned resources are needed, returns a ucard array back
TArray<class UCard*> APurpleAIController::GetLegalResources()
{
	TArray<class UCard*> legals;

	// loops through hand checking if resource is legal or not
	for (int i = 0; i < playerHand.Num(); i++)
	{
		if (playerHand[i]->currentOwner == "Any" || playerHand[i]->currentOwner == playerRole->name 
			|| (playerHand[i]->type == "Individual - Found")&& !Transgressions.Contains(playerHand[i]))
		{
			legals.Add(playerHand[i]);
		}
	}
	return legals;
}

void APurpleAIController::DiscardHelper(UCard * card)
{
	AProjectPurpleGameMode* currentGameMode = static_cast<AProjectPurpleGameMode*>(GetWorld()->GetAuthGameMode());

	playerHand.RemoveSingle(card);
	playerFoundObjects.RemoveSingle(card);

	// check if player is legal owner, if not add a transgression
	if ((card->currentOwner != "Any" && card->currentOwner != playerRole->name && card->type != "Individual - Found") || card->currentOwner == "Stranger")
	{
		if (this->IsA(APurpleAIController::StaticClass()))
		{
			Transgressions.Add(card);
		}
	}
	else
	{
		// add to correct discard pile based on the type of card
		if (card->type == "Individual - Found" || card->type == "Individual - Lost")
		{
			currentGameMode->eventsListDiscard.Add(card);
			handLimit -= 1;
		}

		else if (card->type == "Resource")
		{
			card->currentOwner = (card->GetStringProperty("owner"));
			card->SetDoubleProperty("value", (card->GetDoubleProperty("initial_value")));
			currentGameMode->resourcesListDiscard.Add(card);
			//tracking discard for shepherd power
			currentGameMode->shepherdPowerCard = card;
		}

		else if (card->type == "Starting Resource")
		{
			if (card)
			{
				card = nullptr;
				GetWorld()->ForceGarbageCollection(true);
			}
		}

		else if (card->type == "Lost Resource")
		{
			card->currentOwner = (card->GetStringProperty("owner"));
			card->SetDoubleProperty("value", (card->GetDoubleProperty("initial_value")));
			currentGameMode->lostResourcesDiscard.Add(card);
			//tracking discard for shepherd power
			currentGameMode->shepherdPowerCard = card;
		}
	}
}




// need new implementations ----------------------------------------------------------------------------------------------------

// used for making Teshuvah
UCard* APurpleAIController::Teshuvah(int32 costModifier)
{
	AProjectPurpleGameMode* currentGameMode = static_cast<AProjectPurpleGameMode*>(GetWorld()->GetAuthGameMode());

	TArray<UCard*> contributions;

	// need to get the highest cost
	UCard* transgression = nullptr;

	int index = 0;
	int max = Transgressions[0]->GetDoubleProperty("value");

	for (int i = 0; i < Transgressions.Num(); i++)
	{
		if (Transgressions[i]->GetDoubleProperty("value") > max)
		{
			max = Transgressions[i]->GetDoubleProperty("value");
			index = i;
		}
	}

	transgression = Transgressions[index];

	if (teshuvahsCompleted == 0)
	{
		costModifier += 1;
	}

	costModifier += teshuvahsCompleted;

	// Sum of Contributions
	int legalSum = 0;
	TArray<UCard*> legals = GetLegalResources();

	for (int i = 0; i < legals.Num(); i++)
	{
		legalSum += legals[i]->GetDoubleProperty("value");

		if (legalSum >= transgression->GetDoubleProperty("value") + costModifier)
		{
			for (int j = 0; j <= i; j++)
			{
				contributions.Add(legals[j]);
			}

			for (int k = contributions.Num() - 1; k >= 0; k--)
			{
				DiscardHelper(contributions[k]);
			}

			Transgressions.RemoveSingle(transgression);

			if (transgression->type == "Individual - Found" || transgression->type == "Individual - Lost")
			{
				currentGameMode->eventsListDiscard.Add(transgression);
			}

			else if (transgression->type == "Resource")
			{
				transgression->currentOwner = transgression->GetStringProperty("owner");
				transgression->SetDoubleProperty("value", (transgression->GetDoubleProperty("initial_value")));
				currentGameMode->resourcesListDiscard.Add(transgression);
			}

			else if (transgression->type == "Starting Resource")
			{
				transgression->currentOwner = (transgression->GetStringProperty("owner"));
				transgression->SetDoubleProperty("value", (transgression->GetDoubleProperty("initial_value")));
			}

			else if (transgression->type == "Lost Resource")
			{
				transgression->currentOwner = transgression->GetStringProperty("owner");
				transgression->SetDoubleProperty("value", (transgression->GetDoubleProperty("initial_value")));
				currentGameMode->lostResourcesDiscard.Add(transgression);
			}

			teshuvahsCompleted++;

			break;
		}
	}

	return transgression;
}

// called when a player needs to contribute to communal
TArray<class UCard*> APurpleAIController::ContributeToCommunalResponsibility()
{
	AProjectPurpleGameMode* currentGameMode = static_cast<AProjectPurpleGameMode*>(GetWorld()->GetAuthGameMode());

	// the first element of this array will be the communal paying for, and the rest will be the cards donated
	TArray<UCard*> returns;
	UCard* communal = currentGameMode->activeCommunalResponsibilities[FindLowestCommunal()];
	returns.Add(communal);


	// sorting the legal array
	TArray<UCard*> legals = SortCards(GetLegalResources());
	int total = 0;
	int turnsLeft = currentGameMode->eventsList.Num();

	if (legals.Num() > 0)
	{
		if (turnsLeft > 15)
		{
			// to frontload the game we need to have the ai paying more for communals to try and help the player and board get to 6
			// ai will split payments without consideration to player to help out
			int avg = FMath::CeilToInt(communal->GetDoubleProperty("cost") / (currentGameMode->players.Num() - 1));

			int midIndex = FMath::CeilToInt(legals.Num() / 2);

			if (legals[midIndex]->GetDoubleProperty("value") >= avg)
			{
				legals[midIndex] = UsePotterPower(legals[midIndex]);
				total += legals[midIndex]->GetDoubleProperty("value");
				returns.Add(legals[midIndex]);
			}
			else
			{
				for (int i = midIndex; i < legals.Num(); i++)
				{
					legals[midIndex] = UsePotterPower(legals[midIndex]);
					total += legals[i]->GetDoubleProperty("value");
					returns.Add(legals[i]);
				}
			}
		}
		else
		{
			int avg = FMath::FloorToInt(communal->GetDoubleProperty("cost") / currentGameMode->players.Num());

			int midIndex = FMath::FloorToInt(legals.Num() / 2);

			if (legals[midIndex]->GetDoubleProperty("value") >= avg)
			{
				legals[midIndex] = UsePotterPower(legals[midIndex]);
				total += legals[midIndex]->GetDoubleProperty("value");
				returns.Add(legals[midIndex]);
			}
			else
			{
				for (int i = midIndex; i < legals.Num(); i++)
				{
					legals[midIndex] = UsePotterPower(legals[midIndex]);
					total += legals[i]->GetDoubleProperty("value");
					returns.Add(legals[i]);
				}
			}
		}
	}

	float newValue = communal->GetDoubleProperty("cost");
	newValue -= total;

	if (newValue <= 0)
	{
		newValue = 0;
	}

	communal->SetDoubleProperty("cost", newValue);

	// paid for
	if (communal->GetDoubleProperty("cost") <= 0)
	{
		// adds the communal to the completed list to track progress, and removes the communal from the active list showing on screen
		currentGameMode->completedCommunalResponsibilities.Add(communal);
		currentGameMode->activeCommunalResponsibilities.RemoveSingle(communal);

		for (int i = 0; i < currentGameMode->activeCommunalResponsibilities.Num(); i++)
		{
			currentGameMode->AdjustResponsibilityValue(currentGameMode->activeCommunalResponsibilities[i]);
		}

		if (currentGameMode->communalResponsibilities.Num() > 0)
		{
			UCard* newCom = currentGameMode->communalResponsibilities.Pop();

			// need to fully update this communal
			currentGameMode->AdjustResponsibilityOnDraw(newCom);

			currentGameMode->activeCommunalResponsibilities.Add(newCom);
		}

		for (auto i : currentGameMode->players)
		{
			for (auto j : i->playerActiveFamilyResponsibilities)
			{
				currentGameMode->AdjustResponsibilityValue(j);
			}
		}

		for (int i = returns.Num() - 1; i > 0; i--)
		{
			DiscardHelper(returns[i]);
		}
	}
	else
	{
		for (int i = returns.Num() - 1; i > 0; i--)
		{
			DiscardHelper(returns[i]);
		}
		UE_LOG(LogTemp, Warning, TEXT("Cost = %d"), communal->GetDoubleProperty("cost"));
	}

	// sort to show contributed cards highest to lowest
	if (returns.Num() > 0)
	{
		returns.RemoveAt(0);
		SortCards(returns);
		returns.Insert(communal, 0);
	}

	return returns;
}

TArray<class UCard*> APurpleAIController::SortCards(TArray<class UCard *> cards)
{

	//Bubble Sort Cards
	int i, j, flag = 1;
	UCard* temp;
	int length = cards.Num();
	for (i = 1; (i < length) && flag; i++)
	{
		flag = 0;
		for (j = 0; j < (length - 1); j++)
		{
			if (cards[j + 1]->GetDoubleProperty("value") > cards[j]->GetDoubleProperty("value"))
			{
				temp = cards[j];
				cards[j] = cards[j + 1];
				cards[j + 1] = temp;
				flag = 1;
			}
		}
	}

	//Return sorted cards
	return cards;
}

// called when player needs to contribute to a disaster
TArray<UCard*> APurpleAIController::ContributeToDisaster()
{
	AProjectPurpleGameMode* currentGameMode = static_cast<AProjectPurpleGameMode*>(GetWorld()->GetAuthGameMode());

	// the first element of this array will be the disaster paying for, and the rest will be the cards donated
	TArray<UCard*> returns;

	UCard* disaster = currentGameMode->currentEvent;
	returns.Add(disaster);

	TArray<UCard*> legals = SortCards(GetLegalResources());
	int total = 0;


	// ai will split payments without consideration to player to help out
	int avg = FMath::CeilToInt(disaster->GetDoubleProperty("col_cost") / currentGameMode->players.Num() - 1);

	if (legals.Num() > 0)
	{
		int midIndex = FMath::FloorToInt(legals.Num() / 2);

		if (legals[midIndex]->GetDoubleProperty("value") >= avg)
		{
			legals[midIndex] = UsePotterPower(legals[midIndex]);
			total += legals[midIndex]->GetDoubleProperty("value");
			returns.Add(legals[midIndex]);
		}
		else
		{
			for (int i = midIndex; i < legals.Num(); i++)
			{
				legals[midIndex] = UsePotterPower(legals[midIndex]);
				total += legals[i]->GetDoubleProperty("value");
				returns.Add(legals[i]);
			}
		}
	}

	// check for bonus based on amount paid 
	int bonus = 0;
	if (this == currentGameMode->activePlayer)
	{
		if (currentGameMode->currentEvent->name == "Khamsin (Windstorm)")
		{
			// for every 5 paid
			bonus = FMath::FloorToInt(total / 5);
		}

		else if (currentGameMode->currentEvent->name == "Fire")
		{
			// for every 3 paid
			bonus = FMath::FloorToInt(total / 3);
		}

		else if (currentGameMode->currentEvent->name == "Flood")
		{
			// for every 4 paid
			bonus = FMath::FloorToInt(total / 4);
		}
	}

	// sets the cost property to the difference of the original cost and total contributed by player this turn
	float newValue = disaster->GetDoubleProperty("col_cost");
	newValue -= total;
	newValue -= bonus;

	if (newValue <= 0)
	{
		newValue = 0;
	}

	disaster->SetDoubleProperty("col_cost", newValue);

	// paid for
	if (disaster->GetDoubleProperty("col_cost") <= 0)
	{
		for (int i = returns.Num() - 1; i > 0; i--)
		{
			DiscardHelper(returns[i]);
		}
	}
	else
	{
		for (int i = returns.Num() - 1; i > 0; i--)
		{
			DiscardHelper(returns[i]);
		}
		UE_LOG(LogTemp, Warning, TEXT("Cost = %d"), disaster->GetDoubleProperty("col_cost"));
	}

	// sort to show contributed cards highest to lowest
	if (returns.Num() > 0)
	{
		returns.RemoveAt(0);
		SortCards(returns);
		returns.Insert(disaster, 0);
	}

	return returns;
}

// called when a player needs to contribute to crisis
TArray<UCard*> APurpleAIController::ContributeToCrisis()
{
	AProjectPurpleGameMode* currentGameMode = static_cast<AProjectPurpleGameMode*>(GetWorld()->GetAuthGameMode());

	// the first element of this array will be the disaster paying for, and the rest will be the cards donated
	TArray<UCard*> returns;

	// need to get crisis that has been in play longest
	int highestTurnsActive = currentGameMode->activeCrises[0]->GetDoubleProperty("turns_active");
	int highestTurnIndex = 0;
	UCard* crisis = nullptr;

	for (int i = 0; i < currentGameMode->activeCrises.Num(); i++)
	{
		if (currentGameMode->activeCrises[i]->GetDoubleProperty("turns_active") > highestTurnsActive)
		{
			highestTurnsActive = currentGameMode->activeCrises[i]->GetDoubleProperty("turns_active");
			highestTurnIndex = i;
		}
	}

	crisis = currentGameMode->activeCrises[highestTurnIndex];
	returns.Add(crisis);
	int total = 0;

	// find the amount needed for the disaster and give the average amount (col_cost / number of players) ?
	TArray<UCard*> legals = SortCards(GetLegalResources());

	int avg = FMath::CeilToInt(crisis->GetDoubleProperty("col_cost") / currentGameMode->players.Num() - 1);

	if (legals.Num() > 0)
	{
		int midIndex = FMath::FloorToInt(legals.Num() / 2);

		if (legals[midIndex]->GetDoubleProperty("value") >= avg)
		{
			legals[midIndex] = UsePotterPower(legals[midIndex]);
			total += legals[midIndex]->GetDoubleProperty("value");
			returns.Add(legals[midIndex]);
		}
		else
		{
			for (int i = midIndex; i < legals.Num(); i++)
			{
				legals[midIndex] = UsePotterPower(legals[midIndex]);
				total += legals[i]->GetDoubleProperty("value");
				returns.Add(legals[i]);
			}
		}
	}


	// sets the cost property to the difference of the original cost and total contributed by player this turn
	float newValue = crisis->GetDoubleProperty("col_cost");
	newValue -= total;

	if (newValue <= 0)
	{
		newValue = 0;
	}

	crisis->SetDoubleProperty("col_cost", newValue);

	// paid for
	if (crisis->GetDoubleProperty("col_cost") <= 0)
	{
		currentGameMode->activeCrises.Remove(crisis);

		for (int i = returns.Num() - 1; i > 0; i--)
		{
			DiscardHelper(returns[i]);
		}
	}
	else
	{
		for (int i = returns.Num() - 1; i > 0; i--)
		{
			DiscardHelper(returns[i]);
		}
		UE_LOG(LogTemp, Warning, TEXT("Cost = %d"), crisis->GetDoubleProperty("col_cost"));
	}

	// sort to show contributed cards highest to lowest
	if (returns.Num() > 0)
	{
		returns.RemoveAt(0);
		SortCards(returns);
		returns.Insert(crisis, 0);
	}

	return returns;
}

// called when player needs to contribute to family
bool APurpleAIController::ShouldContributeToFamily()
{
	AProjectPurpleGameMode* currentGameMode = static_cast<AProjectPurpleGameMode*>(GetWorld()->GetAuthGameMode());

	if (playerActiveFamilyResponsibilities.Num() == 0)
	{
		return false;
	}

	int index = FindLowestCostFamily();
	UCard* family = playerActiveFamilyResponsibilities[index];

	TArray<UCard*> legals = SortCards(GetLegalResources());
	int total = 0;

	if (legals.Num() > 0)
	{
		for (int i = 0; i < legals.Num(); i++)
		{
			legals[i] = UsePotterPower(legals[i]);
			total += legals[i]->GetDoubleProperty("value");

			if (total >= family->GetDoubleProperty("cost"))
			{
				return true;
			}
		}
	}
	else
	{
		if (currentGameMode->eventsList.Num() < 15)
		{
			for (int i = 0; i < playerHand.Num(); i++)
			{
				playerHand[i] = UsePotterPower(playerHand[i]);
				total += playerHand[i]->GetDoubleProperty("value");

				if (total >= family->GetDoubleProperty("cost"))
				{
					return true;
				}
			}
		}
	}
	

	return false;
}

bool APurpleAIController::ShouldContributeToFamilyEndGame(int32 costModifer)
{
	AProjectPurpleGameMode* currentGameMode = static_cast<AProjectPurpleGameMode*>(GetWorld()->GetAuthGameMode());

	if (playerActiveFamilyResponsibilities.Num() == 0)
	{
		return false;
	}

	int index = FindLowestCostFamily();
	UCard* family = playerActiveFamilyResponsibilities[index];

	family->SetDoubleProperty("cost", family->GetDoubleProperty("cost") + costModifer);

	TArray<UCard*> legals = SortCards(GetLegalResources());
	int total = 0;

	if (legals.Num() > 0)
	{
		for (int i = 0; i < legals.Num(); i++)
		{
			legals[i] = UsePotterPower(legals[i]);
			total += legals[i]->GetDoubleProperty("value");

			if (total >= family->GetDoubleProperty("cost"))
			{
				return true;
			}
		}
	}
	else
	{
		if (currentGameMode->eventsList.Num() < 15)
		{
			for (int i = 0; i < playerHand.Num(); i++)
			{
				playerHand[i] = UsePotterPower(playerHand[i]);
				total += playerHand[i]->GetDoubleProperty("value");

				if (total >= family->GetDoubleProperty("cost"))
				{
					return true;
				}
			}
		}
	}


	return false;
}

bool APurpleAIController::ShouldContributeToCommunal()
{
	AProjectPurpleGameMode* currentGameMode = static_cast<AProjectPurpleGameMode*>(GetWorld()->GetAuthGameMode());
	TArray<UCard*> legals = GetLegalResources();
	int standing = currentGameMode->CheckPlayerStanding();

	if (legals.Num() > 0 && currentGameMode->activeCommunalResponsibilities.Num() > 0 && currentGameMode->completedCommunalResponsibilities.Num() < 6 && standing >= 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

TArray<UCard*> APurpleAIController::ContributeToFamilyResponsibility(int32 costModifier)
{
	AProjectPurpleGameMode* currentGameMode = static_cast<AProjectPurpleGameMode*>(GetWorld()->GetAuthGameMode());
	int index = FindLowestCostFamily();
	TArray<UCard*> familyReturn;

	UCard* family = playerActiveFamilyResponsibilities[index];

	// first index is the family card contributed to
	familyReturn.Add(family);

	int sum = 0;

	// check if can be done with legals only
	TArray<UCard*> legals = SortCards(GetLegalResources());
	TArray<UCard*> contributions;
	int urgency = currentGameMode->eventsList.Num(); // how many events are left before end
	bool completed = false;

	if (legals.Num() > 0)
	{
		for (int i = 0; i < legals.Num(); i++)
		{
			legals[i] = UsePotterPower(legals[i]);
			sum += legals[i]->GetDoubleProperty("value");

			if (sum >= family->GetDoubleProperty("cost"))
			{
				completed = true;

				if (family->name == "Teach The Law")
				{
					responsibilitiesAbleToContribute = 2;
				}

				else if (family->name == "Redemption of Firstborn")
				{
					handLimit += 1;
				}

				else if (family->name == "Celebrate Marriage")
				{
					marriage = true;
				}

				else if (family->name == "Teach to Swim")
				{
					swim = true;
				}

				else if (family->name == "Teach a Trade")
				{
					trade = true;
				}


				for (int j = 0; j <= i; j++)
				{
					legals[j] = UsePotterPower(legals[j]);
					contributions.Add(legals[j]);
				}



				float newValue = family->GetDoubleProperty("cost");
				newValue -= sum;

				if (newValue <= 0)
				{
					newValue = 0;
				}

				family->SetDoubleProperty("cost", newValue);

				for (int k = contributions.Num() - 1; k >= 0; k--)
				{
					familyReturn.Add(contributions[k]);
					DiscardHelper(contributions[k]);
				}

				playerActiveFamilyResponsibilities.RemoveSingle(family);
				playerCompletedFamilyResponsibilities.Add(family);

				if (playerFamilyResponsibilities.Num() > 0)
				{
					UCard* newFam = playerFamilyResponsibilities.Pop();

					currentGameMode->AdjustResponsibilityOnDraw(newFam);

					playerActiveFamilyResponsibilities.Add(newFam);
				}

				return familyReturn;
			}
		}
	}

	if (completed != true)
	{
		sum = 0;
		TArray<UCard*> sortedHand = SortCards(playerHand);

		for (int i = 0; i < sortedHand.Num(); i++)
		{
			sortedHand[i] = UsePotterPower(sortedHand[i]);
			sum += sortedHand[i]->GetDoubleProperty("value");

			if (sum >= family->GetDoubleProperty("cost"))
			{
				if (family->name == "Teach The Law")
				{
					responsibilitiesAbleToContribute = 2;
				}

				else if (family->name == "Redemption of Firstborn")
				{
					handLimit += 1;
				}

				else if (family->name == "Celebrate Marriage")
				{
					marriage = true;
				}

				else if (family->name == "Teach to Swim")
				{
					swim = true;
				}

				else if (family->name == "Teach a Trade")
				{
					trade = true;
				}

				for (int j = 0; j <= i; j++)
				{
					sortedHand[j] = UsePotterPower(sortedHand[j]);
					contributions.Add(sortedHand[j]);
				}

				float newValue = family->GetDoubleProperty("cost");
				newValue -= sum;

				if (newValue <= 0)
				{
					newValue = 0;
				}

				family->SetDoubleProperty("cost", newValue);

				for (int k = contributions.Num() - 1; k >= 0; k--)
				{
					familyReturn.Add(contributions[k]);
					DiscardHelper(contributions[k]);
				}

				playerActiveFamilyResponsibilities.RemoveSingle(family);
				playerCompletedFamilyResponsibilities.Add(family);

				if (playerFamilyResponsibilities.Num() > 0)
				{
					UCard* newFam = playerFamilyResponsibilities.Pop();

					currentGameMode->AdjustResponsibilityOnDraw(newFam);

					playerActiveFamilyResponsibilities.Add(newFam);
				}

				return familyReturn;
			}
		}
	}

	return familyReturn;
}

// this event is called when a player needs to discard at the end of a turn during the aforementioned phase
TArray<class UCard*> APurpleAIController::Discard()
{
	AProjectPurpleGameMode* currentGameMode = static_cast<AProjectPurpleGameMode*>(GetWorld()->GetAuthGameMode());

	// first priority of AI discard is lowest cost legal items
	TArray<UCard*> discards;
	TArray<UCard*> legals = SortCards(GetLegalResources());
	TArray<UCard*> sortedHand = SortCards(GetLegalResources());
	int num = playerHand.Num() - handLimit;

	if (num > 0)
	{
		if (legals.Num() > num)
		{
			for (int i = legals.Num() - 1; i >= 0; i--)
			{
				discards.Add(legals[i]);
				DiscardHelper(legals[i]);

				if (discards.Num() == num)
				{
					return discards;
				}
			}
		}
		else
		{
			for (int i = sortedHand.Num() - 1; i >= 0; i--)
			{
				discards.Add(sortedHand[i]);
				DiscardHelper(sortedHand[i]);

				if (discards.Num() == num)
				{
					return discards;
				}
			}
		}
	}

	return discards;
}

bool APurpleAIController::WillingToContributeToPlayer(UCard* card)
{
	AProjectPurpleGameMode* currentGameMode = static_cast<AProjectPurpleGameMode*>(GetWorld()->GetAuthGameMode());

	for (int j = 0; j < currentGameMode->players.Num(); j++)
	{
		if (card->currentOwner == currentGameMode->players[j]->playerRole->name)
		{
			if (currentGameMode->players[j]->IsA(APurpleController::StaticClass()))
			{
				APurpleController* humanPlayer = static_cast<APurpleController*>(currentGameMode->players[j]);

				if (humanPlayer->GetPlayerStanding() >= 0)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
		}
	}

	return false;
}

// called when one card is going to another player or stranger(discard)
void APurpleAIController::ReturnItem(UCard* cardToReturn)
{
	AProjectPurpleGameMode* currentGameMode = static_cast<AProjectPurpleGameMode*>(GetWorld()->GetAuthGameMode());

	bool playerFound = false;

	if (cardToReturn->currentOwner != "Stranger")
	{
		for (int i = 0; i < currentGameMode->players.Num(); i++)
		{
			if (currentGameMode->players[i]->playerRole->name != this->playerRole->name)
			{
				if (cardToReturn->currentOwner == currentGameMode->players[i]->playerRole->name)
				{
					playerFound = true;
					playerHand.RemoveSingle(cardToReturn);
					playerFoundObjects.RemoveSingle(cardToReturn);
					currentGameMode->players[i]->playerHand.Add(cardToReturn);
				}
			}
		}

		if (playerFound != true)
		{
			playerHand.RemoveSingle(cardToReturn);
			playerFoundObjects.RemoveSingle(cardToReturn);
			currentGameMode->resourcesListDiscard.Add(cardToReturn);
		}
	}
	else
	{
		playerHand.RemoveSingle(cardToReturn);
		playerFoundObjects.RemoveSingle(cardToReturn);
		currentGameMode->resourcesListDiscard.Add(cardToReturn);
	}
}

// mostly used for return objects in festival
TArray<UCard*> APurpleAIController::ReturnItems()
{
	AProjectPurpleGameMode* currentGameMode = static_cast<AProjectPurpleGameMode*>(GetWorld()->GetAuthGameMode());
	TArray<UCard*> temp = playerFoundObjects;

	for (int i = playerFoundObjects.Num() - 1; i >= 0; i--)
	{
		ReturnItem(playerFoundObjects[i]);
	}

	return temp;
}

void APurpleAIController::ReturnLostItemToHand(UCard* card)
{
	playerLostObjects.RemoveSingle(card);
	playerHand.Add(card);
}

int32 APurpleAIController::FindLowestCommunal()
{
	AProjectPurpleGameMode* currentGameMode = static_cast<AProjectPurpleGameMode*>(GetWorld()->GetAuthGameMode());

	int min = currentGameMode->activeCommunalResponsibilities[0]->GetDoubleProperty("cost");
	int32 index = -1;

	for (int i = 0; i < currentGameMode->activeCommunalResponsibilities.Num(); i++)
	{
		if (currentGameMode->activeCommunalResponsibilities[i]->GetDoubleProperty("cost") <= min)
		{
			min = currentGameMode->activeCommunalResponsibilities[i]->GetDoubleProperty("cost");
			index = i;
		}
	}

	return index;
}

// will be used for heshbon help
UCard* APurpleAIController::TakeBackFamilyToHand()
{
	int index = FindLowestCostFamily();
	UCard* card = playerActiveFamilyResponsibilities[index];

	playerCompletedFamilyResponsibilities.RemoveSingle(card);

	card->SetDoubleProperty("cost", card->GetDoubleProperty("initial_cost"));

	playerActiveFamilyResponsibilities.Add(card);

	return card;
}

UCard* APurpleAIController::CompleteFamilyForFree()
{
	int index = FindHighestCostFamily();
	UCard* card = playerActiveFamilyResponsibilities[index];

	playerActiveFamilyResponsibilities.RemoveSingle(card);

	playerCompletedFamilyResponsibilities.Add(card);

	return card;
}

int32 APurpleAIController::TransgressionChoice(UCard* transgression, int32 modifier)
{
	// need to check if the player has enough to pay for transgression at higher cost
	// if not then the player will take back a family to hand
	TArray<UCard*> legals = GetLegalResources();
	int sum = 0;

	for (int i = 0; i < legals.Num(); i++)
	{
		sum += legals[i]->GetDoubleProperty("value");
	}

	if (teshuvahsCompleted == 0)
	{
		modifier += 1;
	}

	modifier += teshuvahsCompleted;

	if (sum >= (transgression->GetDoubleProperty("value") + modifier))
	{
		// goes to teshuvah function
		return 0;
	}
	else
	{
		// take back to hand
		return 1;
	}
}

int32 APurpleAIController::FindLowestCostFamily()
{
	int min = playerActiveFamilyResponsibilities[0]->GetDoubleProperty("cost");
	int32 index = -1;

	for (int i = 0; i < playerActiveFamilyResponsibilities.Num(); i++)
	{
		if (playerActiveFamilyResponsibilities[i]->GetDoubleProperty("cost") <= min)
		{
			min = playerActiveFamilyResponsibilities[i]->GetDoubleProperty("cost");
			index = i;
		}
	}

	return index;
}

int32 APurpleAIController::FindHighestCostFamily()
{
	int max = playerActiveFamilyResponsibilities[0]->GetDoubleProperty("cost");
	int32 index = -1;

	for (int i = 0; i < playerActiveFamilyResponsibilities.Num(); i++)
	{
		if (playerActiveFamilyResponsibilities[i]->GetDoubleProperty("cost") >= max)
		{
			max = playerActiveFamilyResponsibilities[i]->GetDoubleProperty("cost");
			index = i;
		}
	}

	return index;
}
//the following functions handle AI powers
bool APurpleAIController::UseShepherdPower()
{
	//is tracking most recently discarded card for shepherd to take
	AProjectPurpleGameMode* currentGameMode = static_cast<AProjectPurpleGameMode*>(GetWorld()->GetAuthGameMode());
	UCard* draw = currentGameMode->shepherdPowerCard;
	//AI will only discard >=5
	if (draw->GetDoubleProperty("value") >= 5)
	{
		if (draw->currentOwner != playerRole->name && draw->currentOwner != "Any")
		{
			playerFoundObjects.Add(draw);
		}

		playerHand.Add(draw);
		shepherdPower = true;
	}
	return shepherdPower;
}
//if card is Empty Vessel, Jar of Milk, Vessel of Wine, or Jar of Honey: value of card +1
UCard* APurpleAIController::UsePotterPower(UCard* contribution)
{
	//checking if role is Potter to apply Ability
	if (roleString == "POTTER")
	{
		//checking if legal owner
		if (contribution->currentOwner == playerRole->name || contribution->currentOwner == "Any")
		{
			//checking if card is kind to receive new value
			if (contribution->name == "EMPTY VESSEL" || contribution->name == "JAR OF MILK"
				|| contribution->name == "PITCHER OF WINE" || contribution->name == "JAR OF HONEY")
			{
				//adding +1 value to card
				int oldValue = contribution->GetDoubleProperty("value");
				contribution->SetDoubleProperty("value", oldValue + 1);
			}
			return contribution;
		}
		else
			return contribution;
	}
	else
		return contribution;
}
TArray<class UCard*> APurpleAIController::UseVintnerPower()
{
	//AI should find legally owned resources worth 2 or less, discard lowest value one
	//if tie, choose arbitrarily
	AProjectPurpleGameMode* currentGameMode = static_cast<AProjectPurpleGameMode*>(GetWorld()->GetAuthGameMode());
	TArray<UCard*>legals = GetLegalResources();
	TArray<UCard*>lowValueCards;
	TArray<UCard*> discard;//return value, should have size of one
	//finding lowest value cards
	for (int i = 0; i < legals.Num(); i++)
	{
		if (legals[i]->GetDoubleProperty("value") <= 2)
			lowValueCards.Push(legals[i]);
	}
	vintnerPower = true;
	//finding lowest cards
	if (lowValueCards.Num() > 1)//only loop if more than 1 low value card
	{
		int32 lowestValueIndex = 0;
		int32 lowestValue = 2;
		for (int i = 0; i < lowValueCards.Num(); i++)
		{
			if (lowValueCards[i]->GetDoubleProperty("value") <= lowestValue)
			{
				lowestValueIndex = i;
				lowestValue = lowValueCards[i]->GetDoubleProperty("value");
			}
		}
		discard.Push(lowValueCards[lowestValueIndex]);
		UCard* tempCard = discard[0];
		playerHand.RemoveSingle(discard[0]);
		currentGameMode->resourcesList.Insert(tempCard, 0);//should add card to bottom of resource deck
		return discard;
	}
	else if (lowValueCards.Num() <= 0)
		return discard;
	else
	{
		UCard* tempCard = lowValueCards[0];
		playerHand.RemoveSingle(lowValueCards[0]);
		currentGameMode->resourcesList.Insert(tempCard,0);//should add card to bottom of resource deck
		return lowValueCards;
	}
}
;
