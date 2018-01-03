// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Gameframework/Actor.h"

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

	//Find the owner Actor
	Owner = GetOwner();

	//Defining start yaw position of closed door 
	StartingYawPos = Owner->GetActorRotation().Yaw;
}

void UOpenDoor::OpenDoor()
{
	//Set the door rotation     //Create a rotator
	Owner->SetActorRotation(FRotator(0.0f, (StartingYawPos + OpenAngle), 0.0f));
}

void UOpenDoor::CloseDoor()
{
	//Set the door rotation     //Create a rotator
	Owner->SetActorRotation(FRotator(0.0f, StartingYawPos, 0.0f));
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Poll the Trigger Volume
	//if ActorThatOpens is in the Volume
	if (GetTotalMassOfActorsOnPlate() > 30.0f) // TODO make into parameter
	{
		OpenDoor();

		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	
	//check if it's time to close the door

	if (DoorCloseDelay < GetWorld()->GetTimeSeconds() - LastDoorOpenTime)
	{
		CloseDoor();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.0f;

	//Find all the overlapping actors
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	//Iterate through them adding their masses
	for (const auto* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}