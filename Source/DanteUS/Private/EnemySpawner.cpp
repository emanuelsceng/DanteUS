// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "EnemyShop.h"
#include "EnemyBase.h"    // <-- Añádelo aquí para la lógica de conteo de enemigos

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
	if (!bYaSeActivo && OtherActor && OtherActor->ActorHasTag("Player"))
	{
		if (TiendaLocal)
		{
			bYaSeActivo = true;

			for (int32 i = 0; i < Cantidad; i++)
			{
				
				FVector SpawnPos = GetActorLocation() + FVector(i * 150.0f, 0.0f, 50.0f);

				// LLAMADA AL FACTORY METHOD
				TiendaLocal->SpawnEnemy(RolA_Spawnear, SpawnPos, GetActorRotation());
			}
			EnemigosVivos = Cantidad; //D
			//this->Destroy(); quitamos el destroy para poder llevar la cuenta, D
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("EnemySpawner: ¡Peligro! No has asignado la TiendaLocal en este Spawner del mapa."));
		}
	}
}

//agrega la función NotificarEnemyMuerto, Domingo
void AEnemySpawner::NotificarEnemyMuerto()
{
	EnemigosVivos = FMath::Max(0, EnemigosVivos - 1);

	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange,
		FString::Printf(TEXT("Spawner: Enemigos restantes: %d"), EnemigosVivos));

	if (EnemigosVivos == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
			TEXT("Spawner: Arena limpia"));

		OnArenaLimpia.Broadcast();
		Destroy();
	}
}