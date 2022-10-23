#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "T7GunInteractable.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UT7PushableComponent;
UCLASS()
class GP3_API AT7GunInteractable : public AActor
{
	GENERATED_BODY()

public:
	AT7GunInteractable();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;


public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UT7PushableComponent* PushableComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bPlayerOverlapping;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector BoxSide;
};
