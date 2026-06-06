// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyShop.h" 
#include "Nivel3EnemyShop.generated.h"

UCLASS()
class DANTEUS_API ANivel3EnemyShop : public AEnemyShop
{
	GENERATED_BODY()

public:
	// Cambiamos al constructor explícito para sincronizar el enlazador de Unreal
	ANivel3EnemyShop(const FObjectInitializer& ObjectInitializer);

	// Aseguramos que la firma coincida al 100% con la clase padre
	virtual AEnemyBase* SpawnEnemy(ERolEnemigo Rol, FVector Posicion, FRotator Rotacion) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Configuracion de Nivel (Hambre)")
	TMap<ERolEnemigo, TSubclassOf<AEnemyBase>> CatalogoVampiros;
};