// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectPurple.h"
//#include "PaperSprite.h"
#include "ProjectPurpleFunctionLibrary.h"

UObject* UProjectPurpleFunctionLibrary::LoadObjectFromAssetPath(TSubclassOf<UObject> ObjectClass, FName Path, bool& IsValid)
{
	IsValid = false;

	if (Path == NAME_None) return NULL;
	//~~~~~~~~~~~~~~~~~~~~~

	UObject* LoadedObj = StaticLoadObject(ObjectClass, NULL, *Path.ToString());

	IsValid = LoadedObj != nullptr;

	return LoadedObj;
}
FName UProjectPurpleFunctionLibrary::GetObjectPath(UObject* Obj)
{
	if (!Obj) return NAME_None;
	if (!Obj->IsValidLowLevel()) return NAME_None;
	//~~~~~~~~~~~~~~~~~~~~~~~~~

	FStringAssetReference ThePath = FStringAssetReference(Obj);

	if (!ThePath.IsValid()) return "";

	//The Class FString Name For This Object
	FString str = Obj->GetClass()->GetDescription();

	//Remove spaces in Material Instance Constant class description!
	str = str.Replace(TEXT(" "), TEXT(""));

	str += "'";
	str += ThePath.ToString();
	str += "'";

	return FName(*str);
}

//FVector2D UProjectPurpleFunctionLibrary::GetSourceSize(UPaperSprite* sprite)
//{
//    return sprite->GetSourceSize();
//    
//}

bool UProjectPurpleFunctionLibrary::VictorySoundVolumeChange(USoundClass* SoundClassObject, float NewVolume)
{
    if(!SoundClassObject)
    {
        return false;
    }
    
    SoundClassObject->Properties.Volume = NewVolume;
    return true;
	   
    /*
     FAudioDevice* Device = GEngine->GetAudioDevice();
     if (!Device || !SoundClassObject)
     {
     return false;
     }
	    
     bool bFound = Device->SoundClasses.Contains(SoundClassObject);
     if(bFound)
     {
     Device->SetClassVolume(SoundClassObject, NewVolume);
     return true;
     }
     return false;
     */
    
    /*
     bool SetBaseSoundMix( class USoundMix* SoundMix );
     
     */
}
float UProjectPurpleFunctionLibrary::VictoryGetSoundVolume(USoundClass* SoundClassObject)
{
    if (!SoundClassObject)
    {
        return -1;
    }
    return SoundClassObject->Properties.Volume;
    /*
     FAudioDevice* Device = GEngine->GetMainAudioDevice();
     if (!Device || !SoundClassObject)
     {
     return -1;
     }
	    
     FSoundClassProperties* Props = Device->GetSoundClassCurrentProperties(SoundClassObject);
     if(!Props) return -1;
     return Props->Volume;
     */
}


