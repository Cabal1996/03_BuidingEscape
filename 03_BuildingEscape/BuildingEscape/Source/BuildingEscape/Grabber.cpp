// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"

#define OUT
// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	//Look for attached Physics Handle
	FindPhysicsHandleComponent();

	//Look for attached Input Component
	SetupInputComponent();
}

void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHadle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHadle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has no Physics Handel Component attached"), *GetOwner()->GetName());
	}
}

void UGrabber::SetupInputComponent()
{
	PawnInput = GetOwner()->FindComponentByClass<UInputComponent>();
	if (PawnInput)
	{
		PawnInput->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		PawnInput->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s has no Input Component attached"), *GetOwner()->GetName());
	}
}

void UGrabber::Grab()
{
	//LINE TRACE and see if we reach any actor with physics body collision channel set
	auto HitResault = GetFirstPhysicBodyInReach();
	auto ComponentToGrab = HitResault.GetComponent();
	auto ActorHit = HitResault.GetActor();

	//if we hit something then attach a physics handle
	if (ActorHit)
	{
		if (!PhysicsHadle) { return; }
		//attach physics handle
		PhysicsHadle->GrabComponentAtLocationWithRotation(
			ComponentToGrab,
			NAME_None, // no bone required
			ComponentToGrab->GetOwner()->GetActorLocation(),
			ComponentToGrab->GetOwner()->GetActorRotation()
		);
	}
}

void UGrabber::Release()
{
	//release physics handle
	if (!PhysicsHadle) { return; }
	PhysicsHadle->ReleaseComponent();
}

FHitResult UGrabber::GetFirstPhysicBodyInReach()
{
	//Setup query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	///Line-Trace (Ray-cast) out to reach distance
	FHitResult HitResault;
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResault,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);
	return HitResault;
}

FVector UGrabber::GetReachLineStart()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
	return PlayerViewPointLocation;
}

FVector UGrabber::GetReachLineEnd()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Rach;
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//if the physics is attached
	if (!PhysicsHadle) { return; }
	if (PhysicsHadle->GetGrabbedComponent())
	{
		//move the object we are holding
		PhysicsHadle->SetTargetLocation(GetReachLineEnd());
	}
}
