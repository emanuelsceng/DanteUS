#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h" // 1. HERENCIA: Ya sabe caminar, perseguir y morir.
#include "EnemigoEscupidor.generated.h"

UCLASS()
class DANTEUS_API AEnemigoEscupidor : public AEnemyBase
{
	GENERATED_BODY()

public:
	AEnemigoEscupidor();

	// 2. ENCAPSULAMIENTO y FLEXIBILIDAD:
	// Igual que hiciste antes, creamos un "molde" para que en Unreal
	// puedas arrastrar el Blueprint de tu bola de vómito.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Peste | Combate")
	TSubclassOf<AActor> ClaseProyectilVomito;

	// 3. POLIMORFISMO: La palabra clave 'override' es la estrella aquí.
	// Le dice al compilador que vamos a reescribir esta función a nuestra manera.
	virtual void AtacarJugador() override;
};