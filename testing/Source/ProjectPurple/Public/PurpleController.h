// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Card.h"
#include "SuperController.h"
#include "PurpleController.generated.h"

UCLASS(Config = Game, BlueprintType, Blueprintable, Meta = (ShortTooltip = "A Player Controller is an actor responsible for controlling a Pawn used by the player."))
class PROJECTPURPLE_API APurpleController : public ASuperController
{
	GENERATED_BODY()

public:
	// variable to track how the human player is communal or rogue, + is communal, - is rogue
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		int32 playerStanding;

	// Sets default values for this actor's properties
	virtual void initController() override;

	//// Drawing and game functions -------------------------------------------------
	// draws number of resources equivalent to the property numToDraw which may be affted by responsibilities
	virtual TArray<UCard*> DrawResources() override;

	// draws a single lost resource, consequence of events
	virtual UCard* DrawLostResource() override;

	// draws a single resource, used for events mostly
	virtual UCard* DrawResource() override;

	virtual UCard* DrawResourceDiscard() override;

	// Draw event card
	virtual void DrawEvent() override;

	// Draw event card
	virtual void DiscardHelper(UCard* card) override;

	// Choose role for player
	virtual void SetRole() override;
    
    // Special function for role selecting a specific character
    UFUNCTION(BlueprintCallable, Category = "PlayerEvents")
        void SetRoleSpecial(); 

	virtual TArray<class UCard*> GetLegalResources() override;


	// different for player than AI ------------------------------------------------------------------------------------------
	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		bool ContributeToFamilyResponsibility(TArray<ACardActor*> contributions, UCard* Family);

	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		bool ContributeToFamilyResponsibilityEndGame(TArray<ACardActor*> contributions, UCard* Family, int32 costModifier);

	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		void ContributeToCommunalResponsibility(TArray<ACardActor*> contributions, UCard* Communal);

	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		void ContributeToDisaster(TArray<ACardActor*> contributions, UCard* Disaster);

	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		void ContributeToCrisis(TArray<ACardActor*> contributions, UCard* Crisis);

	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		void ModifyTransgressionCost(UCard* card, int32 costModifier);

	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		bool Teshuvah(TArray<ACardActor*> contributions, UCard* Transgression);

	UFUNCTION(BlueprintCallable, Category = "PlayerEvents")
		void Discard(TArray<ACardActor*> discards);

	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		void ReturnItem(ACardActor* item);

	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		void ReturnLostItemToHand(ACardActor* item);

	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		void ReturnItems(TArray<ACardActor*> items);

	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		void TakeBackFamilyToHand(ACardActor * Family);

	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		void CompleteFamilyForFree(ACardActor * Family);

	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		int32 GetPlayerStanding();

	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		void AddToPlayerStanding(int32 value);

	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		void SubtractFromPlayerStanding(int32 value);

	//functions for player powers
	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		void UseVintnerPower(ACardActor* card);
	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		ACardActor* UsePotterPower(ACardActor* contribution);
	/*DEPREACTED
	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		void UseCowherdPower();*/
	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		void UseShepherdPower();
};
