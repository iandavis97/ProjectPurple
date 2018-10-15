// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectPurple.h"
#include "Card.h"

#if UE_EDITOR

//DEFINE_LOG_CATEGORY(LogUnion);

#endif

#define CHECK_PROPERTY(key, ret) \
if (!ContainsProperty(key)) \
{ \
		UE_LOG(LogTemp, Warning, TEXT("[Card %s] Invalid property: %s"), *name, *key); \
		return ret; \
} \

FString UCard::GetStringProperty(FString key)
{
	CHECK_PROPERTY(key, "<INVALID_PROPERTY>");
	return properties[key].GetSubtype<FString>();
}

float UCard::GetDoubleProperty(FString key)
{
	CHECK_PROPERTY(key, 0);
	return (float)properties[key].GetSubtype<double>();
}

bool UCard::GetBoolProperty(FString key)
{
	CHECK_PROPERTY(key, 0);
	return properties[key].GetSubtype<bool>();
}

ESeason UCard::GetSeasonProperty()
{
	CHECK_PROPERTY(TEXT("season"), ESeason::None);
	return properties[TEXT("season")].GetSubtype<ESeason>();
}

FString UCard::GetFormattedText()
{
	//	if (!formattedText.IsEmpty())
	//	{
	//		return formattedText;
	//	}

	formattedText = text;
	for (auto it = properties.CreateConstIterator(); it; ++it)
	{
		FString textToReplace;
		Property value = it.Value();
		if (value.HasSubtype<FString>())
		{
			textToReplace = value.GetSubtype<FString>();
		}
		else if (value.HasSubtype<double>())
		{
			textToReplace = FString::FormatAsNumber(value.GetSubtype<double>());
		}
		else if (value.HasSubtype<ESeason>())
		{
			textToReplace = Utils::GetSeasonAsString(value.GetSubtype<ESeason>());
		}
		else {
			textToReplace = "";
		}

		formattedText = formattedText.Replace(*FString::Printf(TEXT("{%s}"), *it.Key()), *textToReplace);
	}


	// need to check all cards by type and from there figure out where line breaks need to go in the text.
	// Line breaks in text for text render components are accomplished using the standard html break tag <br>
	// for event cards, the line will break after the determined character length of a line, and after any periods (when the sentence ends)

	// this check will be for communals
	if (type == "Communal Responsibility")
	{
		formattedText = FormatTextForCommunal(formattedText);
	}
	else if (type == "Family Responsibility")
	{
		formattedText = FormatTextForFamily(formattedText);
	}
	else if (type == "Individual - Lost" || type == "Individual - Found" || type == "Community - Festival" || type == "Community - Crisis" || type == "Community - Disaster")
	{
		formattedText = FormatTextForEvent(formattedText);
	}
	else if (type == "Resource" || type == "Lost Resource" || type == "Starting Resource")
	{
		formattedText = FormatTextForResources(formattedText);
	}
	else if (type == "Starting Role")
	{
		formattedText = FormatTextForRoles(formattedText);
	}

	return formattedText;
}

FString UCard::FormatTextForCommunal(FString textOnCard)
{
	textOnCard.ReplaceInline(*FString("\n"), *FString(" "));
	int lengthOfLine = 0; // once length of line reaches 64 with no periods, need to add line break tag. if in middle of word, go back to last space and add tag
	TArray<int32> indicesForBreaks;

	for (int i = 0; i < textOnCard.Len(); i++)
	{
		if (textOnCard[i] == '.')
		{
			indicesForBreaks.Add(i + 1);
			lengthOfLine = 0; // reset the counter for a new full line
		}

		else if (lengthOfLine == 64)
		{
			if (textOnCard[i] == ' ')
			{
				indicesForBreaks.Add(i);
			}
			else // the last char on the line is the beginning or in the middle of a word --> Need to turn back
			{
				// need to loop backwards to find the last space between words, to add a break
				for (int j = i; j > 0; j--)
				{
					if (textOnCard[j] == ' ')
					{
						indicesForBreaks.Add(j);
						break;
					}
				}
			}

			lengthOfLine = 0;
		}

		else // no conditions met
		{
			lengthOfLine++;
		}
	}

	for (int i = 0; i < indicesForBreaks.Num(); i++)
	{
		indicesForBreaks[i] += (4 * i);
		textOnCard.InsertAt(indicesForBreaks[i], TEXT("<br>"));
	}

	// need to find the "Bonus" string and add a break before it, this may change
	int bonusIndex = textOnCard.Find(FString("Bonus"));
	//if (textOnCard[bonusIndex - 1] != *TEXT(">")) textOnCard.InsertAt(bonusIndex - 1, TEXT("<br>"));

	return textOnCard;
}

FString UCard::FormatTextForFamily(FString textOnCard)
{
	textOnCard.ReplaceInline(*FString("\n"), *FString(" "));
	int lengthOfLine = 0; // once length of line reaches 36 with no periods, need to add line break tag. if in middle of word, go back to last space and add tag
	TArray<int32> indicesForBreaks;

	for (int i = 0; i < textOnCard.Len(); i++)
	{
		if (textOnCard[i] == '.')
		{
			indicesForBreaks.Add(i + 1);
			lengthOfLine = 0; // reset the counter for a new full line
		}

		else if (lengthOfLine == 36)
		{
			if (textOnCard[i] == ' ')
			{
				indicesForBreaks.Add(i);
			}
			else // the last char on the line is the beginning or in the middle of a word --> Need to turn back
			{
				// need to loop backwards to find the last space between words, to add a break
				for (int j = i; j > 0; j--)
				{
					if (textOnCard[j] == ' ')
					{
						indicesForBreaks.Add(j);
						break;
					}
				}
			}

			lengthOfLine = 0;
		}

		else // no conditions met
		{
			lengthOfLine++;
		}
	}

	for (int i = 0; i < indicesForBreaks.Num(); i++)
	{
		indicesForBreaks[i] += (4 * i);
		textOnCard.InsertAt(indicesForBreaks[i], TEXT("<br>"));
	}

	// need to find the "Bonus" string and add a break before it, this may change
	//int bonusIndex = textOnCard.Find(FString("Bonus"));

	return textOnCard;
}

