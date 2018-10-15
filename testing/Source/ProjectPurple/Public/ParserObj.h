// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "ProjectPurpleGameMode.h"
#include <Json.h>
#include <JsonObject.h>
#include <JsonReader.h>
#include <JsonSerializer.h>
#include <UnrealString.h>
#include "ParserObj.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class PROJECTPURPLE_API UParserObj : public UObject
{
	GENERATED_BODY()

public:

	// Sets default values for this actor's properties
	
	UParserObj();
	// Called when the game starts or when spawned
	//	virtual void BeginPlay() override;
	// Called every frame

	UFUNCTION(BlueprintCallable, Category = "CardArray")
		void LoadCards();

	// Card Object
	class UCard* ParseCard(class FJsonObject*);

	// Array of resource cards
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardArray")
		TArray<class UCard*> resources;

	// Contains all the resources that are lost in the game
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardArray")
		TArray<class UCard*> lostResources;

	// Starting resources array 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardArray")
		TArray<class UCard*> startingResources;

	// Contains all the event cards in this array
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardArray")
		TArray<class UCard*> events;

	// Contains all the event cards in this array
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardArray")
		TArray<class UCard*> roles;
	
	// Commmunal responsibilities deck
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardArray")
		TArray<class UCard*> communalResponsibilities;

	// family responsibilties deck
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardArray")
		TArray<class UCard*> familyResponsibilities1;

	// family responsibilties deck
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardArray")
		TArray<class UCard*> familyResponsibilities2;

	// family responsibilties deck
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardArray")
		TArray<class UCard*> familyResponsibilities3;

	// family responsibilties deck
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardArray")
		TArray<class UCard*> familyResponsibilities4;

	// family responsibilties deck
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardArray")
		TArray<class UCard*> familyResponsibilities5;
	
	
	
};
