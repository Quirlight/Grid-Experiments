
#include "CubicGrid.h"


ACubicGrid::ACubicGrid()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ACubicGrid::BeginPlay()
{
	
}

// Called every frame
void ACubicGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

