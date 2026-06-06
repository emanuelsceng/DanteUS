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
	EnemigoGComun1         UMETA(DisplayName = "Ceniza"),
	GuerraMiniBoss       UMETA(DisplayName = "Comandante"),


	EnemigoPComun1       UMETA(DisplayName = "Escupidor"),
	EnemigoPComun2	   UMETA(DisplayName = "Pustulento"),
	PesteMiniBoss        UMETA(DisplayName = "MiniBossPeste"),

	EnemigoHComun1       UMETA(DisplayName = "Siervo"),
	EnemigoHComun2      UMETA(DisplayName = "Guardian"),
	EnemigoHComun3    UMETA(DisplayName = "Acechador"),
	MiniBossHambre        UMETA(DisplayName = "MiniBosshambre"),


	EnemigoMComun1      UMETA(DisplayName = "Fantasma"),
	MuerteMiniBoss       UMETA(DisplayName = "Recolector"),
};

UCLASS() 
class DANTEUS_API AEnemyShop : public AActor
{
	GENERATED_BODY()

public:
	AEnemyShop(const FObjectInitializer& ObjectInitializer);

		// Las subclases por nivel implementarán obligatoriamente este método.
	virtual AEnemyBase* SpawnEnemy(ERolEnemigo Rol, FVector Posicion, FRotator Rotacion) 
	PURE_VIRTUAL(AEnemyShop::SpawnEnemy, return nullptr;);
};