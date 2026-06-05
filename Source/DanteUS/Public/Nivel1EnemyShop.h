// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyShop.h"
#include "Nivel1EnemyShop.generated.h"

UCLASS()
class DANTEUS_API ANivel1EnemyShop : public AEnemyShop
{
	GENERATED_BODY()

public:
	ANivel1EnemyShop(const FObjectInitializer& ObjectInitializer);

	// Firma idéntica obligatoria
	virtual AEnemyBase* SpawnEnemy(ERolEnemigo Rol, FVector Posicion, FRotator Rotacion) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Configuracion de Nivel (Guerra)")
	TMap<ERolEnemigo, TSubclassOf<AEnemyBase>> CatalogoGuerra;
};