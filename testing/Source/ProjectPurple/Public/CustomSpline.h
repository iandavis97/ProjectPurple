// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/SplineComponent.h"
#include "CustomSpline.generated.h"

UCLASS() // ********************** This class is now deprecated, no need for any of these functions at the moment
class PROJECTPURPLE_API ACustomSpline : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACustomSpline();

//	// Called when the game starts or when spawned
//	virtual void BeginPlay() override;
//	
//	// Called every frame
//	virtual void Tick( float DeltaSeconds ) override;
    
    //UFUNCTION(Category = "Spline Functions")
    //void init(TArray<FVector> points);
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Instanced, Category = "Spline Properties")
    USplineComponent* PathSpline;
    
    // Should set starting points and defines additonal points for the defined spline
    UFUNCTION(BlueprintCallable, Category = "Spline Functions")
    void createSplineWithPoints(const FVector& pointA, const FVector& pointB, const TArray<FVector>& additionalPoints);
    
    // Sets starting points of splines
    UFUNCTION(BlueprintCallable, Category = "Spline Functions")
    void createSpline(const FVector& pointA, const FVector& pointB);
};
