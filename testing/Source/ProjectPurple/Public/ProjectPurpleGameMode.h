// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "PurpleController.h"
#include "CardActor.h"
#include "Card.h"
#include "CustomSpline.h"
#include "Engine.h"
#include "BoxButton.h"
#include <string.h>
#include "ProjectPurpleGameMode.generated.h"

/**
 * Game mode is used to control the flow of the game, the c++ of game mode is just functions that are used and extended in the blueprint portion of our code. Think
 of the c++ as a logic and function library for the blueprint where the flow of the game is handled
 */

 //Implementable Event Definitions
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDiscardAnimationDispatcher, ACardActor*, Actor, FVector, Destination);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDrawAnimationDispatcher, ACardActor*, Actor);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDisasterAnimationDispatcher, ACardActor*, Actor);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCrisisAnimationDispatcher, ACardActor*, Actor);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMoveCardAnimationDispatcher, ACardActor*, Actor, FVector, Destination);


UENUM(BlueprintType)
enum class ETurnPhase: uint8 {
	None,
	DrawResource,
	DrawEvents,
	ResolveEvents,
	ReturnItems,
	ResolutionOfCrisis,
	AdvanceResponsibilities,
    Teshuvah,
	Discard,
};

UENUM(BlueprintType)
enum class ECrisisFailed: uint8 {
	None,
	Locusts,
	Monsoon,
	Plague,
};

UENUM(BlueprintType)
enum class EInputTypes: uint8 {
	None,
	Communal,
	Continue,
	Crisis,
	Disaster,
	DiscardTray,
	EndGame,
	Family,
	ReturnItem,
	Teshuvah,
};

UENUM(BlueprintType)
enum class EHeshbonType : uint8
{
	None,
	BeyondTheLaw,
	LostAndFound,
	Transgression,
};

UENUM(BlueprintType)
enum class EEndGameTypes: uint8
{
	None,
	Destitution,
	NoMoreEvents,
};

UENUM(BlueprintType)
enum class EEventOptions: uint8
{
	None,
	AnimalRunsAway,
	Crisis,
	Disaster,
	Festival,
	FoundObjectInRiver,
	HoneyJarCracks,
	KeepReturnAndIgnore,
	MoneyInDust,
	MoneyPurseInRiver,
	NoOptionAcquire,
	NoOptionLoss,
};

USTRUCT(BlueprintType)
struct FHeshbon
{
	GENERATED_BODY()

		UPROPERTY(BlueprintReadOnly)
		int32 outcome;

	UPROPERTY(BlueprintReadOnly)
		int32 weight;

	UPROPERTY(BlueprintReadOnly)
		FString text;

	UPROPERTY(BlueprintReadOnly)
		EHeshbonType type;

};

extern FHeshbon HESHBON[14];

USTRUCT(blueprintable)
struct FArrayOfCardArrays
{
    GENERATED_USTRUCT_BODY()
    
    UPROPERTY(BlueprintReadWrite)
    TArray<UCard*> dateFarmersCards;
    
    UPROPERTY(BlueprintReadWrite)
    TArray<UCard*> strangersCards;
    
    UPROPERTY(BlueprintReadWrite)
    TArray<UCard*> vintnersCards;
    
    UPROPERTY(BlueprintReadWrite)
    TArray<UCard*> potterCards;
    
    UPROPERTY(BlueprintReadWrite)
    TArray<UCard*> cowherdsCards;
    
    UPROPERTY(BlueprintReadWrite)
    TArray<UCard*> shepherdCards;
    
    UPROPERTY(BlueprintReadWrite)
    TArray<UCard*> anyCards;
    
};

UCLASS()
class PROJECTPURPLE_API AProjectPurpleGameMode : public AGameMode
{
	GENERATED_BODY()

		ETurnPhase turnPhase;


public:
    

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GameMode Properties")
		UWorld* World;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GameMode Properties")
		bool DatePowerCheck = false;//ensures player won't get stuck in loop
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GameMode Properties")
		bool AIDatePowerCheck = false;//ensures AI won't get stuck in loop
	// Card BP declaration for card actors
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BP Classes")
		UClass* CardBlueprint;

