// Fill out your copyright notice in the Description page of Project Settings.


#include "Nivel1EnemyShop.h"

ANivel1EnemyShop::ANivel1EnemyShop(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
}

AEnemyBase* ANivel1EnemyShop::SpawnEnemy(ERolEnemigo Rol, FVector Posicion, FRotator Rotacion)
{
	UWorld* Mundo = GetWorld();
	if (!Mundo) return nullptr;

	if (CatalogoGuerra.Contains(Rol))
	{
		TSubclassOf<AEnemyBase> ClaseA_Spawnear = CatalogoGuerra[Rol];
		if (ClaseA_Spawnear)
		{
			FActorSpawnParameters Params;
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			return Mundo->SpawnActor<AEnemyBase>(ClaseA_Spawnear, Posicion, Rotacion, Params);
		}
	}
	return nullptr;
}