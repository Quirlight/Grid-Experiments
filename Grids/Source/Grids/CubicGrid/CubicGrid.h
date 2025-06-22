// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CubicGrid.generated.h"

USTRUCT()
struct FSquareCell
{
	GENERATED_BODY()

	FSquareCell() = default;
	FSquareCell(TArray<FVector2D> positions, TArray<int32> indices);

	TArray<FVector> Vertices;
	FVector Normal;
	TArray<FSquareCell*> Neighbors;
};

UCLASS()
class GRIDS_API ACubicGrid : public AActor
{
	GENERATED_BODY()

public:
	ACubicGrid();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	int NumCellsHeight = 1;

	UPROPERTY(EditAnywhere)
	int NumCellsWidth = 1;

	UPROPERTY(EditAnywhere)
	int NumCellsDepth = 1;

protected:
	TArray<FVector> Vertices;
	TArray<int32> Indices;
};