	// Card Spawning Helper Funcitons
//	UFUNCTION(BlueprintCallable, Category = "Card Spawning")
//		ACardActor* SpawnCard(UCard* cardData, FVector location, FRotator rotation, float scalar);

	// Spline Spawning Helper Functions. With and without additonal points
	//UFUNCTION(BlueprintCallable, Category = "Spline Spawning")
		//ACustomSpline* SpawnSplineWithPoints(const FVector& pointA, const FVector& pointB, const TArray<FVector>& additionalPoints, FVector splineLocation, FRotator splineRotation);

	//UFUNCTION(BlueprintCallable, Category = "Spline Spawning")
		//ACustomSpline* SpawnSpline(const FVector& pointA, const FVector& pointB, FVector splineLocation, FRotator splineRotation);
    
    UFUNCTION(BlueprintCallable, Category = "Array Sorting")
    FArrayOfCardArrays SortArrayByOwner(TArray<class UCard*> cards);


	// Constructor
	AProjectPurpleGameMode(const FObjectInitializer& ObjectInitializer);

	//virtual void BeginPlay() override;

	// Properties that track game objects necessary for logic ---------------------------------
    
    

	

	//// Array of Event cards that will affect the Players 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameModeProperties")
		TArray<class UCard*> eventsList;

	//// The total list of Communal Responsibilities 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameModeProperties")
		TArray<class UCard*> communalResponsibilities;

	//// The total list of Family Responsibilities 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameModeProperties")
		TArray<class UCard*> familyResponsibilities1;

	//// The total list of Family Responsibilities 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameModeProperties")
		TArray<class UCard*> familyResponsibilities2;

	//// The total list of Family Responsibilities 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameModeProperties")
		TArray<class UCard*> familyResponsibilities3;

	//// The total list of Family Responsibilities 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameModeProperties")
		TArray<class UCard*> familyResponsibilities4;

	//// The total list of Family Responsibilities 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameModeProperties")
		TArray<class UCard*> familyResponsibilities5;

	//// A list of the Lost Resource objects 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameModeProperties")
		TArray<class UCard*> lostResources;

	//// A list of roles 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameModeProperties")
		TArray<class UCard*> rolesList;

	//// A list of roles for male 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameModeProperties")
		TArray<class UCard*> maleRolesList;

	//// A list of roles for female 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameModeProperties")
		TArray<class UCard*> femaleRolesList;

	//// A list of Resource objects 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameModeProperties")
		TArray<class UCard*> resourcesList;

	// A list of completed  Communal Responsibilities 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameModeProperties")
		TArray<class UCard*> completedCommunalResponsibilities;

	// A list of the active Communal Responsibilities
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameModeProperties")
		TArray<class UCard*> activeCommunalResponsibilities;

	//// A list of active crisis cards 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameModeProperties")
		TArray<class UCard*> activeCrises;

	// need to know for tshuvah
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameModeProperties")
		int32 festivalsComplete;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameModeProperties")
		bool TshuvahAllowed;


	// Discard Piles ---------------------------------------------------------------------------------

	//// A list of the Lost Resource objects discard
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameModeProperties")
		TArray<class UCard*> lostResourcesDiscard;

	//// A list of Resource objects discard
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameModeProperties")
		TArray<class UCard*> resourcesListDiscard;

	//// A list of Resource objects 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameModeProperties")
		TArray<class UCard*> eventsListDiscard;

	//// A list of the starting resource objects 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameModeProperties")
		TArray<class UCard*> startingResources;

	// Current Event 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameModeProperties")
	class UCard* currentEvent;

	// Active player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameModeProperties")
	class ASuperController* activePlayer;

	//// Active player Index
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameModeProperties")
		int32 activePlayerIndex;

	//// Destitution bool
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameModeProperties")
		bool destitution;



	//// index used for money in the dust event, only in c++
	UPROPERTY(EditAnywhere, Category = "GameModeProperties")
		int32 moneyInDustIndex;

