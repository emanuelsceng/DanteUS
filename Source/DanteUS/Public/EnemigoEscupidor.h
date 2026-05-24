#pragma once

#include "CoreMinimal.h"
#include "EnemyRanged.h" // Hereda del sistema base
#include "EnemigoEscupidor.generated.h"

UCLASS()
class DANTEUS_API AEnemigoEscupidor : public AEnemyRanged
{
	GENERATED_BODY()

public:
	AEnemigoEscupidor();

	// POLIMORFISMO: Esta es la pieza clave. 
	// Sobrescribimos el ataque base para darle el comportamiento de "escupitajo"
	virtual void AtacarJugador() override;
	// En EnemigoEscupidor.h, dentro de la clase:
private:
	float TiempoEntreDisparos = 2.0f; // Segundos que espera entre cada disparo
	float UltimoTiempoDisparo = 0.0f; // Guarda cuándo fue el último disparo
};