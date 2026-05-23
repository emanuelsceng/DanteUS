// Fill out your copyright notice in the Description page of Project Settings.

// Dante: El Último Sello - Nivel 3
// Proyectil lanzado por el Mini-Boss
// Usa Overlap en vez de Hit - mas confiable en UE5

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "ProyectilPocion.generated.h"


UCLASS()
class DANTEUS_API AProyectilPocion : public AActor
{
    GENERATED_BODY()

public:
    AProyectilPocion();

protected:
    virtual void BeginPlay() override;

public:

    // Colision esferica del proyectil
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dante|Proyectil")
    USphereComponent* ColisionEsfera;

    // Mesh visual del proyectil
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dante|Proyectil")
    UStaticMeshComponent* MeshPocion;

    // Maneja el movimiento automaticamente
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dante|Proyectil")
    UProjectileMovementComponent* MovimientoProyectil;

    // Velocidad de vuelo
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante|Proyectil|Stats")
    float VelocidadVuelo = 800.0f;

    // Daño que aplica al golpear a Dante
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante|Proyectil|Stats")
    float Dano = 15.0f;

    // Clase de ZonaCongelada a spawnear al impactar
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante|Proyectil|Stats")
    TSubclassOf<AActor> ClaseZonaCongelada;

    // Efecto de escarcha al impactar (asignar NS_Basic_7 desde el editor)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante|Proyectil|Stats")
    UNiagaraSystem* EfectoImpacto;

    // Se llama cuando el proyectil se superpone con algo (Overlap)
    UFUNCTION()
    void AlSuperponerse(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

private:
    // Evita que el proyectil se destruya multiples veces
    bool bYaImpacto = false;

    // Crea la zona congelada en el punto de impacto
    void CrearZonaCongelada(FVector Ubicacion);
};