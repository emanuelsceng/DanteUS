// Archivo: EnemigoEscupidor.h

// Previene errores de compilación por archivos duplicados
#pragma once

// Archivo base del motor Unreal
#include "CoreMinimal.h"
// Importamos la clase padre AEnemyRanged (POO: Herencia)[cite: 9]
#include "EnemyRanged.h"
// Generación de código interno de Unreal
#include "EnemigoEscupidor.generated.h"

// Etiqueta para que Unreal Engine reconozca esta clase
UCLASS()
// Declaramos al Escupidor como hijo de AEnemyRanged[cite: 9]
class DANTEUS_API AEnemigoEscupidor : public AEnemyRanged
{
    // Habilita las funciones internas y el recolector de basura
    GENERATED_BODY()

public:
    // Constructor de la clase[cite: 9]
    AEnemigoEscupidor();

    // POLIMORFISMO: Sobrescribimos el ataque base[cite: 9, 10]
    virtual void AtacarJugador() override;

    // ¡NUEVO! POLIMORFISMO: Sobrescribimos el cerebro táctico para arreglar la distancia[cite: 10]
    virtual void RutinaCerebroTactico() override;

    // Función que dispara la bala desde el Blueprint visual
    UFUNCTION(BlueprintCallable, Category = "Dante | Animacion")
    void DispararDesdeAnimacion();
};