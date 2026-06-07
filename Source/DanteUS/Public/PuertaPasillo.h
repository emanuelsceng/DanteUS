// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObservadorArena.h"
#include "EstadoPuerta.h"
#include "EstadoPuertaBloqueada.h"
#include "EstadoPuertaAbierta.h"
#include "Components/BoxComponent.h"
#include "PuertaPasillo.generated.h"

UCLASS()
class DANTEUS_API APuertaPasillo : public AActor, public IObservadorArena
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APuertaPasillo();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
    // El trigger físico que detecta a Dante
    UPROPERTY(VisibleAnywhere, Category = "Puerta")
    UBoxComponent* ZonaTrigger;

    // Estado actual — equivalente a CurrentState del libro
    UPROPERTY()
    UEstadoPuerta* EstadoActual;

    // Los dos estados concretos — creados una sola vez en BeginPlay
    UPROPERTY()
    UEstadoPuertaBloqueada* EstBloqueada;

    UPROPERTY()
    UEstadoPuertaAbierta* EstAbierta;

    // Para evitar que el trigger se active múltiples veces
    bool bTransicionIniciada = false;

public:
    // Referencia al Facade — asignada desde el editor
    UPROPERTY(EditAnywhere, Category = "Puerta | Config")
    class ASistemaTransicionFacade* MiFacade;

    // Nombre del mapa destino — asignado desde el editor
    UPROPERTY(EditAnywhere, Category = "Puerta | Config")
    FName MapaDestino;

    // Implementación del Observer — llamado por ArenaManager
    virtual void OnArenaLimpia() override;

    // Llamado por EstadoPuertaBloqueada cuando recibe el aviso
    void CambiarAEstadoAbierto();

    // Llamado por EstadoPuertaAbierta cuando Dante pisa el trigger
    void IniciarTransicion();

private:
    UFUNCTION()
    void AlPisarTrigger(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

};
