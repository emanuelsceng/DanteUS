// Fill out your copyright notice in the Description page of Project Settings.

// Dante: El Último Sello - Nivel 3
// Zona de ralentización creada por el proyectil del Mini-Boss
//
// FLUJO:
// 1. El ProyectilPocion impacta y crea esta zona
// 2. Si Dante entra en la zona se ralentiza
// 3. Cuando Dante sale recupera su velocidad normal
// 4. La zona desaparece después de unos segundos

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "ZonaCongelada.generated.h"

UCLASS()
class DANTEUS_API AZonaCongelada : public AActor
{
    GENERATED_BODY()

public:
    AZonaCongelada();

protected:
    virtual void BeginPlay() override;

public:

    // Zona circular que detecta cuando Dante entra o sale
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dante|ZonaCongelada")
    UCapsuleComponent* ZonaDeteccion;

    // Mesh visual de la zona (charco en el suelo)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dante|ZonaCongelada")
    UStaticMeshComponent* MeshZona;

    // Cuanto reduce la velocidad de Dante (0.4 = 40% de velocidad normal)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante|ZonaCongelada|Stats")
    float MultiplicadorVelocidad = 0.4f;

    // Cuantos segundos dura la zona antes de desaparecer
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante|ZonaCongelada|Stats")
    float DuracionZona = 5.0f;

    // Velocidad normal de Dante (la guardamos para restaurarla al salir)
    float VelocidadOriginalDante = 500.0f;

    // Se llama cuando Dante entra en la zona
    UFUNCTION()
    void AlEntrar(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    // Se llama cuando Dante sale de la zona
    UFUNCTION()
    void AlSalir(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
    FTimerHandle TimerDestruccion;
    void DestruirZona();

    // Referencia a Dante si esta dentro de la zona
    ACharacter* DanteDentro = nullptr;
};