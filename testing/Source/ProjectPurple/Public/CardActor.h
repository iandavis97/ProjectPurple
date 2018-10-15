// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Card.h"
#include "CardActor.generated.h"

UCLASS()
class PROJECTPURPLE_API ACardActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACardActor();
    
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true) , Category = "Card Actor Properties")
    UCard* CardData;
	
};
