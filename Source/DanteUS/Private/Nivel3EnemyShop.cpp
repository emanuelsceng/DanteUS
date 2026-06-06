// Fill out your copyright notice in the Description page of Project Settings.


#include "Nivel3EnemyShop.h"

// Implementamos el constructor usando la macro e inicializador nativos de Unreal
ANivel3EnemyShop::ANivel3EnemyShop(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
}

AEnemyBase* ANivel3EnemyShop::SpawnEnemy(ERolEnemigo Rol, FVector Posicion, FRotator Rotacion)
{
	UWorld* Mundo = GetWorld();
	if (!Mundo) return nullptr;

	if (CatalogoVampiros.Contains(Rol))
	{
		TSubclassOf<AEnemyBase> ClaseA_Spawnear = CatalogoVampiros[Rol];

		if (ClaseA_Spawnear)
		{
			FActorSpawnParameters Params;
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			return Mundo->SpawnActor<AEnemyBase>(ClaseA_Spawnear, Posicion, Rotacion, Params);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Nivel3EnemyShop: El rol solicitado no tiene un Blueprint asignado."));
	return nullptr;
}