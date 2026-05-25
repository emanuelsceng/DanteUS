// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "FantasmaErrante.generated.h"

/**
 * 
 */
UCLASS()
class DANTEUS_API AFantasmaErrante : public AEnemyBase
{
	GENERATED_BODY()
public:
	AFantasmaErrante();

	//para que siga con la mirada
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:
	// Sobrescribimos TakeDamage para interceptar el daño y contar los golpes
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	// --- Mecánica de Invulnerabilidad ---

	// Contador de golpes recibidos
	int32 ContadorGolpes;

	// Estado actual de invulnerabilidad
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dante | Enemigo")
	bool bEsInvulnerable;

	// Duración de la invulnerabilidad (4 segundos según el guion)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante | Enemigo")
	float TiempoInvulnerabilidad;

	// Manejador del temporizador
	FTimerHandle TemporizadorInvulnerabilidad;

	// Función para desactivar la invulnerabilidad
	UFUNCTION()
	void RestaurarVulnerabilidad();

	// Eventos para efectos visuales en Blueprint
	UFUNCTION(BlueprintImplementableEvent, Category = "Dante | Enemigo")
	void OnInvulnerabilidadIniciada();

	UFUNCTION(BlueprintImplementableEvent, Category = "Dante | Enemigo")
	void OnInvulnerabilidadTerminada();
};