// Dante: El Último Sello - Nivel 3
// Enemigo común vampírico - Guardián Vampírico
// Hereda de AEnemyBase, por lo que ya tiene:
//   - Salud (20 HP), Estados, Sensor de visión, TakeDamage y Morir
// Diferencias con los otros enemigos comunes:
//   - Es el MÁS FUERTE de los comunes: 10 de daño por golpe
//   - Regenera MÁS vida al atacar: +4 HP por golpe (el doble que los demás)
//   - Es más lento pero muy peligroso si Dante lo ignora

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "Animation/AnimMontage.h"
#include "GuardianVampirico.generated.h"

UCLASS()
class DANTEUS_API AGuardianVampirico : public AEnemyBase
{
    GENERATED_BODY()

public:
    AGuardianVampirico();

    // Regenera +4 HP por golpe SOLO si el golpe conecta fisicamente
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante|Enemigo|Vampirico")
    float RegeneracionPorGolpe = 4.0f;

    // Montaje de animacion de muerte
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante|Enemigo|Animaciones")
    UAnimMontage* MontajeMuerte;

    // Montaje de animacion de ataque
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante|Enemigo|Animaciones")
    UAnimMontage* MontajeAtaqueGuardian;

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // Sobrescribimos EjecutarGolpeMelee para curar SOLO si el golpe conecta
    virtual void EjecutarGolpeMelee() override;

};