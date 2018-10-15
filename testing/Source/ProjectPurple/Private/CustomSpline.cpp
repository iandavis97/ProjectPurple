// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectPurple.h"
#include "CustomSpline.h"


// Sets default values
ACustomSpline::ACustomSpline()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    PathSpline = CreateDefaultSubobject<USplineComponent>(FName("Path Spline"));
}

void ACustomSpline::createSplineWithPoints(const FVector& pointA, const FVector& pointB, const TArray<FVector>& additionalPoints)
{
    PathSpline->SetLocationAtSplinePoint(0, pointA, ESplineCoordinateSpace::Local);
    PathSpline->SetLocationAtSplinePoint(1, pointB, ESplineCoordinateSpace::Local);
    for (int i = 0; i < additionalPoints.Num(); i++)
    {
        PathSpline->AddSplinePoint(additionalPoints[i], ESplineCoordinateSpace::Local);
    }
}

void ACustomSpline::createSpline(const FVector& pointA, const FVector& pointB)
{
    PathSpline->SetLocationAtSplinePoint(0, pointA, ESplineCoordinateSpace::Local);
    PathSpline->SetLocationAtSplinePoint(1, pointB, ESplineCoordinateSpace::Local);
}

//void ACustomSpline::init(TArray<FVector> points)
//{
//    for (int i=0; i < points.Num(); i++)
//    {
//        PathSpline->AddSplineLocalPoint(points[i]);
//    }
//}


// Called when the game starts or when spawned
//void ACustomSpline::BeginPlay()
//{
//	Super::BeginPlay();
//	
//}
//
//// Called every frame
//void ACustomSpline::Tick( float DeltaTime )
//{
//	Super::Tick( DeltaTime );
//
//}

