// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BoxButton.generated.h"

UCLASS()
class PROJECTPURPLE_API ABoxButton : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoxButton();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Box Actor Properties")
    bool isTouched;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (DisplayName = "Text", ExposeOnSpawn = true) , Category = "Box Actor Properties")
    FString text;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (DisplayName = "Index", ExposeOnSpawn = true) , Category = "Box Actor Properties")
    int32 index;
};
