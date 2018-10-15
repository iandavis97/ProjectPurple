// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Controller.h"
#include "SuperController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTPURPLE_API ASuperController : public AController
{
	GENERATED_BODY()
	

public:
	// Sets default values for this actor's properties
	virtual void initController() {}

	// amount to draw, changed with responsibilities
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		int32 numToDraw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		bool swim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		bool marriage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		bool trade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		int32 handLimit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		int32 responsibilitiesAbleToContribute;

	// Players role
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	class UCard* playerRole;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		FString roleString;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		FString gender;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		int32 teshuvahsCompleted;


	// Player card containers ------------------------------------------------------
	// Hand for the player, containing card objects
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		TArray<class UCard*> playerHand;

	// Players' lost items
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		TArray<class UCard*> playerLostObjects;

	// Players' found items
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		TArray<class UCard*> playerFoundObjects;

	// Players' event cards for use in game
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		TArray<class UCard*> playerOwnedEvents;

	// Family resp, containing card objects
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		TArray<class UCard*> playerFamilyResponsibilities;

	// Family resp that can be contributed to, containing card objects
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		TArray<class UCard*> playerActiveFamilyResponsibilities;

	// Family resp, containing card objects
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		TArray<class UCard*> playerCompletedFamilyResponsibilities;

	// Transgression cards / number
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		TArray<class UCard*> Transgressions;

	// Beyond the law cards / number
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		TArray<class UCard*> BeyondTheLaws;

	// Array of Hand cards displayed on screen
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameModeProperties")
		TArray<class ACardActor*> displayedHand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameModeProperties")
		TArray<class ACardActor*> displayedFoundObjects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameModeProperties")
		TArray<class ACardActor*> displayedFamilyResponsibilities;
	

	// functions
	UFUNCTION(BlueprintCallable, Category = "PlayerEvents")
		virtual TArray<UCard*> DrawResources() 
	{ 
		TArray<UCard*> returnArray = {};
		return returnArray;
	}

	// draws a single lost resource, consequence of events
	UFUNCTION(BlueprintCallable, Category = "PlayerEvents")
		virtual UCard* DrawLostResource() { return nullptr; }

	// draws a single resource, used for events mostly
	UFUNCTION(BlueprintCallable, Category = "PlayerEvents")
		virtual UCard* DrawResource() { return nullptr; }

	UFUNCTION(BlueprintCallable, Category = "PlayerEvents")
		virtual UCard* DrawResourceDiscard() { return nullptr; }

	// Draw event card
	UFUNCTION(BlueprintCallable, Category = "PlayerEvents")
		virtual void DrawEvent() {}

	// Draw event card
	UFUNCTION(BlueprintCallable, Category = "PlayerEvents")
		virtual void DiscardHelper(UCard* card) {}

	// Choose role for player
	UFUNCTION(BlueprintCallable, Category = "PlayerEvents")
		virtual void SetRole() {}

	UFUNCTION(BlueprintCallable, Category = "GameHelpers")
		virtual TArray<class UCard*> GetLegalResources()
	{
		TArray<UCard*> returnArray = {};
		return returnArray;
	}
};
