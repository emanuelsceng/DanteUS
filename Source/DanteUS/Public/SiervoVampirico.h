// Dante: El Último Sello - Nivel 3
// Enemigo común vampírico - Siervo Vampírico

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "SiervoVampirico.generated.h"

UCLASS()
class DANTEUS_API ASiervoVampirico : public AEnemyBase
{
    GENERATED_BODY()

public:
    ASiervoVampirico();

    // Regenera +2 HP por golpe SOLO si el golpe conecta fisicamente
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante|Enemigo|Vampirico")
    float RegeneracionPorGolpe = 2.0f;

protected:
    virtual void BeginPlay() override;

public:
    // Sobrescribimos EjecutarGolpeMelee para curar SOLO si el golpe conecta
    // NO AtacarJugador - eso evitaba la curacion telepatica
    virtual void EjecutarGolpeMelee() override;
};