	//// index used for animal runs away event, only in c++
	UPROPERTY(EditAnywhere, Category = "GameModeProperties")
		int32 animalRunsIndex;

	//// index used for the honey jar cracks events, only in c++
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameModeProperties")
		int32 honeyJarIndex;

	//// index used for the honey jar cracks events, only in c++
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameModeProperties")
		int32 vesselIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameModeProperties")
		UCard* jarOfHoney;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameModeProperties")
		UCard* pitcherOfWine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameModeProperties")
		UCard* emptyVessel;



	// Display Piles ---------------------------------------------------------------------------------

	// Array of Event cards displayed on screen
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameModeProperties")
	class ACardActor* displayedEvent;

	// The list of Communal Responsibilities displayed on screen
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameModeProperties")
		TArray<class ACardActor*> displayedCommunalResponsibilities;

	// The list of Crises shown
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameModeProperties")
		TArray<class ACardActor*> displayedCrises;



	// Splines for displaying cards ------------------------------------------------------------------

	// Spline that determines orientation of cards on screen for communal responsibilities
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameModeProperties")
		ACustomSpline* communalSpline;

	// Spline that determines orientation of cards on screen for hand
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameModeProperties")
		ACustomSpline* handSpline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameModeProperties")
		ACustomSpline* restingHandSpline;

	//Spline used for swiping animations
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameModeProperties")
		ACustomSpline* swipeSpline;

	// Spline that determines path of card during animation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameModeProperties")
		ACustomSpline* drawAnimationSpline;

	// Spline that shows completed family
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameModeProperties")
		ACustomSpline* completedCommunalSpline;

	// Spline that shows completed communal
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameModeProperties")
		ACustomSpline* completedFamilySpline;

	//Need a game state object ****

	// Functions needed for initialization and game logic

	// spawns player objects for instantiation
	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		void SpawnPlayers();
    
	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		void NeedToReshuffleResources();

	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		void AdjustResponsibilityValue(UCard* resp);

	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		void AdjustResponsibilityOnDraw(UCard* resp);

	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		TArray<ASuperController*> DisasterPlayerList();

    // spawns player objects for instantiation
    UFUNCTION(BlueprintCallable, Category = "GameFunctions")
        void PopulateRolesList();

	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		void StartGame();

	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		void GiveStartingHands();

	// sets turn phase
	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		void AdvanceTurnPhase();

	// gets turn phase
	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		ETurnPhase GetTurnPhase();

	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		void SetTurnPhase(ETurnPhase phase);

	UFUNCTION(BlueprintImplementableEvent, Category = "GameFunctions")
		void OnBeginTurnPhase(ETurnPhase turnPhase);

	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		bool IsGameOver();

	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		EEndGameTypes ReturnGameOverEnum();

	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		EEventOptions ReturnEventEnum();

	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		UCard* NoOptionAcquireEvents();

	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		UCard* NoOptionLossEvents();

	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		void KeepReturnAndIgnoreEvents();

	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		void MoneyPurseInRiver();

	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		bool AnimalRunsAwayStageOne();

	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		UCard* AnimalRunsAwayStageTwo();

	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		void AnimalRunsAwayStageThree(ACardActor* card);

	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		bool HoneyJarCracksStageOne();

	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		void HoneyJarCracksStageTwoNegotiatePass(TArray<class ACardActor*> cards);

	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		void HoneyJarCracksStageTwoNegotiateFail();

	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		void HoneyJarCracksStageTwoIgnore();

	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		void HoneyJarCracksStageTwoHelp();

	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		UCard* FoundObjectInRiverStageOne();

	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		void FoundObjectInRiverStageTwo(ACardActor* card);
	
	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		bool MoneyInDustStageOne();

	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		UCard* MoneyInDustStageTwo();

	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		void MoneyInDustStageThree(ACardActor* card);

	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		bool DisasterPassed();

	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		ECrisisFailed CrisisFail();

	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		void AddToActiveCrises(UCard* crisis);

	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		void FestivalFinalStage();

	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		FHeshbon GetHeshbonByType(EHeshbonType type);

