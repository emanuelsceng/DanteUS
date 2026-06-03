// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"

AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	ZonaActivacion = CreateDefaultSubobject<UBoxComponent>(TEXT("ZonaActivacion"));
	RootComponent = ZonaActivacion;
	ZonaActivacion->SetBoxExtent(FVector(200.0f, 200.0f, 100.0f));
}

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	ZonaActivacion->OnComponentBeginOverlap.AddDynamic(this, &AEnemySpawner::AlEntrarEnZona);
}

void AEnemySpawner::AlEntrarEnZona(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Detectamos al Jugador (Dante) mediante tu Tag original "Player"
	if (!bYaSeActivo && OtherActor && OtherActor->ActorHasTag("Player"))
	{
		if (TiendaLocal)
		{
			bYaSeActivo = true;

			for (int32 i = 0; i < Cantidad; i++)
			{
				// Tu fórmula original de posicionamiento en fila
				FVector SpawnPos = GetActorLocation() + FVector(i * 150.0f, 0.0f, 50.0f);

				// LLAMADA AL FACTORY METHOD: Le pedimos el rol a la tienda instanciada
				TiendaLocal->SpawnEnemy(RolA_Spawnear, SpawnPos, GetActorRotation());
			}

			this->Destroy();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("EnemySpawner: ¡Peligro! No has asignado la TiendaLocal en este Spawner del mapa."));
		}
	}
}