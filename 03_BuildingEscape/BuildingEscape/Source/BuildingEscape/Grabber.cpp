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

	// ...
	UE_LOG(LogTemp, Warning, TEXT("Grabber on %s is reporting for duty!"), *GetOwner()->GetName());

	///Look for attached Physics Handle
	PhysicsHadle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHadle)
	{

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s has no Physics Handel Component attached"), *GetOwner()->GetName());
	}

	///Look for attached Input Component
	PawnInput = GetOwner()->FindComponentByClass<UInputComponent>();
	if (PawnInput)
	{
		UE_LOG(LogTemp, Warning, TEXT("Input Component found"));
		///Bind the input axis
		PawnInput->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s has no Input Component attached"), *GetOwner()->GetName());
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed"));
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	///Get player view this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Rach;
	
	/*
	//test output
	FString Loaction = PlayerViewPointLocation.ToString();
	FString Rotation = PlayerViewPointRotation.ToString();
	UE_LOG(LogTemp, Warning, TEXT("Player Location is: %s ; Player rotation is: %s"), *Loaction, *Rotation);
	*/
	

	///Draw a rad trace in the world to visualize
	DrawDebugLine(
		GetWorld(), PlayerViewPointLocation, LineTraceEnd, FColor(255, 0, 0), false, 0.0f, 0.0f, 5.0f
		);

	///Setup query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	///Line-Trace (Ray-cast) out to reach distance
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	///See what we hit
	AActor* ActorHit = Hit.GetActor();

	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit at: %s"), *ActorHit->GetName());
	}


}


