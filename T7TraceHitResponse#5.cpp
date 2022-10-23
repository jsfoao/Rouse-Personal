#include "T7TraceHitResponse.h"

#include "T7AmmoComponent.h"
#include "T7GunInteractable.h"
#include "GP3/Gun/T7FreezeResponse.h"
#include "GP3/Gun/T7LevitateResponse.h"
#include "GP3/Gun/T7StickyResponse.h"

UT7TraceHitResponse::UT7TraceHitResponse()
{
}

void UT7TraceHitResponse::TriggerEnter(UT7TraceShootComponent* TraceShootComponent)
{
	OnHitEnter.Broadcast(TraceShootComponent);
}

void UT7TraceHitResponse::TriggerExit(UT7TraceShootComponent* TraceShootComponent)
{
	OnHitExit.Broadcast(TraceShootComponent);
}

void UT7TraceHitResponse::TriggerStay(UT7TraceShootComponent* TraceShootComponent)
{
	OnHitStay.Broadcast(TraceShootComponent);
}

void UT7TraceHitResponse::TriggerInteract(UT7TraceShootComponent* TraceShootComponent)
{
	// UT7FreezeResponse* FreezeResponse = GetOwner()->FindComponentByClass<UT7FreezeResponse>();
	// UT7LevitateResponse* LevitateResponse = GetOwner()->FindComponentByClass<UT7LevitateResponse>();
	// UT7StickyResponse* StickyResponse = GetOwner()->FindComponentByClass<UT7StickyResponse>();
	// GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0, FColor::Green, FString(TEXT("Triggered interact")));
	// if(FreezeResponse->bIsFrozen == true)
	// {
	// 	FreezeResponse->StopFreeze();
	// }
	// if(LevitateResponse->bIsLevitating == true)
	// {
	// 	LevitateResponse->StopLevitate();
	// }
	// if(StickyResponse->bIsSticky == true)
	// {
	// 	StickyResponse->StopSticky();
	// }
	OnInteract.Broadcast(TraceShootComponent);
}
