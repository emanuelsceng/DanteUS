// Fill out your copyright notice in the Description page of Project Settings.

// Dante: El Último Sello - Nivel 3
// Enemigo común vampírico - Acechador Sanguinario
// Hereda de AEnemyBase, por lo que ya tiene:
//   - Salud (20 HP), Estados, Sensor de visión, TakeDamage y Morir
// Diferencias con el Siervo:
//   - Hace MENOS daño (3 por golpe) pero es más ágil
//   - También regenera vida al atacar (+2 HP por golpe)


#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "AcechadorSanguinario.generated.h"

UCLASS()
class DANTEUS_API AAcechadorSanguinario : public AEnemyBase
{
    GENERATED_BODY()

public:
    AAcechadorSanguinario();

    // Regenera +2 HP por golpe SOLO si el golpe conecta fisicamente
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante|Enemigo|Vampirico")
    float RegeneracionPorGolpe = 2.0f;

protected:
    virtual void BeginPlay() override;

public:
    // Sobrescribimos EjecutarGolpeMelee para curar SOLO si el golpe conecta
    virtual void EjecutarGolpeMelee() override;
};
