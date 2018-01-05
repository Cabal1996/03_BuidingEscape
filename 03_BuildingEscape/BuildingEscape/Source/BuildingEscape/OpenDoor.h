// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/InputComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Runtime/Core/Public/Delegates/Delegate.h"
#include "Engine/TriggerVolume.h"
#include "Engine/World.h"
#include "OpenDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDoorEvent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable)
	FDoorEvent OnOpenRequest;

	UPROPERTY(BlueprintAssignable)
	FDoorEvent OnCloseRequest;

private:

	UInputComponent * PawnInput = nullptr;
	
	UPROPERTY(EditAnywhere)
	ATriggerVolume* MyTriggerVolume = nullptr;

	UPROPERTY(EditAnywhere)
	bool OpenByHand = true;

	UPROPERTY(EditAnywhere)
	bool OpenByTriggerMass = false;

	UPROPERTY(EditAnywhere)
	float TrigerMass = 30.0f;

	UPROPERTY(EditAnywhere)
	bool OpenBySpecilObject = false;

	UPROPERTY(EditAnywhere)
	AActor* SpecialObject = nullptr;

	bool DoorIsOpen = false;

	AActor *Owner = nullptr; // The owning door

	AActor *MyPown = nullptr;

	void SetupInputComponent();
	void Interact();

	float GetTotalMassOfActorsOnPlate();

};