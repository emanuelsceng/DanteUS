// Fill out your copyright notice in the Description page of Project Settings.

// Dante: El Último Sello - Nivel 3
// Enemigo común vampírico - Guardián Vampírico
// Hereda de AEnemyBase, por lo que ya tiene:
//   - Salud (20 HP), Estados, Sensor de visión, TakeDamage y Morir
// Diferencias con los otros enemigos comunes:
//   - Es el MÁS FUERTE de los comunes: 10 de daño por golpe
//   - Regenera MÁS vida al atacar: +4 HP por golpe (el doble que los demás)
//   - Es más lento pero muy peligroso si Dante lo ignora

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h" // Clase base de todos los enemigos
#include "Animation/AnimInstance.h"
#include "GuardianVampirico.generated.h"

UCLASS()
class DANTEUS_API AGuardianVampirico : public AEnemyBase
{
	GENERATED_BODY()

public:
	// Constructor: ajustamos los valores del Guardián según el documento
	AGuardianVampirico();

	// --- MECÁNICA VAMPÍRICA MEJORADA ---
	// Según el documento: "+4 HP por golpe" (el doble que Siervo y Acechador)
	// El Guardián es el más peligroso si no se elimina rápido
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante|Enemigo|Vampirico")
	float RegeneracionPorGolpe = 4.0f;

	// En la sección pública:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante|Enemigo|Animaciones")
	UAnimMontage* MontajeMuerte;

protected:
	// Se ejecuta cuando el enemigo aparece en el nivel
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	// Sobrescribimos el ataque para agregar la regeneración vampírica mejorada
	virtual void AtacarJugador() override;
	virtual void Morir() override;
};