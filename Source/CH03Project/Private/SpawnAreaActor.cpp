#include "SpawnAreaActor.h"


ASpawnAreaActor::ASpawnAreaActor()
{
	SpawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea"));
	RootComponent = SpawnArea;
}

