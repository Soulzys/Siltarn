#include "GameModes/DefaultGameMode.h"
#include "Siltarn/Public/HUDs/GameplayHUD.h"
#include "Siltarn/Public/Characters/SiltarnPlayerController.h"

ADefaultGameMode::ADefaultGameMode()
{
	HUDClass              = AGameplayHUD            ::StaticClass();
	PlayerControllerClass = ASiltarnPlayerController::StaticClass();

	UE_LOG(LogTemp, Error, TEXT("I'm a game mode bitch !"));
}

void ADefaultGameMode::BeginPlay()
{
	Super::BeginPlay();

	FDateTime _DateTimeZone = FDateTime::Now();


	FString _DateTimeZoneToInt = _DateTimeZone.ToString();

	int32 _Milliseconds = _DateTimeZone.GetMillisecond();

	

	int32 _FillingNumber = 0;

	// Not optimal but... eh 
	if (_Milliseconds < 100)
	{	
		_FillingNumber = FMath::RandRange(1, 9);

		if (_Milliseconds < 10)
		{
			_FillingNumber = FMath::RandRange(10, 99);
		}		
	}


	_DateTimeZoneToInt.RemoveAt(10);
	_DateTimeZoneToInt = _DateTimeZoneToInt.Replace(TEXT("."), TEXT(""));
	_DateTimeZoneToInt.Append(FString::FromInt(_Milliseconds));


	if (_FillingNumber > 0)
	{
		_DateTimeZoneToInt.Append(FString::FromInt(_FillingNumber));
	}
	

	



	int32 _a = FMath::RandRange(1, 9);
	int32 _b = FMath::RandRange(0, 9);
	int32 _c = FMath::RandRange(0, 9);

	_DateTimeZoneToInt[0] = FString::FromInt(_a)[0];
	_DateTimeZoneToInt[1] = FString::FromInt(_c)[0];
	_DateTimeZoneToInt.Append(FString::FromInt(_b));

	FString _bS = FString::FromInt(_b);
	

	int64 _DateInInt = FCString::Atoi64(*_DateTimeZoneToInt);


	UE_LOG(LogTemp, Warning, TEXT("DATETIME Zone (Original) : %s"), *_DateTimeZone.ToString());
	UE_LOG(LogTemp, Warning, TEXT("DATETIME Zone (To int)   : %s"), *_DateTimeZoneToInt);
	UE_LOG(LogTemp, Warning, TEXT("DATETIME Zone (In int)   : %lld"), _DateInInt);
	UE_LOG(LogTemp, Warning, TEXT("Milliseconds             : %d"), _DateTimeZone.GetMillisecond());
	UE_LOG(LogTemp, Warning, TEXT("Appending int            : %s"), *FString::FromInt(_b));
	UE_LOG(LogTemp, Warning, TEXT("Filling milli            : %s"), *FString::FromInt(_FillingNumber));

	UE_LOG(LogTemp, Warning, TEXT("Random number generator : %d"), _a);
	UE_LOG(LogTemp, Warning, TEXT("Random number generator : %d"), _b);

	UE_LOG(LogTemp, Warning, TEXT("Size of id : %d"), sizeof(_DateInInt));
}
