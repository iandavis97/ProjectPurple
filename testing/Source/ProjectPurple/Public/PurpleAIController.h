// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "Card.h"
#include "SuperController.h"
#include "PurpleAIController.generated.h"

/**
 *
 */
UCLASS()
class PROJECTPURPLE_API APurpleAIController : public ASuperController
{
	GENERATED_BODY()

public:
    
    //Constructor for initializing game values
    APurpleAIController();

	//// Drawing and game functions -------------------------------------------------
	// draws number of resources equivalent to the property numToDraw which may be affted by responsibilities
	UFUNCTION(BlueprintCallable, Category = "AIPlayerEvents")
		virtual TArray<UCard*> DrawResources() override;

	// draws a single lost resource, consequence of events
	UFUNCTION(BlueprintCallable, Category = "AIPlayerEvents")
		virtual UCard* DrawLostResource() override;

	// draws a single resource, used for events mostly
	UFUNCTION(BlueprintCallable, Category = "AIPlayerEvents")
		virtual UCard* DrawResource() override;

	UFUNCTION(BlueprintCallable, Category = "PlayerEvents")
		virtual UCard* DrawResourceDiscard() override;

	// Draw event card
	UFUNCTION(BlueprintCallable, Category = "AIPlayerEvents")
		virtual void DrawEvent() override;

	// Draw event card
	UFUNCTION(BlueprintCallable, Category = "AIPlayerEvents")
		virtual void DiscardHelper(UCard* card) override;

	// Choose role for player
	UFUNCTION(BlueprintCallable, Category = "AIPlayerEvents")
		virtual void SetRole() override;

	UFUNCTION(BlueprintCallable, Category = "AIPlayerEvents")
		virtual TArray<class UCard*> GetLegalResources() override;


	// different for AI from player
	UFUNCTION(BlueprintCallable, Category = "AIGameFunctions")
		TArray<UCard*> ContributeToDisaster();

	UFUNCTION(BlueprintCallable, Category = "AIGameFunctions")
		TArray<UCard*> ContributeToCrisis();

	UFUNCTION(BlueprintCallable, Category = "AIGameFunctions")
		UCard* Teshuvah(int32 costModifier);


	// done
	UFUNCTION(BlueprintCallable, Category = "AIGameFunctions")
		TArray<class UCard*> Discard();

	UFUNCTION(BlueprintCallable, Category = "AIGameFunctions")
		UCard* TakeBackFamilyToHand();

	UFUNCTION(BlueprintCallable, Category = "AIGameFunctions")
		UCard* CompleteFamilyForFree();

	UFUNCTION(BlueprintCallable, Category = "AIGameFunctions")
		int32 TransgressionChoice(UCard* transgression, int32 modifier);

	UFUNCTION(BlueprintCallable, Category = "AIGameFunctions")
		int32 FindLowestCostFamily();

	UFUNCTION(BlueprintCallable, Category = "AIGameFunctions")
		int32 FindHighestCostFamily();

	UFUNCTION(BlueprintCallable, Category = "AIGameFunctions")
		bool WillingToContributeToPlayer(UCard* card);

	UFUNCTION(BlueprintCallable, Category = "AIGameFunctions")
		void ReturnItem(UCard* cardToReturn);

	UFUNCTION(BlueprintCallable, Category = "AIGameFunctions")
		bool ShouldContributeToFamily();

	UFUNCTION(BlueprintCallable, Category = "AIGameFunctions")
		bool ShouldContributeToFamilyEndGame(int32 costModifier);

	UFUNCTION(BlueprintCallable, Category = "AIGameFunctions")
		bool ShouldContributeToCommunal();

	UFUNCTION(BlueprintCallable, Category = "AIGameFunctions")
		TArray<UCard*> ContributeToFamilyResponsibility(int32 costModifier);

	UFUNCTION(BlueprintCallable, Category = "AIGameFunctions")
		TArray<class UCard*> ContributeToCommunalResponsibility();

	UFUNCTION(BlueprintCallable, Category = "AIGameFunctions")
		TArray<UCard*> ReturnItems();

	UFUNCTION(BlueprintCallable, Category = "AIGameFunctions")
		void ReturnLostItemToHand(UCard* card);

	UFUNCTION(BlueprintCallable, Category = "AIGameFunctions")
		int32 FindLowestCommunal();
    
    UFUNCTION(BlueprintCallable, Category = "AIGameFunctions")
        TArray<class UCard*> SortCards(TArray<class UCard*> cards);
};
