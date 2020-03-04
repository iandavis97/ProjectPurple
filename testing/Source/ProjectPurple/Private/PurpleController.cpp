// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectPurple.h"
#include "PurpleController.h"
#include "Math.h"
#include "CardActor.h"
#include "ProjectPurpleGameMode.h"


// Sets default values
void APurpleController::initController()
{
	responsibilitiesAbleToContribute = 1;
	numToDraw = 2;
	handLimit = 3;
	gender = "Male";
	marriage = false;
	swim = false;
	trade = false;
}

// Is called when the player wants to draw resources- This also adds the cards to the hand of the player, variable based on responsibilities, which change numtodraw
TArray<UCard*> APurpleController::DrawResources()
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
UCard* APurpleController::DrawLostResource()
{
	AProjectPurpleGameMode* currentGameMode = static_cast<AProjectPurpleGameMode*>(GetWorld()->GetAuthGameMode());

	UCard* draw = currentGameMode->lostResources.Pop(true);

	playerHand.Add(draw);

	playerFoundObjects.Add(draw);

	return draw;
}

// draws a single resource, for events mostly
UCard* APurpleController::DrawResource()
{
	AProjectPurpleGameMode* currentGameMode = static_cast<AProjectPurpleGameMode*>(GetWorld()->GetAuthGameMode());

	if (currentGameMode->resourcesList.Num() <= 0)
	{
		currentGameMode->ShuffleCards(currentGameMode->resourcesListDiscard);
		currentGameMode->resourcesList.Append(currentGameMode->resourcesListDiscard);
	}

	UCard* draw = currentGameMode->resourcesList.Pop(true);

	if (draw->currentOwner != playerRole->name && draw->currentOwner != "Any")
	{
		playerFoundObjects.Add(draw);
	}


	if (trade == true && draw->currentOwner == playerRole->name)
	{
		draw->SetDoubleProperty("value", (draw->GetDoubleProperty("value") + 1));
	}

	playerHand.Add(draw);

	return draw;
}

UCard* APurpleController::DrawResourceDiscard()
{
	AProjectPurpleGameMode* currentGameMode = static_cast<AProjectPurpleGameMode*>(GetWorld()->GetAuthGameMode());

	int range = currentGameMode->resourcesListDiscard.Num() - 0 + 1;
	int num = rand() % range + 0;

	UCard* randDraw = currentGameMode->resourcesListDiscard[num];
	currentGameMode->resourcesListDiscard.RemoveAt(num);

	if (randDraw->currentOwner != playerRole->name && randDraw->currentOwner != "Any")
	{
		playerFoundObjects.Add(randDraw);
	}

	if (trade == true && randDraw->currentOwner == playerRole->name)
	{
		randDraw->SetDoubleProperty("value", (randDraw->GetDoubleProperty("value") + 1));
	}

	playerHand.Add(randDraw);

	return randDraw;
}

// Is called when we want to draw events. This only draws the cards, it does not execute the resolution of the events.
void APurpleController::DrawEvent()
{
	AProjectPurpleGameMode* currentGameMode = static_cast<AProjectPurpleGameMode*>(GetWorld()->GetAuthGameMode());
	UCard* draw = currentGameMode->eventsList.Pop(true);
	currentGameMode->currentEvent = draw;
	
	if (currentGameMode->currentEvent->type == "Community - Disaster" && swim)
	{
		currentGameMode->currentEvent->SetDoubleProperty("col_cost", (currentGameMode->currentEvent->GetDoubleProperty("col_cost") - 4));
	}
}

void APurpleController::DiscardHelper(UCard * card)
{
	AProjectPurpleGameMode* currentGameMode = static_cast<AProjectPurpleGameMode*>(GetWorld()->GetAuthGameMode());

	playerHand.RemoveSingle(card);
	playerFoundObjects.RemoveSingle(card);

	// check if player is legal owner, if not add a transgression
	if ((card->currentOwner != "Any" && card->currentOwner != playerRole->name && card->type != "Individual - Found") || card->currentOwner == "Stranger")
	{
		if (this->IsA(APurpleController::StaticClass()))
		{
			Transgressions.Add(card);
		}

		SubtractFromPlayerStanding(1);
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
			card->currentOwner = card->GetStringProperty("owner");
			card->SetDoubleProperty("value", (card->GetDoubleProperty("initial_value")));
			currentGameMode->resourcesListDiscard.Add(card);
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
			card->currentOwner = card->GetStringProperty("owner");
			card->SetDoubleProperty("value", (card->GetDoubleProperty("initial_value")));
			currentGameMode->lostResourcesDiscard.Add(card);
		}
	}
}

