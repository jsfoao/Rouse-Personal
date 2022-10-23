#include "T7GunInteractable.h"
#include "T7PushableComponent.h"
#include "Components/BoxComponent.h"
#include "GP3/Library/T7Debug.h"

AT7GunInteractable::AT7GunInteractable()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	RootComponent = BoxComponent;
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	PushableComponent = CreateDefaultSubobject<UT7PushableComponent>(TEXT("PushableComponent"));
}

void AT7GunInteractable::BeginPlay()
{
	Super::BeginPlay();
}

void AT7GunInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}