// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grid.generated.h"

UENUM(BlueprintType)
enum EGridType
{
	Cubic,
	Hexagonal,
	Triangular
};

UCLASS()
class GRIDS_API AGrid : public AActor
{
	GENERATED_BODY()

public:
	AGrid();

	/**
	 * TODO: Will be added later
	 * 
	 * UPROPERTY(EditAnywhere, BlueprintReadWrite)
	 * float CapsuleHeight = 0.f;
	 */
};
