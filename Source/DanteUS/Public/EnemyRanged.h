#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "EnemyRanged.generated.h"

class AProyectilBase;

UCLASS()
class DANTEUS_API AEnemyRanged : public AEnemyBase
{
	GENERATED_BODY()

public:
	AEnemyRanged();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante | Rango")
	TSubclassOf<AProyectilBase> ClaseProyectil;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante | Rango")
	int32 TamanoPiscina;

	UPROPERTY(VisibleAnywhere, Category = "Dante | Rango")
	TArray<AProyectilBase*> PiscinaProyectiles;

	// Interfaz pública para disparar munición del Pool
	void EjecutarDisparo(FVector Origen, FRotator Rotacion, float Velocidad, float Gravedad);

protected:
	virtual void BeginPlay() override;

	// ARREGLO CONTROL DE MEMORIA: Se ejecuta al morir o descargar el mapa para limpiar la RAM
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	AProyectilBase* ObtenerProyectilDisponible();
};