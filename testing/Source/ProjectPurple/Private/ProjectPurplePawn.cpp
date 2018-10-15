// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectPurple.h"
#include "ProjectPurplePawn.h"


// Sets default values
AProjectPurplePawn::AProjectPurplePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AProjectPurplePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectPurplePawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void AProjectPurplePawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

