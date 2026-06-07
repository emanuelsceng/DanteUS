// Fill out your copyright notice in the Description page of Project Settings.


#include "Nivel2EnemyShop.h"
#include "EnemyBase.h"

ANivel2EnemyShop::ANivel2EnemyShop(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
}

AEnemyBase* ANivel2EnemyShop::SpawnEnemy(ERolEnemigo Rol, FVector Posicion, FRotator Rotacion,
	AActor* InOwner)
{
	UWorld* Mundo = GetWorld();
	if (!Mundo) return nullptr;

	if (CatalogoPeste.Contains(Rol))
	{
		TSubclassOf<AEnemyBase> ClaseA_Spawnear = CatalogoPeste[Rol];
		if (ClaseA_Spawnear)
		{
			FActorSpawnParameters Params;
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			Params.Owner = Owner;
			return Mundo->SpawnActor<AEnemyBase>(ClaseA_Spawnear, Posicion, Rotacion, Params);
		}
	}
	return nullptr;
}
