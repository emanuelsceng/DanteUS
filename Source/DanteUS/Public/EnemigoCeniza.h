// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "EnemigoCeniza.generated.h"

/**
 * 
 */
UCLASS()
class DANTEUS_API AEnemigoCeniza : public AEnemyBase
{
	GENERATED_BODY()
	
public:
	AEnemigoCeniza();
	// Sobrescribimos la función de morir para que ejecute la explosión después de 2 segundos
protected:
	// Sobrescribimos la función de morir para que no desaparezca de inmediato
	virtual void Morir() override;

	// Función que ejecutará la explosión final
	void EjecutarExplosion();

	// Timer para manejar los 2 segundos de espera
	FTimerHandle TemporizadorExplosion;
	
};
