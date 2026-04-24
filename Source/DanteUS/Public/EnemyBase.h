// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyBase.generated.h"


class UPawnSensingComponent;

// 1. PATRÓN ESTADO: Definimos los estados posibles del enemigo
UENUM(BlueprintType)
enum class EEstadoEnemigo : uint8
{
    Inactivo        UMETA(DisplayName = "Inactivo"),
    Persiguiendo    UMETA(DisplayName = "Persiguiendo"),
    Atacando        UMETA(DisplayName = "Atacando"),
    Muerto          UMETA(DisplayName = "Muerto")
};





UCLASS()
class DANTEUS_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase();


    // Variable que guarda el Estado Actual
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dante | IA")
    EEstadoEnemigo EstadoActual;


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

	// Componente para detectar a Dante
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dante | IA")
    UPawnSensingComponent* SensorVision;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante | IA")
    float DistanciaAtaque;

    APawn* ObjetivoActual;

    FTimerHandle TemporizadorAtaque;


    // Función que se activa cuando Dante lo golpea (le quita 5 HP)
    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

    // Función para manejar la muerte y limpieza de memoria
    virtual void Morir();

	// Función que se activa cuando el enemigo ve a Dante
    UFUNCTION()
    void AlVerJugador(APawn* JugadorVisto);

    virtual void AtacarJugador();
    void FinalizarAtaque(); // Reemplaza a ResetearAtaque



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