// this event is called when a player needs to discard at the end of a turn during the aforementioned phase
void APurpleController::Discard(TArray<ACardActor*> discards)
{
	AProjectPurpleGameMode* currentGameMode = static_cast<AProjectPurpleGameMode*>(GetWorld()->GetAuthGameMode());

	for (int i = discards.Num() - 1; i >= 0; i--)
	{
		DiscardHelper(discards[i]->CardData);
	}
}

// Is called to set the player roles
void APurpleController::SetRole()
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
		roleString = FString(playerRole->name);
		currentGameMode->femaleRolesList.RemoveAt(ind);
		currentGameMode->maleRolesList.RemoveAt(ind);
	}
}

// Is called to set the player's role specifically
void APurpleController::SetRoleSpecial()
{
    int32 ind;

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
    
    for(ind = 0; ind < currentGameMode->femaleRolesList.Num(); ++ind)
    {
//        if(currentGameMode->femaleRolesList[ind]->name == "DATE FARMER")
//        {
//            break;
//        }
        if(currentGameMode->femaleRolesList[ind]->name == "COWHERD")
        {
            break;
        }
    }
    
    playerRole = currentGameMode->femaleRolesList[ind];
    roleString = FString(playerRole->name);
    currentGameMode->femaleRolesList.RemoveAt(ind);
    currentGameMode->maleRolesList.RemoveAt(ind);
    
}



// used for making Teshuvah
bool APurpleController::Teshuvah(TArray<ACardActor*> contributions, UCard * Transgression)
{
	AProjectPurpleGameMode* currentGameMode = static_cast<AProjectPurpleGameMode*>(GetWorld()->GetAuthGameMode());

	// Sum of Contributions
	int total = 0;

	for (int i = 0; i < contributions.Num(); i++)
	{
		total += contributions[i]->CardData->GetDoubleProperty("value");
	}

	if (total >= Transgression->GetDoubleProperty("value"))
	{
		Discard(contributions);

		Transgressions.RemoveSingle(Transgression);

		if (Transgression->type == "Individual - Found" || Transgression->type == "Individual - Lost")
		{
			currentGameMode->eventsListDiscard.Add(Transgression);
		}

		else if (Transgression->type == "Resource")
		{
			Transgression->currentOwner = Transgression->GetStringProperty("owner");
			Transgression->SetDoubleProperty("value", (Transgression->GetDoubleProperty("initial_value")));
			currentGameMode->resourcesListDiscard.Add(Transgression);
		}

		else if (Transgression->type == "Starting Resource")
		{
			Transgression->currentOwner = (Transgression->GetStringProperty("owner"));
			Transgression->SetDoubleProperty("value", (Transgression->GetDoubleProperty("initial_value")));
		}

		else if (Transgression->type == "Lost Resource")
		{
			Transgression->currentOwner = Transgression->GetStringProperty("owner");
			Transgression->SetDoubleProperty("value", (Transgression->GetDoubleProperty("initial_value")));
			currentGameMode->lostResourcesDiscard.Add(Transgression);
		}

		teshuvahsCompleted++;
		return true;
	}
	else
	{
		// log message for failure of payment
		// debug purposes
		UE_LOG(LogTemp, Warning, TEXT("Not enough dinarim to pay for Teshuvah"));
		return false;
	}
}

// will be used for heshbon help
void APurpleController::TakeBackFamilyToHand(ACardActor * Family)
{
	playerCompletedFamilyResponsibilities.RemoveSingle(Family->CardData);

	Family->CardData->SetDoubleProperty("cost", Family->CardData->GetDoubleProperty("initial_cost"));

	playerActiveFamilyResponsibilities.Add(Family->CardData);
}

void APurpleController::CompleteFamilyForFree(ACardActor * Family)
{
	playerActiveFamilyResponsibilities.RemoveSingle(Family->CardData);

	playerCompletedFamilyResponsibilities.Add(Family->CardData);
}

int32 APurpleController::GetPlayerStanding()
{
	return playerStanding;
}

void APurpleController::AddToPlayerStanding(int32 value)
{
	playerStanding += value;
}

void APurpleController::SubtractFromPlayerStanding(int32 value)
{
	playerStanding -= value;
}

