// Fill out your copyright notice in the Description page of Project Settings.


#include "Nivel3EnemyShop.h"
#include "EnemyBase.h"

// Implementamos el constructor usando la macro e inicializador nativos de Unreal
ANivel3EnemyShop::ANivel3EnemyShop(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
}

AEnemyBase* ANivel3EnemyShop::SpawnEnemy(ERolEnemigo Rol, FVector Posicion, FRotator Rotacion, AActor* InOwner)
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