	// used in trading with another player
	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		void SwapCards(const TArray<UCard*>& firstCards, const TArray<UCard*>& otherPlayerCards, ASuperController* firstPlayer, ASuperController* otherPlayer);

	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		bool AIWillingToTrade(ASuperController* humanPlayer, ASuperController* AIPlayer);

	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		TArray<UCard*> TradeFromHumanToAI(const TArray<UCard*>& humanCards, ASuperController* humanPlayer, ASuperController* AIPlayer);

	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		void ChangeOwnerOnCard(UCard* card, ASuperController * player);

	UFUNCTION(BlueprintCallable, Category = "GameFunctions")
		int32 CheckPlayerStanding();


	// Card functions to help set up game  -----------------------------------------------------
	// populates decks with each list of cards from the parser
	UFUNCTION(BlueprintCallable, Category = "CardFunctions")
		void SetupCardLists();

	//Shuffle cards for respective deck
	UFUNCTION(BlueprintCallable, Category = "CardFunctions")
		TArray<class UCard*> ShuffleCards(TArray<class UCard*> deck);

	//Shuffle cards for Events
	UFUNCTION(BlueprintCallable, Category = "CardFunctions")
		TArray<class UCard*> ShuffleEvents(TArray<class UCard*> events);


	// local helper functions -----------------------------------------------------------------

	// gets player with the lowest amount of family responsibilities
	UFUNCTION(BlueprintCallable, Category = "GameHelpers")
		int32 getPlayerWithLowestFamily();

	// gets player with the highest amount of family responsibilities
	UFUNCTION(BlueprintCallable, Category = "GameHelpers")
		int32 getPlayerWithHighestFamily();


	//Event Dispatchers for calling Events ----------------------------------------------------- DEPRECATED
	/*UPROPERTY(BlueprintAssignable, Category = "AnimationManager")
		FDiscardAnimationDispatcher DiscardAnimationDispatcher;

	UPROPERTY(EditAnywhere, BlueprintAssignable, Category = "AnimationManager")
		FDrawAnimationDispatcher DrawAnimationDispatcher;

	UPROPERTY(BlueprintAssignable, Category = "AnimationManager")
		FDisasterAnimationDispatcher DisasterAnimationDispatcher;

	UPROPERTY(BlueprintAssignable, Category = "AnimationManager")
		FCrisisAnimationDispatcher CrisisAnimationDispatcher;*/

	/*UPROPERTY(BlueprintAssignable, Category = "AnimationManager")
		FMoveCardAnimationDispatcher MoveCardAnimationDispatcher;*/

	// Returns a value for C++
	//UFUNCTION(BlueprintImplementableEvent , Category = "AnimationManager")
		//void DisplayButtonOptions(TArray<FString> options, int32& choice);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "AnimationManager")
		void DisplayButtonOptions(const TArray<FString>& options);

	// Returns a value for C++
	UFUNCTION(BlueprintImplementableEvent, Category = "AnimationManager")
		void DisplayHandOptions(const TArray<ACardActor*>& hand);

	// properties used for implementables
	// The hand choices
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Properties")
		TArray<class ACardActor*> handChoices;

	// The hand choices
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Properties")
		int32 buttonChoice;
    
    

	UFUNCTION(BlueprintCallable, Category = "Input Functions")
		void assignButtonChoice(int32 newValue);

	UFUNCTION(BlueprintCallable, Category = "Input Functions")
		int32 retrieveButtonChoice();

	UFUNCTION(Category = "Input Functions")
		void checkButtons();
    
    //Property to set the number of players in a game.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameModeProperties")
    int32 initialNumPlayers = 5;
    
    //// Array of Players which has access to: Cards, responsibilities, role, and number of cards
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameModeProperties")
    TArray<class ASuperController*> players;

	//The index of player who looses when in the nooptionloss phase
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameModeProperties")
	int32 lostUsedIndex = -1;
};
