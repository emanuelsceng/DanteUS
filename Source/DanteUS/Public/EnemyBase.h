// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyBase.generated.h"

UCLASS()
class DANTEUS_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase();

public:
    // Salud actual del enemigo
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante | Enemigo")
    float Salud;

    // Salud máxima (Sección 0.1: 20, 75 o 150 HP)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante | Enemigo")
    float SaludMaxima;

    // Daño que inflige este enemigo (2 puntos para comunes)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante | Enemigo")
    float DanoAtaque;

    // Función que se activa cuando Dante lo golpea (le quita 5 HP)
    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

    // Función para manejar la muerte y limpieza de memoria
    virtual void Morir();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
