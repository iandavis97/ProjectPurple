// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "Union.h"
#include "Card.generated.h"

// Season Enum (can be used as bit flag to hold more than one season)
// e.g. "season = ESeason::Winter | ESeason::Spring;"
UENUM(BlueprintType)
enum class ESeason : uint8
{
	None   = 0x00,
	Winter = 0x01,
	Spring = 0x02,
	Summer = 0x04,
	Autumn = 0x08,
};
ENUM_CLASS_FLAGS(ESeason)

UENUM(BlueprintType)
enum class ECardLayout : uint8
{
	None,
	Resource,
	LostResource,
	Event,
	FamilyResponsibility,
	CommunalResponsibility,
	Role
};

namespace Utils {
	static FORCEINLINE FString GetSeasonAsString(ESeason EnumValue)
	{
		const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ESeason"), true);
		if (!EnumPtr)
		{
			return FString("Invalid");
		}

		return EnumPtr->GetEnumName(int32(EnumValue));
	}

	template<typename TEnum>
	static FORCEINLINE FString GetEnumValueToString(const FString& Name, TEnum Value)
	{
		const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, *Name, true);
		if (!enumPtr)
		{
			return FString("Invalid");
		}

		return enumPtr->GetEnumName(int32(Value));
	}
}

typedef TUnion<FString, double, bool, ESeason> Property;

/**
 *
 */
UCLASS(BlueprintType)
class PROJECTPURPLE_API UCard : public UObject
{
	GENERATED_BODY()

	FString formattedText;

public:
	
	// properties common to all cards
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Common")
		int32 id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Common")
		FString name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Common")
		FString image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Common")
		FString text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Common")
		FString type;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Common")
		ECardLayout layout;


	// needed for trading and some events
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Common")
		FString currentOwner;



	// union for properties and respective getters and setters for each type of dynamic property
	TMap<FString, Property> properties;


	// Helper funtions for card properties ----------------------------------------------------------

	// Getters
	// gets string property with given key 
	UFUNCTION(BlueprintCallable, Category = "Common")
		FString GetStringProperty(FString key);

	// it is called double from parser, but float is supported in ue4
	UFUNCTION(BlueprintCallable, Category = "Common")
		float GetDoubleProperty(FString key);

	// gets bool for given key
	UFUNCTION(BlueprintCallable, Category = "Common")
		bool GetBoolProperty(FString key);

	// gets Season for given key
	UFUNCTION(BlueprintCallable, Category = "Common")
		ESeason GetSeasonProperty();

	// gets Season for given key
	UFUNCTION(BlueprintCallable, Category = "Common")
		FString GetFormattedText();

	UFUNCTION(BlueprintCallable, Category = "Common")
		FString FormatTextForCommunal(FString textOnCard);

	UFUNCTION(BlueprintCallable, Category = "Common")
		FString FormatTextForFamily(FString textOnCard);

	UFUNCTION(BlueprintCallable, Category = "Common")
		FString FormatTextForEvent(FString textOnCard);

	UFUNCTION(BlueprintCallable, Category = "Common")
		FString FormatTextForResources(FString textOnCard);

	UFUNCTION(BlueprintCallable, Category = "Common")
		FString FormatTextForRoles(FString textOnCard);

	// Setters
	// sets string value for given key
	UFUNCTION(BlueprintCallable, Category = "Common")
		void SetStringProperty(FString key, FString value);

	// sets double value for given key
	UFUNCTION(BlueprintCallable, Category = "Common")
		void SetDoubleProperty(FString key, float value);

	// sets bool value for given key
	UFUNCTION(BlueprintCallable, Category = "Common")
		void SetBoolProperty(FString key, bool value);

	// gets Season for given key
	UFUNCTION(BlueprintCallable, Category = "Common")
		void SetSeasonProperty(ESeason value);


	// checks to see if property is valid part of the card
	UFUNCTION(BlueprintCallable, Category = "Common")
		bool ContainsProperty(FString key);
};
