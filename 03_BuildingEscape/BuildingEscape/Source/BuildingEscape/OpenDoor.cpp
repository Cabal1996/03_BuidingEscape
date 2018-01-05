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

	Init();
	SetupInputComponent();
}

void UOpenDoor::Init()
{
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
		UE_LOG(LogTemp, Error, TEXT("Trigger Volume not assign to %s"), *Owner->GetName());
	}

	if (TypeOfDoor == ETypeOfDoor::OpenBySpecilObject)
	{
		if (!SpecialObject)
		{
			UE_LOG(LogTemp, Error, TEXT("Special Object not assign to %s"), *Owner->GetName());
		}
	}
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

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	if (TypeOfDoor == ETypeOfDoor::OpenByTriggerMass)
	{
		MassTrigger();
	}
	if (TypeOfDoor == ETypeOfDoor::OpenBySpecilObject)
	{
		SpecaialObgectTrigger();
	}
}

//Trigger Door Open/Close by Left Mouse Button of player
void UOpenDoor::Interact()
{
	if (TypeOfDoor == ETypeOfDoor::OpenByHand)
	{
		if (!MyTriggerVolume) { return; }
		if (!MyPown) { return; }
		if (MyTriggerVolume->IsOverlappingActor(MyPown))
		{
			DoorAct(!BDoorIsOpen);
		}
	}
}

// Trigger Door Open/Close by Mass in trigger volume
void UOpenDoor::MassTrigger()
{

	if (GetTotalMassOfActorsOnPlate() > TrigerMass)
	{
		DoorAct(true);
	}
	else
	{
		DoorAct(false);
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

// Trigger Door Open/Close by special object overlaps trigger volume
void UOpenDoor::SpecaialObgectTrigger()
{
	if (!MyTriggerVolume) { return; }
	if (!SpecialObject) { return; }
	if (MyTriggerVolume->IsOverlappingActor(SpecialObject))
	{
		DoorAct(true);
	}
	else
	{
		DoorAct(false);
	}
}

//Force door to act
//open on "TRUE" close on "FALSE"
void UOpenDoor::DoorAct(bool BOpenClose)
{
	if (BOpenClose)
	{
		if (!BDoorIsOpen)
		{
			OnOpenRequest.Broadcast();//Broadcasting element to blueprint
			BDoorIsOpen = true;
		}
	}
	else
	{
		if (BDoorIsOpen)
		{
			OnCloseRequest.Broadcast();//Broadcasting element to blueprint
			BDoorIsOpen = false;
		}

	}
}