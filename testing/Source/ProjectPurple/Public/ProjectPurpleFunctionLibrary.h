// Fill out your copyright notice in the Description page of Project Settings.
// Uses 4 functions from the VictoryBPFunctionLibrary created by Rama on the UE4 answers forum.
// Can be found here: https://github.com/EverNewJoy/VictoryPlugin
// These 4 functions were used because in the whole plugin apex was not supported on mobile, so the project would not build
#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
//#include "PaperSprite.h"
#include "ProjectPurpleFunctionLibrary.generated.h"

/**
 *
 */
UCLASS()
class PROJECTPURPLE_API UProjectPurpleFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	/** The FName that is expected is the exact same format as when you right click on asset -> Copy Reference! You can directly paste copied references into this node! IsValid lets you know if the path was correct or not and I was able to load the object. MAKE SURE TO SAVE THE RETURNED OBJECT AS A VARIABLE. Otherwise your shiny new asset will get garbage collected. I recommend you cast the return value to the appropriate object and then promote it to a variable :)  -Rama */
	UFUNCTION(BlueprintCallable, Category = "PurpleBPLibrary")
		static UObject* LoadObjectFromAssetPath(TSubclassOf<UObject> ObjectClass, FName Path, bool& IsValid);

	/** Uses the same format as I use for LoadObjectFromAssetPath! Use this node to get the asset path of objects in the world! -Rama */
	UFUNCTION(BlueprintPure, Category = "PurpleBPLibrary")
		static FName GetObjectPath(UObject* Obj);
    
//    UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Source Size", Keywords = "PurpleBPLibrary"), Category = Custom) //Here you can change the keywords, name and category
//        static FVector2D GetSourceSize(UPaperSprite* sprite);
    
    /** Change volume of Sound class of your choosing, sets the volume instantly! Returns false if the sound class was not found and volume was not set. */
    UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary|Sound")
    static bool VictorySoundVolumeChange(USoundClass* SoundClassObject, float NewVolume);
    
    /** Get Current Sound Volume! Returns -1 if sound class was not found*/
    UFUNCTION(BlueprintPure, Category = "VictoryBPLibrary|Sound")
    static float VictoryGetSoundVolume(USoundClass* SoundClassObject);
	
};