// called when player needs to contribute to family
bool APurpleController::ContributeToFamilyResponsibility(TArray<ACardActor*> contributions, UCard* Family)
{
	AProjectPurpleGameMode* currentGameMode = static_cast<AProjectPurpleGameMode*>(GetWorld()->GetAuthGameMode());

	// Sum of Contributions
	int total = 0;

	for (int i = 0; i < contributions.Num(); i++)
	{
		// check if player is legal owner of each card chosen
		if (contributions[i]->CardData->currentOwner == "Any" || contributions[i]->CardData->currentOwner == currentGameMode->activePlayer->playerRole->name
			|| contributions[i]->CardData->type == "Individual - Found")
		{
			total += contributions[i]->CardData->GetDoubleProperty("value");
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Transgression for player."));
			total += contributions[i]->CardData->GetDoubleProperty("value");
		}
	}

	// paid for in total
	if (total >= Family->GetDoubleProperty("cost"))
	{
		float newValue = Family->GetDoubleProperty("cost");
		newValue -= total;

		if (newValue <= 0)
		{
			newValue = 0;
		}

		Family->SetDoubleProperty("cost", newValue);

		if (Family->name == "Teach The Law")
		{
			responsibilitiesAbleToContribute = 2;
		}

		else if (Family->name == "Redemption of Firstborn")
		{
			handLimit += 1;
		}

		else if (Family->name == "Celebrate Marriage")
		{
			marriage = true;
		}

		else if (Family->name == "Teach to Swim")
		{
			swim = true;
		}

		else if (Family->name == "Teach a Trade")
		{
			trade = true;
		}

		Discard(contributions);

		playerActiveFamilyResponsibilities.RemoveSingle(Family);
		playerCompletedFamilyResponsibilities.Add(Family);

		if (playerFamilyResponsibilities.Num() > 0)
		{
			UCard* newFam = playerFamilyResponsibilities.Pop();

			currentGameMode->AdjustResponsibilityOnDraw(newFam);

			playerActiveFamilyResponsibilities.Add(newFam);
		}
		SubtractFromPlayerStanding(1);
		return true;
	}
	else
	{
		// log message for failure of payment
		// debug purposes
		//Family->SetDoubleProperty("cost", (Family->GetDoubleProperty("cost") - total));
		UE_LOG(LogTemp, Warning, TEXT("Not enough dinarim to pay for family"));

		return false;
	}
}

// called when player needs to contribute to family
bool APurpleController::ContributeToFamilyResponsibilityEndGame(TArray<ACardActor*> contributions, UCard* Family, int32 costModifier)
{
	AProjectPurpleGameMode* currentGameMode = static_cast<AProjectPurpleGameMode*>(GetWorld()->GetAuthGameMode());


	// Sum of Contributions
	int total = 0;

	for (int i = 0; i < contributions.Num(); i++)
	{
		// check if player is legal owner of each card chosen
		if (contributions[i]->CardData->currentOwner == "Any" || contributions[i]->CardData->currentOwner == currentGameMode->activePlayer->playerRole->name
			|| contributions[i]->CardData->type == "Individual - Found")
		{
			contributions[i] = UsePotterPower(contributions[i]);//if potter, certain cards will have new value, otherwise no change
			total += contributions[i]->CardData->GetDoubleProperty("value");
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Transgression for player."));
			contributions[i] = UsePotterPower(contributions[i]);//if potter, certain cards will have new value, otherwise no change
			total += contributions[i]->CardData->GetDoubleProperty("value");
		}
	}

	// paid for in total
	if (total >= Family->GetDoubleProperty("cost") - costModifier)
	{
		float newValue = Family->GetDoubleProperty("cost");
		newValue -= total;

		if (newValue <= 0)
		{
			newValue = 0;
		}

		Family->SetDoubleProperty("cost", newValue);

		if (Family->name == "Teach The Law")
		{
			responsibilitiesAbleToContribute = 2;
		}

		else if (Family->name == "Redemption of Firstborn")
		{
			handLimit += 1;
		}

		else if (Family->name == "Celebrate Marriage")
		{
			marriage = true;
		}

		else if (Family->name == "Teach to Swim")
		{
			swim = true;
		}

		else if (Family->name == "Teach a Trade")
		{
			trade = true;
		}

		Discard(contributions);

		playerActiveFamilyResponsibilities.RemoveSingle(Family);
		playerCompletedFamilyResponsibilities.Add(Family);

		if (playerFamilyResponsibilities.Num() > 0)
		{
			UCard* newFam = playerFamilyResponsibilities.Pop();

			currentGameMode->AdjustResponsibilityOnDraw(newFam);

			playerActiveFamilyResponsibilities.Add(newFam);
		}

		return true;
	}
	else
	{
		// log message for failure of payment
		// debug purposes
		//Family->SetDoubleProperty("cost", (Family->GetDoubleProperty("cost") - total));
		UE_LOG(LogTemp, Warning, TEXT("Not enough dinarim to pay for family"));

		return false;
	}
}

