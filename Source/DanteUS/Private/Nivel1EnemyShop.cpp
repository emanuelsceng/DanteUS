// Fill out your copyright notice in the Description page of Project Settings.


#include "Nivel1EnemyShop.h"
#include "EnemyBase.h"

ANivel1EnemyShop::ANivel1EnemyShop(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
}

AEnemyBase* ANivel1EnemyShop::SpawnEnemy(ERolEnemigo Rol, FVector Posicion, FRotator Rotacion, AActor* InOwner)
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

			// 1. Spawneamos al enemigo
			AEnemyBase* NuevoEnemigo = Mundo->SpawnActor<AEnemyBase>(ClaseA_Spawnear, Posicion, Rotacion, Params);

			// 2. FORZAMOS EL OWNER EXPLÍCITAMENTE (El truco salvavidas)
			if (NuevoEnemigo && InOwner)
			{
				NuevoEnemigo->SetOwner(InOwner);
			}

			return NuevoEnemigo;
		}
	}
	return nullptr;
}