// Fill out your copyright notice in the Description page of Project Settings.

// Dante: El Último Sello - Nivel 3
// Enemigo común vampírico - Siervo
// Hereda de AEnemyBase, por lo que ya tiene:
//   - Salud (20 HP), Daño (5), Estados (Inactivo/Persiguiendo/Atacando/Muerto)
//   - Sensor de visión para detectar a Dante
//   - Sistema de TakeDamage y Morir
// Lo único que agregamos aquí es la REGENERACIÓN al atacar

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h" // Incluimos la clase base de todos los enemigos
#include "SiervoVampirico.generated.h"

UCLASS()
class DANTEUS_API ASiervoVampirico : public AEnemyBase
{
	GENERATED_BODY()

public:
	// Constructor: aquí ajustamos los valores base heredados de EnemyBase
	// para que coincidan con el Siervo Vampírico del documento
	ASiervoVampirico();

	// --- MECÁNICA VAMPÍRICA ---
	// Según el documento: "+2 HP por golpe"
	// Cada vez que el Siervo golpea a Dante, se cura esta cantidad
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante|Enemigo|Vampirico")
	float RegeneracionPorGolpe = 2.0f;

protected:
	// Se ejecuta cuando el enemigo aparece en el nivel
	virtual void BeginPlay() override;

public:
	// Sobrescribimos el método de ataque heredado de EnemyBase
	// para agregar la lógica de regeneración vampírica después de golpear
	virtual void AtacarJugador() override;
};