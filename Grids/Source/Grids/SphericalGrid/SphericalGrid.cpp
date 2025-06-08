
#include "SphericalGrid.h"

#if WITH_EDITOR
void FCircle::DebugDraw(const UWorld* InWorld) const
	{
		DrawDebugCircle(InWorld, Center, Radius * 100.f, 50, FColor::Blue, true, -1, 0, 5.f);
	}
#endif

ASphericalGrid::ASphericalGrid()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASphericalGrid::BeginPlay()
{
	for (int i = 1; i <= NumberOfCircles; ++i)
	{
		Circles.Add(FCircle(GetActorLocation(), i));
		Circles.Add(FCircle(GetActorLocation() + FVector(MaxHeight, 0, 0), i));
	}

	for (int i = 100.f; i < MaxHeight; i = i + 100.f)
	{
		Circles.Add(FCircle(GetActorLocation() + FVector(i, 0, 0), NumberOfCircles));
	};

#if WITH_EDITOR
	for (FCircle& Circle : Circles)
	{
		Circle.DebugDraw(GetWorld());
	}
#endif
}

