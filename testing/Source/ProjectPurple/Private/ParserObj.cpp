// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectPurple.h"
#include "ParserObj.h"
#include "Card.h"
#include "CardData.h"


// Sets default values

UParserObj::UParserObj()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
//void UParserObj::BeginPlay()
//{
	//Super::BeginPlay();


//}


// Parse by name,text,type, image and seasons
UCard* UParserObj::ParseCard(FJsonObject* jsonCard) {
	UCard* card = NewObject<UCard>();

	for (auto it = jsonCard->Values.CreateConstIterator(); it; ++it)
	{

		if (it.Key().Equals(TEXT("name"), ESearchCase::IgnoreCase))
		{
			card->name = it.Value().Get()->AsString();
		}
		else if (it.Key().Equals(TEXT("text"), ESearchCase::IgnoreCase))
		{
			if (!it.Value().Get()->IsNull())
			{
				card->text = it.Value().Get()->AsString();
			}
		}
		else  if (it.Key().Equals(TEXT("type"), ESearchCase::IgnoreCase))
		{
			card->type = it.Value().Get()->AsString();
		}
		else  if (it.Key().Equals(TEXT("image"), ESearchCase::IgnoreCase))
		{
			card->image = it.Value().Get()->AsString();
		}

		// Assign season to each card
		else  if (it.Key().Equals(TEXT("seasons"), ESearchCase::IgnoreCase))
		{
			ESeason season = ESeason::None;

			auto seasons = it.Value().Get()->AsArray();
			for (auto i = it.Value().Get()->AsArray().CreateConstIterator(); i; ++i)
			{
				season = season | ESeason(1 << int32((*i)->AsNumber() - 1)); // OR the season to the card
			}

			card->properties.Add(it.Key(), Property(season));
		}
		else
		{

			FJsonValue* val = it.Value().Get();
			switch (val->Type)
			{
			case EJson::String:
				card->properties.Add(it.Key(), Property(val->AsString()));
				break;
			case EJson::Boolean:
				card->properties.Add(it.Key(), Property(val->AsBool()));
				break;
			case EJson::Number:
				card->properties.Add(it.Key(), Property(val->AsNumber()));
				break;
			case EJson::Array:
				/*card->properties.Add(it.Key(), Property(val->AsArray()));
				break;*/
			case EJson::None:
			case EJson::Null:
			case EJson::Object:
			default:
				// error, or warning...
				break;
			}

		}
	}
	return card;
}

