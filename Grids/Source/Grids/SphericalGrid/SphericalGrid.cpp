
#include "SphericalGrid.h"

#if WITH_EDITOR
void FCircle::DebugDraw(const UWorld* InWorld) const
{
	DrawDebugCircle(InWorld, Center, Radius, 40, FColor::Blue, true, -1, 0,
		2.f, FVector(1, 0, 0), FVector(0, 1, 0), false);

	for (FVector Corner : CellCorners)
	{
		DrawDebugPoint(InWorld, Corner, 10, FColor::Red, true, -1, 0);
	}
}
#endif

void FCircle::CreateEquidistantPoints(int NumOfPoints)
{
	// Num of point == Num of Cells
	constexpr float DegreeOfACircle = 360.f;
	float Angle = DegreeOfACircle / static_cast<float>(NumOfPoints);

	for (int i = 0; i < NumOfPoints; i++)
	{
		float CurrentAngleInRad = Angle * i * (PI / 180.f);
		float LengthAdjacentSide = FMath::Cos(CurrentAngleInRad);
		float LengthOppositeSide = FMath::Sin(CurrentAngleInRad);
		FVector NewPoint = FVector::ForwardVector * LengthAdjacentSide +
						   FVector::RightVector * LengthOppositeSide;

		NewPoint *= Radius;
		NewPoint += Center;

		CellCorners.Add(NewPoint);
	}
}

ASphericalGrid::ASphericalGrid()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ASphericalGrid::CreateCirclesOnCylinder()
{
	const float DonutWidth = Radius / static_cast<float>(NumberOfCirclesOnPlane);
	FVector Offset = FVector(0, 0, 0);

	if (Height > 0.f)
	{
		Offset.Z = Height / 2.f;
	}
	FVector MidPoint = GetActorLocation() - Offset;

	for (float i = DonutWidth; i <= Radius; i+= DonutWidth)
	{
		Circles.Add(FCircle(MidPoint, i));
	}

	if (Height > 0.f)
	{
		const float DonutHeight = Height / static_cast<float>(NumberOfCirclesOnSide + 1);
		for (float i = DonutHeight; i < Height; i+= DonutHeight)
		{
			FVector TemporaryOffset = FVector(0, 0, i);
			Circles.Add(FCircle(MidPoint + TemporaryOffset, Radius));
		}

		MidPoint = GetActorLocation() + Offset;
		for (float i = DonutWidth; i <= Radius; i+= DonutWidth)
		{
			Circles.Add(FCircle(MidPoint, i));
		}
	}
}

void ASphericalGrid::BeginPlay()
{
	CreateCirclesOnCylinder();

	for (FCircle& Circle : Circles)
	{
		Circle.CreateEquidistantPoints(NumOfCells);
	}

#if WITH_EDITOR
	for (FCircle& Circle : Circles)
	{
		Circle.DebugDraw(GetWorld());
	}
#endif
}

void ASphericalGrid::FitToMesh()
{
	//TODO
}

