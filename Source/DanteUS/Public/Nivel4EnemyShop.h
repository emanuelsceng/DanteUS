// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyShop.h"
#include "EnemyBase.h"
#include "Nivel4EnemyShop.generated.h"

UCLASS()
class DANTEUS_API ANivel4EnemyShop : public AEnemyShop
{
	GENERATED_BODY()

public:
	ANivel4EnemyShop(const FObjectInitializer& ObjectInitializer);

	virtual AEnemyBase* SpawnEnemy(ERolEnemigo Rol, FVector Posicion, FRotator Rotacion,
		AActor* InOwner = nullptr) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Configuracion de Nivel (Muerte)")
	TMap<ERolEnemigo, TSubclassOf<AEnemyBase>> CatalogoMuerte;
};