// called when a player needs to contribute to communal
void APurpleController::ContributeToCommunalResponsibility(TArray<ACardActor*> contributions, UCard * Communal)
{
	AProjectPurpleGameMode* currentGameMode = static_cast<AProjectPurpleGameMode*>(GetWorld()->GetAuthGameMode());


	int total = 0;

	for (int i = 0; i < contributions.Num(); i++)
	{
		contributions[i] = UsePotterPower(contributions[i]);//if potter, certain cards will have new value, otherwise no change
		total += contributions[i]->CardData->GetDoubleProperty("value");
	}

	float newValue = Communal->GetDoubleProperty("cost");
	newValue -= total;

	if (newValue <= 0)
	{
		newValue = 0;
	}

	Communal->SetDoubleProperty("cost", newValue);

	// paid for
	if (Communal->GetDoubleProperty("cost") <= 0)
	{
		// adds the communal to the completed list to track progress, and removes the communal from the active list showing on screen
		currentGameMode->completedCommunalResponsibilities.Add(Communal);
		currentGameMode->activeCommunalResponsibilities.RemoveSingle(Communal);

		for (int i = 0; i < currentGameMode->activeCommunalResponsibilities.Num(); i++)
		{
			currentGameMode->AdjustResponsibilityValue(currentGameMode->activeCommunalResponsibilities[i]);
		}

		if (currentGameMode->communalResponsibilities.Num() > 0)
		{
			UCard* newCom = currentGameMode->communalResponsibilities.Pop();

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

		Discard(contributions);

		AddToPlayerStanding(2);
	}
	else
	{
		Discard(contributions);
		AddToPlayerStanding(1);
		UE_LOG(LogTemp, Warning, TEXT("Cost = %d"), Communal->GetDoubleProperty("cost"));
	}
}

// called when player needs to contribute to a disaster
void APurpleController::ContributeToDisaster(TArray<ACardActor*> contributions, UCard * Disaster)
{
	AProjectPurpleGameMode* currentGameMode = static_cast<AProjectPurpleGameMode*>(GetWorld()->GetAuthGameMode());

	int total = 0;
	int bonus = 0;

	for (int i = 0; i < contributions.Num(); i++)
	{
		// need to check if player paying is the active player, then modulo based on specific disasters number and check if it is equal to 0.
		// if it is, then divide total by number and gain bonus

		contributions[i] = UsePotterPower(contributions[i]);//if potter, certain cards will have new value, otherwise no change

		// check if player is legal owner of each card chosen
		total += contributions[i]->CardData->GetDoubleProperty("value");
	}

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
	float newValue = Disaster->GetDoubleProperty("col_cost");
	newValue -= total;
	newValue -= bonus;

	if (newValue <= 0)
	{
		newValue = 0;
	}

	Disaster->SetDoubleProperty("col_cost", newValue);

	// paid for
	if (Disaster->GetDoubleProperty("col_cost") <= 0)
	{

		Discard(contributions);

		AddToPlayerStanding(1);
	}
	else
	{

		Discard(contributions);

		AddToPlayerStanding(1);
		UE_LOG(LogTemp, Warning, TEXT("Cost = %d"), Disaster->GetDoubleProperty("col_cost"));
	}
}

// called when a player needs to contribute to crisis
void APurpleController::ContributeToCrisis(TArray<ACardActor*> contributions, UCard * Crisis)
{
	AProjectPurpleGameMode* currentGameMode = static_cast<AProjectPurpleGameMode*>(GetWorld()->GetAuthGameMode());

	int total = 0;

	for (int i = 0; i < contributions.Num(); i++)
	{
		// check if player is legal owner of each card chosen

		contributions[i] = UsePotterPower(contributions[i]);//if potter, certain cards will have new value, otherwise no change
		
		total += contributions[i]->CardData->GetDoubleProperty("value");
	}

    float newValue = Crisis->GetDoubleProperty("col_cost");
    newValue -= total;
    if(newValue <= 0)
    {
        newValue = 0;
    }
	Crisis->SetDoubleProperty("col_cost", newValue);

	// paid for
	if (Crisis->GetDoubleProperty("col_cost") <= 0)
	{

		Discard(contributions);

		currentGameMode->activeCrises.RemoveSingle(Crisis);

		AddToPlayerStanding(1);
	}
	else
	{
		// sets the cost property to the difference of the original cost and total contributed by player this turn

		Discard(contributions);

		AddToPlayerStanding(1);
		UE_LOG(LogTemp, Warning, TEXT("Cost = %d"), Crisis->GetDoubleProperty("col_cost"));
	}
}

void APurpleController::ModifyTransgressionCost(UCard * card, int32 costModifier)
{
	if (teshuvahsCompleted == 0)
	{
		costModifier += 1;
	}

	costModifier += teshuvahsCompleted;

	card->SetDoubleProperty("value", card->GetDoubleProperty("value") + costModifier);
}

// called when one card is going to another player or stranger(discard)
void APurpleController::ReturnItem(ACardActor * item)
{
	AProjectPurpleGameMode* currentGameMode = static_cast<AProjectPurpleGameMode*>(GetWorld()->GetAuthGameMode());

	bool playerFound = false;

	if (item->CardData->currentOwner != "Stranger")
	{
		for (int i = 0; i < currentGameMode->players.Num(); i++)
		{
			if (currentGameMode->players[i]->playerRole->name != this->playerRole->name)
			{
				if (item->CardData->currentOwner == currentGameMode->players[i]->playerRole->name)
				{
					playerFound = true;
					playerHand.RemoveSingle(item->CardData);
					playerFoundObjects.RemoveSingle(item->CardData);
					currentGameMode->players[i]->playerHand.Add(item->CardData);
					AddToPlayerStanding(1);
					break;
				}
			}
		}

		if (playerFound != true)
		{
			playerHand.RemoveSingle(item->CardData);
			playerFoundObjects.RemoveSingle(item->CardData);
			currentGameMode->resourcesListDiscard.Add(item->CardData);
		}
	}
	else
	{
		playerHand.RemoveSingle(item->CardData);
		playerFoundObjects.RemoveSingle(item->CardData);
		currentGameMode->resourcesListDiscard.Add(item->CardData);
	}
}

void APurpleController::ReturnLostItemToHand(ACardActor * item)
{
	playerLostObjects.RemoveSingle(item->CardData);
	playerHand.Add(item->CardData);
}

// mostly used for return objects in festival
void APurpleController::ReturnItems(TArray<ACardActor*> items)
{
	AProjectPurpleGameMode* currentGameMode = static_cast<AProjectPurpleGameMode*>(GetWorld()->GetAuthGameMode());

	for (int i = items.Num() - 1; i >= 0; i--)
	{
		ReturnItem(items[i]);
	}
}

// called when players owned resources are needed, returns a ucard array back
TArray<class UCard*> APurpleController::GetLegalResources()
{
	TArray<class UCard*> legals;

	// loops through hand checking if resource is legal or not
	for (int i = 0; i < playerHand.Num(); i++)
	{
		if (playerHand[i]->currentOwner == "Any" || playerHand[i]->currentOwner == playerRole->name || playerHand[i]->type == "Individual - Found")
		{
			legals.Add(playerHand[i]);
		}
	}
	return legals;
}


//chosen card put at bottom of resource deck
//player draws from resource deck
void APurpleController::UseVintnerPower(ACardActor* card)
{
	AProjectPurpleGameMode* currentGameMode = static_cast<AProjectPurpleGameMode*>(GetWorld()->GetAuthGameMode());
	UCard* tempCard=card->CardData;
	playerHand.RemoveSingle(card->CardData);
	currentGameMode->resourcesList.Push(tempCard);//should add card to bottom of resource deck
	DrawResource();
	vintnerPower = true;
}

//if card is Empty Vessel, Jar of Milk, Vessel of Wine, or Jar of Honey: value of card +1
ACardActor* APurpleController::UsePotterPower(ACardActor* contribution)
{
	//checking if role is Potter to apply Ability
	if (roleString == "POTTER")
	{
		//checking if card is kind to receive new value
		if (contribution->CardData->name == "EMPTY VESSEL" || contribution->CardData->name == "JAR OF MILK"
			|| contribution->CardData->name == "VESSEL OF WINE" || contribution->CardData->name == "JAR OF HONEY")
		{
			//adding +1 value to card
			int oldValue = contribution->CardData->GetDoubleProperty("value");
			contribution->CardData->SetDoubleProperty("value",oldValue+1);
		}
		return contribution;
	}
	else
		return contribution;
}

//draws 2 extra Resource cards
void APurpleController::UseCowherdPower()
{
	for (int i = 0; i < 2; i++)
	{
		DrawResource();
	}
	
}
;
