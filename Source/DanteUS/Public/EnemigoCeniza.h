// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "EnemigoCeniza.generated.h"

/**
 *
 */
class UNiagaraSystem;

UCLASS()
class DANTEUS_API AEnemigoCeniza : public AEnemyBase
{
	GENERATED_BODY()

public:
	AEnemigoCeniza();
	// Sobrescribimos la función de morir para que ejecute la explosión después de 2 segundos
protected:
	virtual void BeginPlay() override;
	// Sobrescribimos la función de morir para que no desaparezca de inmediato
	virtual void Morir() override;

	// EFECTO DE NIAGARA: Aquí colocas la variable expuesta al editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combate | Efectos")
	UNiagaraSystem* FX_ExplosionCeniza;

};
