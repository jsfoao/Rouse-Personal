#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "T7InteractableResponse.generated.h"

UCLASS( ClassGroup="Puzzle", meta=(BlueprintSpawnableComponent) )
class GP3_API UT7InteractableResponse : public UActorComponent
{
	GENERATED_BODY()

public:	
	UT7InteractableResponse();

public:	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActivate);
	UPROPERTY(BlueprintAssignable)
	FOnActivate OnActivate;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeactivate);
	UPROPERTY(BlueprintAssignable)
	FOnDeactivate OnDeactivate;
};
