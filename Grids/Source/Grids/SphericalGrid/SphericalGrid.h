
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SphericalGrid.generated.h"

USTRUCT(Blueprintable)
struct FCircle
{
	GENERATED_BODY()

	FCircle() {}
	FCircle(const FVector InCenter, const float InRadius) : Center(InCenter), Radius(InRadius) {}

#if WITH_EDITOR
	void DebugDraw(const UWorld* InWorld) const;
#endif

	void CreateEquidistantPoints(int NumOfPoints);

protected:
	UPROPERTY(BlueprintReadOnly)
	FVector Center = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly)
	float Radius = 1.f;

	UPROPERTY(BlueprintReadOnly)
	TArray<FVector> CellCorners;
};

USTRUCT()
struct FCell
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TArray<FVector> Corners;
};

UCLASS()
class GRIDS_API ASphericalGrid : public AActor
{
	GENERATED_BODY()

public:
	ASphericalGrid();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void FitToMesh();

protected:
	/**
	 * Defines the radius the outermost circle(s) will have.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Radius = 1.f;

	/**
	 * Number of circles that will be created,
	 * with the largest having a radius of @ref Radius.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (UIMin = 1, UIMax = 50))
	int NumberOfCirclesOnPlane = 10;

	/**
	 * Number of circles that will be created at the side of the cylinder,
	 * between the both outermost circles of the top and bottom plane.
	 * This will only have an effect of the @ref Height isn't zero!
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int NumberOfCirclesOnSide = 1.f;

	/**
	 * Defines the height of the cylinder.
	 * If it's zero the grid will only be a plane.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (UIMin = 0.f))
	float Height = 0.f;

	/**
	 * Number of Cells every donut of the grid will have.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int NumOfCells = 8;

private:
	UPROPERTY(VisibleAnywhere)
	TArray<FCircle> Circles;

	void CreateCirclesOnCylinder();
};
