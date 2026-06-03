// Dante: El Ultimo Sello - Nivel 4
// Proyectil de veneno lanzado por el Recolector de Almas
// Clase hija de ProyectilPocion
// Al impactar: aplica mitad del dano de inmediato
// y la otra mitad en 4 ticks progresivos

#pragma once

#include "CoreMinimal.h"
#include "ProyectilPocion.h"
#include "ProyectilVeneno.generated.h"

UCLASS()
class DANTEUS_API AProyectilVeneno : public AProyectilPocion
{
    GENERATED_BODY()

public:
    AProyectilVeneno();

protected:
    virtual void BeginPlay() override;

private:

    // --- ESTADO INTERNO ---
    bool bYaImpactoVeneno;
    UPROPERTY()          //este UPROPERTY para que pueda rastrear el puntero y no falle en el 3er tick aunque dante siga vivo
    AActor* DanteEnvenenado;

    // --- CONFIGURACION DEL VENENO ---

    // Cuantos ticks de dano progresivo aplica (4 por defecto)
    UPROPERTY(EditAnywhere, Category = "Dante|Veneno")
    int32 CantidadTicks;

    int32 TicksRestantes;

    // Dano por tick (se calcula automaticamente como Dano/2 / CantidadTicks)
    float DanoPorTick;

    // Segundos entre cada tick de veneno
    UPROPERTY(EditAnywhere, Category = "Dante|Veneno")
    float IntervaloTick;

    FTimerHandle TimerVeneno;

    // --- FUNCIONES ---

    // Reemplaza el overlap del padre con la logica de veneno
    UFUNCTION()
    void AlSuperponerseVeneno(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    // Se llama cada IntervaloTick segundos para aplicar dano progresivo
    void AplicarTickVeneno();



    //eliminar luego
    UFUNCTION()
    void AlSerDestruido(AActor* ActorDestruido);
};