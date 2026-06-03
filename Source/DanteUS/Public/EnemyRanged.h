#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "ProyectilBase.h"
#include "EnemyRanged.generated.h"

/**
 * AEnemyRanged: Clase Base para enemigos a distancia.
 * OCP (Open/Closed Principle): Esta clase está cerrada a cambios internos,
 * pero abierta a que las hijas modifiquen el comportamiento de AtacarJugador().
 */
UCLASS()
class DANTEUS_API AEnemyRanged : public AEnemyBase
{
	GENERATED_BODY()

public:
	AEnemyRanged();

	// POLIMORFISMO: Marcamos como VIRTUAL. 
	// Así, el Escupidor o el Jefe pueden usar su propio "AtacarJugador" sin tocar este archivo.
	virtual void AtacarJugador() override;

protected:
	virtual void BeginPlay() override;

	// --- 1. MÓDULO DE INTELIGENCIA TÁCTICA ---
	// Puse estas variables en PROTECTED para que los hijos puedan editarlas en su constructor.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante | Ranged | Inteligencia")
	bool bHuyeDelJugador;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante | Ranged | Inteligencia")
	float DistanciaParaHuir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante | Ranged | Inteligencia")
	float DistanciaParaAtacar;

	FTimerHandle TemporizadorCerebroTactico;
	virtual void RutinaCerebroTactico();
	virtual void HuirDeDante(class ACharacter* Dante);

	// --- 2. MÓDULO DE OBJECT POOL ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante | Ranged | Pool")
	TSubclassOf<AProyectilBase> ClaseProyectil;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante | Ranged | Pool")
	int32 TamanoPiscina;

	// La lista de proyectiles reciclables
	TArray<AProyectilBase*> PiscinaProyectiles;

	void InicializarPiscina();
	AProyectilBase* ObtenerProyectilDisponible();

	// Función que el hijo llamará para disparar (usa las variables de la piscina)
	void EjecutarDisparo(FVector Origen, FRotator Rotacion, float Velocidad, float Gravedad);
};