FString UCard::FormatTextForEvent(FString textOnCard)
{
	textOnCard.ReplaceInline(*FString("\n"), *FString(" "));
	int lengthOfLine = 0; // once length of line reaches 50 with no periods, need to add line break tag. if in middle of word, go back to last space and add tag
	TArray<int32> indicesForBreaks;

	for (int i = 0; i < textOnCard.Len(); i++)
	{
		if (textOnCard[i] == '.' && lengthOfLine > 50)
		{
			indicesForBreaks.Add(i + 1);
			lengthOfLine = 0; // reset the counter for a new full line
		}

		else if (lengthOfLine == 70)
		{
			if (textOnCard[i] == ' ')
			{
				indicesForBreaks.Add(i);
			}
			else // the last char on the line is the beginning or in the middle of a word --> Need to turn back
			{
				// need to loop backwards to find the last space between words, to add a break
				for (int j = i; j > 0; j--)
				{
					if (textOnCard[j] == ' ')
					{
						indicesForBreaks.Add(j);
						break;
					}
				}
			}

			lengthOfLine = 0;
		}

		else // no conditions met
		{
			lengthOfLine++;
		}
	}

	for (int i = 0; i < indicesForBreaks.Num(); i++)
	{
		indicesForBreaks[i] += (4 * i);
		textOnCard.InsertAt(indicesForBreaks[i], TEXT("<br>"));
	}

	// need to find the "Bonus" string and add a break before it, this may change
	//int bonusIndex = textOnCard.Find(FString("Bonus"));

	return textOnCard;
}

FString UCard::FormatTextForResources(FString textOnCard)
{
	textOnCard.ReplaceInline(*FString("\n"), *FString(" "));
	int lengthOfLine = 0; // once length of line reaches 50 with no periods, need to add line break tag. if in middle of word, go back to last space and add tag
	TArray<int32> indicesForBreaks;

	for (int i = 0; i < textOnCard.Len(); i++)
	{
		if (lengthOfLine == 50)
		{
			if (textOnCard[i] == ' ')
			{
				indicesForBreaks.Add(i);
			}
			else // the last char on the line is the beginning or in the middle of a word --> Need to turn back
			{
				// need to loop backwards to find the last space between words, to add a break
				for (int j = i; j > 0; j--)
				{
					if (textOnCard[j] == ' ')
					{
						indicesForBreaks.Add(j);
						break;
					}
				}
			}

			lengthOfLine = 0;
		}

		else // no conditions met
		{
			lengthOfLine++;
		}
	}

	for (int i = 0; i < indicesForBreaks.Num(); i++)
	{
		indicesForBreaks[i] += (4 * i);
		textOnCard.InsertAt(indicesForBreaks[i], TEXT("<br>"));
	}

	// need to find the "Bonus" string and add a break before it, this may change
	//int bonusIndex = textOnCard.Find(FString("Bonus"));

	return textOnCard;
}

FString UCard::FormatTextForRoles(FString textOnCard)
{
	textOnCard.ReplaceInline(*FString("\n"), *FString(" "));
	int lengthOfLine = 0; // once length of line reaches 42 with no periods, need to add line break tag. if in middle of word, go back to last space and add tag
	TArray<int32> indicesForBreaks;

	for (int i = 0; i < textOnCard.Len(); i++)
	{
		if (textOnCard[i] == '.' && lengthOfLine > 30)
		{
			indicesForBreaks.Add(i + 1);
			lengthOfLine = 0; // reset the counter for a new full line
		}

		else if (lengthOfLine == 42)
		{
			if (textOnCard[i] == ' ')
			{
				indicesForBreaks.Add(i);
			}
			else // the last char on the line is the beginning or in the middle of a word --> Need to turn back
			{
				// need to loop backwards to find the last space between words, to add a break
				for (int j = i; j > 0; j--)
				{
					if (textOnCard[j] == ' ')
					{
						indicesForBreaks.Add(j);
						break;
					}
				}
			}

			lengthOfLine = 0;
		}

		else // no conditions met
		{
			lengthOfLine++;
		}
	}

	for (int i = 0; i < indicesForBreaks.Num(); i++)
	{
		indicesForBreaks[i] += (4 * i);
		textOnCard.InsertAt(indicesForBreaks[i], TEXT("<br>"));
	}

	// need to find the "Bonus" string and add a break before it, this may change
	//int bonusIndex = textOnCard.Find(FString("Bonus"));

	return textOnCard;
}

void UCard::SetStringProperty(FString key, FString value)
{
	if (this->IsValidLowLevel())
	{
		properties[key] = Property(FString(value));
	}
}

void UCard::SetDoubleProperty(FString key, float value)
{
	if (this->IsValidLowLevel())
	{
		properties[key] = Property(value);
	}
}

void UCard::SetBoolProperty(FString key, bool value)
{
	if (this->IsValidLowLevel())
	{
		properties[key] = Property(value);
	}
}

void UCard::SetSeasonProperty(ESeason value)
{
	if (this->IsValidLowLevel())
	{
		properties[TEXT("season")] = Property(value);
	}
}

bool UCard::ContainsProperty(FString key)
{
	return properties.Contains(key);
}
