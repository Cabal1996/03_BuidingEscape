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

	//Find the MyPown Actor
	MyPown = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!MyPown)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't assign MyPown ptr!!!"));
	}

	//Find the owner Actor
	Owner = GetOwner();
	if (!Owner)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't assign Owner ptr!!!"));
	}
	//Checking for Pressure Plate attached to the door
	if (!MyTriggerVolume)
	{
		UE_LOG(LogTemp, Error, TEXT("PressurePlate not assign to %s"), *Owner->GetName());
	}

	SetupInputComponent();
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (OpenByHand)
	{

	}
	else if(OpenByTriggerMass)
	{
		//Poll the Trigger Volume
		//if ActorThatOpens is in the Volume
		if (GetTotalMassOfActorsOnPlate() > TrigerMass) // TODO make into parameter
		{
			OnOpenRequest.Broadcast();
		}
		else
		{
			OnCloseRequest.Broadcast();
		}
	}
	else if(OpenBySpecilObject)
	{
		if (MyTriggerVolume->IsOverlappingActor(SpecialObject))
		{
			OnOpenRequest.Broadcast();
		}
		else
		{
			OnCloseRequest.Broadcast();
		}
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.0f;

	//Find all the overlapping actors
	TArray<AActor*> OverlappingActors;

	if (!MyTriggerVolume) { return 0.0f; }
	MyTriggerVolume->GetOverlappingActors(OUT OverlappingActors);
	
	//Iterate through them adding their masses
	for (const auto* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}

void UOpenDoor::SetupInputComponent()
{
	if (!MyPown) { return; }
	PawnInput = MyPown->FindComponentByClass<UInputComponent>();
	if (PawnInput)
	{
		PawnInput->BindAction("Interact", IE_Pressed, this, &UOpenDoor::Interact);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s has no Input Component attached"), *GetOwner()->GetName());
	}
}

void UOpenDoor::Interact()
{
	if (!MyTriggerVolume) { return; }
	if (!MyPown) { return; }
	if (MyTriggerVolume->IsOverlappingActor(MyPown))
	{
		if (!DoorIsOpen)
		{
			OnOpenRequest.Broadcast();
			DoorIsOpen = true;
		}
		else
		{
			OnCloseRequest.Broadcast();
			DoorIsOpen = false;
		}
	}
	
}