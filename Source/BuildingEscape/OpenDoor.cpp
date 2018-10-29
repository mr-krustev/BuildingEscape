// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
	if (!PressurePlate) 
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Missing trigger volume pressure plate."), *(Owner->GetName()))
	}
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PressurePlate) { return; }
	// Poll TriggerVolume
	if (GetTotalMassOfActorsOnPlate() > TriggerMass) {
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	else if (GetWorld()->GetTimeSeconds() >= (LastDoorOpenTime + DoorCloseDelay)){
		CloseDoor();
	}

}

void UOpenDoor::OpenDoor()
{
	Owner->SetActorRotation(FRotator{ 0.0f, OpenAngle, 0.0f });
}

void UOpenDoor::CloseDoor()
{
	Owner->SetActorRotation(FRotator{ 0.0f, 0.0f, 0.0f });
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;

	if (!PressurePlate) { return TotalMass; }
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	// TODO: Add class filter.
	for (const auto& OverlappingActor : OverlappingActors) 
	{
		TotalMass += OverlappingActor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	return TotalMass;
}