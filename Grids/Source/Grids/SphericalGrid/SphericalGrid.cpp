
#include "SphericalGrid.h"

#include "UDynamicMesh.h"
#include "DynamicMesh/DynamicMesh3.h"
#include "GeometryScript/MeshSpatialFunctions.h"

namespace my_namespace
{
	void CreateDynamicMesh(const TArray<FVector3f>& InVertices, const TArray<uint32>& InIndices, UE::Geometry::FDynamicMesh3& OutMesh)
	{
		OutMesh.Clear();
		for (const FVector3f& Vertex : InVertices)
		{
			OutMesh.AppendVertex(FVector3d(Vertex));
		}

		check(InIndices.Num() % 3 == 0);
		const int32 NumTriangles = InIndices.Num() / 3;
		for (int32 TriangleID = 0; TriangleID < NumTriangles; ++TriangleID)
		{
			const UE::Geometry::FIndex3i Triangle(InIndices[3 * TriangleID], InIndices[3 * TriangleID + 1], InIndices[3 * TriangleID + 2]);
			OutMesh.AppendTriangle(Triangle);
		}
	}
}

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
	for (FCircle& Circle : Circles)
	{
		for (FVector& Point : Circle.CellCorners)
		{
			FitPointToMesh(Point);
		}
	}
}

void ASphericalGrid::FitPointToMesh(FVector& Point)
{
	UE::Geometry::FDynamicMesh3 DynamicMesh;
	TArray<FVector3f> Vertices;
	TArray<uint32> Indices;
	my_namespace::CreateDynamicMesh(Vertices, Indices, DynamicMesh);

	TObjectPtr<UDynamicMesh> TargetMesh = NewObject<UDynamicMesh>();
	TargetMesh->SetMesh(DynamicMesh);

	FGeometryScriptDynamicMeshBVH Bvh;
	UGeometryScriptLibrary_MeshSpatial::BuildBVHForMesh(TargetMesh, Bvh);

	FGeometryScriptSpatialQueryOptions Options;
	Options.MaxDistance = 1000.f;

	FGeometryScriptTrianglePoint NearestResult;
	EGeometryScriptSearchOutcomePins Result;
	UGeometryScriptLibrary_MeshSpatial::FindNearestPointOnMesh(
		TargetMesh, Bvh, Point, Options, NearestResult, Result);

	Point = NearestResult.Position;
}


