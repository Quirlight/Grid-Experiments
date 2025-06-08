
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

protected:
	UPROPERTY(BlueprintReadOnly)
	FVector Center = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly)
	float Radius = 1.f;
};

UCLASS()
class GRIDS_API ASphericalGrid : public AActor
{
	GENERATED_BODY()

public:
	ASphericalGrid();

	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (UIMin = 1, UIMax = 50))
	float NumberOfCircles = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (UIMin = 1))
	float MaxHeight = 500.f;

private:
	UPROPERTY(VisibleAnywhere)
	TArray<FCircle> Circles;
};
