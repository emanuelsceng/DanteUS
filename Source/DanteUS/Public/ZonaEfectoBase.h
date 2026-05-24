#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZonaEfectoBase.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class DANTEUS_API AZonaEfectoBase : public AActor
{
    GENERATED_BODY()

public:
    AZonaEfectoBase();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dante | Zona")
    USphereComponent* AreaColision;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dante | Zona")
    UStaticMeshComponent* MallaZona;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante | Zona | Balance")
    float MultiplicadorVelocidad;

    float VelocidadOriginal;

    // Evento cuando Dante entra
    UFUNCTION()
    void OnEntraEnZona(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    // Evento cuando Dante sale
    UFUNCTION()
    void OnSaleDeZona(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};