// Fill out your copyright notice in the Description page of Project Settings.


#include "Nivel4EnemyShop.h"
#include "EnemyBase.h"

ANivel4EnemyShop::ANivel4EnemyShop(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
}

AEnemyBase* ANivel4EnemyShop::SpawnEnemy(ERolEnemigo Rol, FVector Posicion, FRotator Rotacion,
	AActor* InOwner)
{
	UWorld* Mundo = GetWorld();
	if (!Mundo) return nullptr;

	if (CatalogoMuerte.Contains(Rol))
	{
		TSubclassOf<AEnemyBase> ClaseA_Spawnear = CatalogoMuerte[Rol];
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
