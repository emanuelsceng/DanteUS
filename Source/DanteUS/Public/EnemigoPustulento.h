#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "EnemigoPustulento.generated.h"

UCLASS()
class DANTEUS_API AEnemigoPustulento : public AEnemyBase
{
    GENERATED_BODY()

public:
    AEnemigoPustulento();

    // Componentes y Lógica
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Peste | Componentes")
    class USphereComponent* AuraVeneno;

    virtual void AtacarJugador() override;

    // --- PUENTE DE ANIMACIÓN ---
    UPROPERTY(BlueprintReadOnly, Category = "Animacion")
    float VelocidadMovimiento;

    UPROPERTY(BlueprintReadOnly, Category = "Animacion")
    bool bEstaAtacando;

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

private:
    // --- VARIABLES DE LÓGICA (ENCAPSULADAS) ---
    AActor* DanteEnvenenado; // ESTA ERA LA QUE FALTABA
    FTimerHandle TimerHandle_ResetAnim;
    FTimerHandle TemporizadorVeneno;

    // Patrón Observador (Delegates)
    UFUNCTION()
    void AlEntrarAlAura(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void AlSalirDelAura(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    void AplicarVeneno();
};