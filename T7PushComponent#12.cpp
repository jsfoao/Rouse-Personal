#include "T7PushComponent.h"
#include "T7PushableComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GP3/Character/T7Character.h"
#include "GP3/Library/T7Debug.h"
#include "Kismet/KismetMathLibrary.h"

UT7PushComponent::UT7PushComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	LiftDistance = 2.f;
	PushOffset = 10.f;
	PushSpeed = 200.f;
	InterpSpeed = 1.f;
	RotationSpeed = 10.f;
}

void UT7PushComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bIsPushing)
		return;

	if (!bLerpToLocation)
		return;

	GetOwner()->SetActorLocation(UKismetMathLibrary::VInterpTo(GetOwner()->GetActorLocation(), TargetLocation, DeltaTime, InterpSpeed));
	GetOwner()->SetActorRotation(UKismetMathLibrary::RInterpTo(GetOwner()->GetActorRotation(), FRotator(0.f, TargetRotation.Yaw, 0.f), DeltaTime, RotationSpeed));
	Timer -= DeltaTime;
	if(Timer <= 0)
	{
		Timer = DefaultTime;
		DirectionToPushable = Pushable->GetOwner()->GetActorLocation() - GetOwner()->GetActorLocation();
		bLerpToLocation = false;
		Cast<AT7Character>(GetOwner())->bLockMovement = false;
	}
}


void UT7PushComponent::TryPush(AActor* ActorToPush)
{
	// Not interacting with valid actor
	if (ActorToPush == nullptr) return;
	UT7PushableComponent* PushableComp = ActorToPush->FindComponentByClass<UT7PushableComponent>();
	// Actor isn't pushable
	if (PushableComp == nullptr) return;
	
	Pushable = PushableComp;
	// Check if pushable is grounded
	if (!Pushable->bIsGrounded)
	{
		LOG("Can't push, object isn't grounded...");
		return;
	}
	// Evaluate which side is being pushed
	bIsOverlapping = Pushable->EvaluateOverlap(GetOwner());
	if (!bIsOverlapping)
	{
		LOG("Can't push, not overlapping pushable...")
		return;
	}
	if (!Pushable->bCanPush)
	{
		LOG("Can't push, object isn't pushable...")
		return;
	}
	
	// Can push...
	bIsPushing = true;

	LOG("Start push!");
	// Move pushable away from actor
	bLerpToLocation = true;
	Cast<AT7Character>(GetOwner())->bLockMovement = true;

	// Target location and rotation
	TargetLocation = GetOwner()->GetActorLocation() + Pushable->BoxSide * PushOffset;
	TargetRotation = UKismetMathLibrary::MakeRotFromX(-Pushable->BoxSide);
	
	// Calculate direction to pushable
	PushableLocationOnStart = Pushable->GetOwner()->GetActorLocation();
	
	// Disable physics
	Pushable->PrimitiveComponent->SetSimulatePhysics(false);

	if (Pushable->DragMultiplier != 0.f)
	{
		// Raise from ground
		Pushable->GetOwner()->AddActorWorldOffset(FVector::UpVector * LiftDistance);
	}
	
	OnStartPush.Broadcast(Pushable);
}

void UT7PushComponent::StopPush()
{
	if (!bIsPushing || Pushable == nullptr)
	{
		LOG("Player isn't pushing to stop...")
		return;
	}
	LOG("Stop push!")
	
	bIsPushing = false;
	DirectionToPushable = FVector::ZeroVector;
	Pushable->PrimitiveComponent->SetSimulatePhysics(true);
	bLerpToLocation = false;
	Cast<AT7Character>(GetOwner())->bLockMovement = false;

	OnStopPush.Broadcast(Pushable);
	Pushable = nullptr;
}

void UT7PushComponent::MovePushable()
{
	if (!bIsPushing || Pushable == nullptr)
	{
		return;
	}
	if (!Pushable->bIsGrounded)
	{
		StopPush();
		LOG("Stop push: not grounded...")
		return;
	}
	if (!Pushable->IsOverlappingOuter(GetOwner()))
	{
		StopPush();
		LOG("Stop push: not overlapping anymore...");
		return;
	}
	
	// Move pushable
	if (!bLerpToLocation)
	{
		FVector NewLocation = GetOwner()->GetActorLocation() + DirectionToPushable;
		NewLocation.Z = PushableLocationOnStart.Z + LiftDistance;
		Pushable->GetOwner()->SetActorLocation(NewLocation, true);
	}
	Pushable->SetHandIKLocation(this);
}


