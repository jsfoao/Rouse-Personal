#include "T7TraceShootComponent.h"
#include "T7TraceHitResponse.h"

UT7TraceShootComponent::UT7TraceShootComponent()
{
	Range = 100.f;
}

void UT7TraceShootComponent::LineTraceShoot(FVector Location, FVector Direction, float LineRange, ECollisionChannel TraceChannel)
{
	FHitResult TraceHit;
	GetWorld()->LineTraceSingleByChannel(TraceHit, Location, Location + (Direction * LineRange), TraceChannel);

	if (TraceHit.bBlockingHit)
	{
		UT7TraceHitResponse* HitResponse = TraceHit.GetActor()->FindComponentByClass<UT7TraceHitResponse>();

		// OnHitEnter
		if (HitResponse != nullptr && HoveredHitResponse == nullptr)
		{
			HoveredHitResponse = HitResponse;
			HoveredHitResponse->TriggerEnter(this);
			bIsHitting = true;
		}
		// OnHitStay
		if (HitResponse != nullptr && HoveredHitResponse != nullptr)
		{
			HoveredHitResponse->TriggerStay(this);
			bIsHitting = true;
		}
		// One object to another without exiting
		if (HitResponse != nullptr && HoveredHitResponse != nullptr && HitResponse != HoveredHitResponse)
		{
			HoveredHitResponse->TriggerExit(this);
			HoveredHitResponse = HitResponse;
			HoveredHitResponse->TriggerEnter(this);
		}
	}
	// OnHitExit
	else if (HoveredHitResponse != nullptr)
	{
		HoveredHitResponse->TriggerExit(this);
		HoveredHitResponse = nullptr;
		bIsHitting = false;
	}
}

void UT7TraceShootComponent::LineTraceShootComponent(USceneComponent* SceneComponent, ECollisionChannel TraceChannel)
{
	LineTraceShoot(SceneComponent->GetComponentLocation(), SceneComponent->GetForwardVector(), Range, TraceChannel);
}

UT7TraceHitResponse* UT7TraceShootComponent::Interact()
{
	if (HoveredHitResponse == nullptr) return nullptr;
	HoveredHitResponse->TriggerInteract(this);
	return HoveredHitResponse;
}