// Load cards parses the data from the json object into the arrays of decks
void UParserObj::LoadCards()
{
	FString text(CardData::Size, CardData::Data);
	//UE_LOG(LogTemp, Log, TEXT("Loaded JSON Data: %s"), *text);

	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(text);   // Jsonreader -> Reference to the reader reference
	TSharedPtr<FJsonObject> CardData;																	   //	Check if parsed correctly
	if (FJsonSerializer::Deserialize(JsonReader, CardData))
	{
		auto resourceArr = CardData->GetArrayField("resources");

		for (int i = 0; i < resourceArr.Num(); i++)
		{
			UCard* card = ParseCard(resourceArr[i].Get()->AsObject().Get());
			card->layout = ECardLayout::Resource;

			if (card->type.Equals(TEXT("Resource"), ESearchCase::IgnoreCase))
			{
				float num = card->GetDoubleProperty("count");
				card->properties.Add("initial_value", Property(card->GetDoubleProperty("value")));
				card->currentOwner = card->GetStringProperty("owner");

				for (int i = 0; i < num; i++)
				{
					resources.Add(card);
				}
			}

			else if (card->type.Equals(TEXT("Lost Resource"), ESearchCase::IgnoreCase))
			{
				card->layout = ECardLayout::LostResource;

				float num = card->GetDoubleProperty("count");

				card->properties.Add("initial_value", Property(card->GetDoubleProperty("value")));

				card->currentOwner = card->GetStringProperty("owner");

				for (int i = 0; i < num; i++)
				{
					lostResources.Add(card);
				}
			}

			else if (card->type.Equals(TEXT("Starting Resource"), ESearchCase::IgnoreCase))
			{
				card->properties.Add("initial_value", Property(card->GetDoubleProperty("value")));
				card->currentOwner = card->GetStringProperty("owner");
				startingResources.Add(card);
			}
		}

		auto eventsArr = CardData->GetArrayField("events");
		for (int i = 0; i < eventsArr.Num(); i++)
		{
			auto eventDesc = eventsArr[i].Get()->AsObject().Get();
			auto seasons = eventDesc->GetArrayField("seasons");
			for (auto i = seasons.CreateConstIterator(); i; ++i)
			{
				UCard* card = ParseCard(eventDesc);
				card->layout = ECardLayout::Event;
				card->properties.Add("season", Property(ESeason(1 << int32((*i)->AsNumber() - 1))));

				if (card->ContainsProperty("value"))
				{
					card->properties.Add("initial_value", Property(card->GetDoubleProperty("value")));
				}

				else if (card->type == "Community - Disaster")
				{
					card->properties.Add("initial_cost", Property(card->GetDoubleProperty("col_cost")));
				}

				else if (card->type == "Community - Crisis")
				{
					card->properties.Add("initial_cost", Property(card->GetDoubleProperty("col_cost")));
					card->properties.Add("turns_active", Property(0.0));
				}

				events.Add(card);
			}
		}

		auto commResponsibilitiesArr = CardData->GetArrayField("communal_responsibilities");
		for (int i = 0; i < commResponsibilitiesArr.Num(); i++)
		{
			UCard* card = ParseCard(commResponsibilitiesArr[i].Get()->AsObject().Get());
			card->layout = ECardLayout::CommunalResponsibility;

			card->properties.Add("initial_cost", Property(card->GetDoubleProperty("cost")));

			communalResponsibilities.Add(card);
		}

		auto famResponsibilitiesArr = CardData->GetArrayField("family_responsibilities");
		for (int i = 0; i < famResponsibilitiesArr.Num(); i++)
		{
			UCard* card1 = ParseCard(famResponsibilitiesArr[i].Get()->AsObject().Get());
			card1->layout = ECardLayout::FamilyResponsibility;
			card1->properties.Add("initial_cost", Property(card1->GetDoubleProperty("cost")));

			UCard* card2 = ParseCard(famResponsibilitiesArr[i].Get()->AsObject().Get());
			card2->layout = ECardLayout::FamilyResponsibility;
			card2->properties.Add("initial_cost", Property(card2->GetDoubleProperty("cost")));

			UCard* card3 = ParseCard(famResponsibilitiesArr[i].Get()->AsObject().Get());
			card3->layout = ECardLayout::FamilyResponsibility;
			card3->properties.Add("initial_cost", Property(card3->GetDoubleProperty("cost")));

			UCard* card4 = ParseCard(famResponsibilitiesArr[i].Get()->AsObject().Get());
			card4->layout = ECardLayout::FamilyResponsibility;
			card4->properties.Add("initial_cost", Property(card4->GetDoubleProperty("cost")));

			UCard* card5 = ParseCard(famResponsibilitiesArr[i].Get()->AsObject().Get());
			card5->layout = ECardLayout::FamilyResponsibility;
			card5->properties.Add("initial_cost", Property(card5->GetDoubleProperty("cost")));

			// need to make 5 separate arrays here
			familyResponsibilities1.Add(card1);
			familyResponsibilities2.Add(card2);
			familyResponsibilities3.Add(card3);
			familyResponsibilities4.Add(card4);
			familyResponsibilities5.Add(card5);
		}

		auto rolesArr = CardData->GetArrayField("roles");
		for (int i = 0; i < rolesArr.Num(); i++)
		{
			UCard* card = ParseCard(rolesArr[i].Get()->AsObject().Get());
			card->layout = ECardLayout::Role;

			roles.Add(card);
		}

	}
	else
	{
		// Couldn't read file 
	}
	UE_LOG(LogTemp, Warning, TEXT("Size of the cards: %d"), events.Num());

}
