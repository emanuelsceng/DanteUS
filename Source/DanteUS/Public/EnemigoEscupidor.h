#pragma once

#include "CoreMinimal.h"
#include "EnemyRanged.h" // <--- Importante: Incluimos a su nuevo padre disparador
#include "EnemigoEscupidor.generated.h"

/**
 * CLASE ENEMIGO ESCUPIDOR (Nivel 2 - Sello de la Peste)
 * * Aplica Programación Orientada a Objetos mediante Especialización por Herencia.
 * Al heredar de AEnemyRanged, adquiere automáticamente el patrón Object Pool
 * para disparar proyectiles optimizados sin necesidad de duplicar código.
 */
UCLASS()
class DANTEUS_API AEnemigoEscupidor : public AEnemyRanged
{
	GENERATED_BODY()

public:
	// Constructor: Aquí configuramos su vida, daño y el tamaño de su cargador
	AEnemigoEscupidor();

	/** * POLIMORFISMO: Sobrescribimos la función de ataque.
	 * En lugar de dar un golpe cuerpo a cuerpo, calculamos el tiro parabólico de su vómito.
	 */
	virtual void AtacarJugador() override;
};