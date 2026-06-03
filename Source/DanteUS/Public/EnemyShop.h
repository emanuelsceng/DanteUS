// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyBase.h" // Conectamos con tu base real que tiene la salud, el sensor, etc.
#include "EnemyShop.generated.h"

// El spawner pedirá un rol abstracto, nunca el nombre de un enemigo específico.
UENUM(BlueprintType)
enum class ERolEnemigo : uint8
{
	EnemigoComun1       UMETA(DisplayName = "Siervo"),
	EnemigoComun2      UMETA(DisplayName = "Guardian"),
	EnemigoComun3    UMETA(DisplayName = "Acechador"),
	MiniBossHambre        UMETA(DisplayName = "MiniBosshambre")
};

UCLASS() 
class DANTEUS_API AEnemyShop : public AActor
{
	GENERATED_BODY()

public:
	AEnemyShop(const FObjectInitializer& ObjectInitializer);

	// EL FACTORY METHOD VIRTUAL PURO (= 0)
	// Las subclases por nivel (Nivel 3, Nivel 1) implementarán obligatoriamente este método.
	virtual AEnemyBase* SpawnEnemy(ERolEnemigo Rol, FVector Posicion, FRotator Rotacion) 
	PURE_VIRTUAL(AEnemyShop::SpawnEnemy, return nullptr;);
};