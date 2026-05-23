// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "MiniBossGuerra.generated.h"

/**
 * 
 */
UCLASS()
class DANTEUS_API AMiniBossGuerra : public AEnemyBase
{
	GENERATED_BODY()

public:
	
	AMiniBossGuerra();
protected:
	// ATRIBUTOS DE COMBATE (Encapsulamiento)
	// Lleva la cuenta de cuántos golpes normales ha dado
	int32 ContadorAtaques;

	// El tiempo que el jefe se queda quieto tras la onda de choque
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combate | Jefe")
	float TiempoRecuperacion;

	// Bandera para saber si el jefe está en estado de "enfriamiento"
	bool bEstaRecuperandose;
	// MÉTODOS DE COMPORTAMIENTO (Polimorfismo)

	// Polimorfismo: Sobreescribimos el método de ataque de EnemyBase
	virtual void AtacarJugador() override;

	//Guardará las coordenadas exactas donde el jefe golpeó el suelo
	FVector CentroDeExplosion;

	// --- EFECTOS DE NIAGARA PARA EL ATAQUE ESPECIAL ---

	// El círculo rúnico rojo que servirá de advertencia/cráter inicial (0 segundos)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combate | Jefe | VFX")
	class UNiagaraSystem* FX_CirculoAdvertencia;

	// La gran explosión de fuego final (a los 2 segundos)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combate | Jefe | VFX")
	class UNiagaraSystem* FX_ExplosionFinal;

	// Animación del salto y golpe en el suelo
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combate | Jefe")
	class UAnimMontage* MontageEspecial;
	// Funciones exclusivas del Mini-Boss
	UFUNCTION(BlueprintCallable, Category = "Combate | Jefe")
	void EjecutarAtaqueEspecial();
	
	//función que ejecutará el daño después de los 2 segundos
	void DetonarOndaChoque();
	void FinalizarRecuperacion();

	// Polimorfismo: Sobreescribimos la muerte para la limpieza de memoria
	virtual void Morir() override;
};
