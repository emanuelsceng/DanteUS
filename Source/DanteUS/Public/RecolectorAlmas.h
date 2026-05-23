// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "RecolectorAlmas.generated.h"

UCLASS()
class DANTEUS_API ARecolectorAlmas : public AEnemyBase
{
	GENERATED_BODY()

public:
	ARecolectorAlmas();

protected:
	virtual void BeginPlay() override;

public:
	// Sobrescribimos TakeDamage para llevar la cuenta de los golpes
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	// La clase del Fantasma Errante que asignaremos en el Blueprint
	UPROPERTY(EditAnywhere, Category = "Recolector | Habilidades")
	TSubclassOf<AActor> ClaseFantasmaErrante;

	// Variables para la lógica de invocación
	bool bYaInvocoFantasmas;
	int32 ContadorGolpesRecibidos;

	// Función que calcula la fila y spawnea a los fantasmas
	void InvocarFantasmasErrantes();
};