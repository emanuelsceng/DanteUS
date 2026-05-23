#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProyectilBase.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class DANTEUS_API AProyectilBase : public AActor
{
	GENERATED_BODY()

public:
	AProyectilBase();

	// Componentes físicos universales expuestos al Blueprint
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Proyectil | Componentes")
	USphereComponent* Colision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Proyectil | Componentes")
	UProjectileMovementComponent* Movimiento;

	// Configuración de estadísticas
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Proyectil | Ajustes")
	float DanoAtaque;

	// Interruptor de Arquitectura (SOLID): true usa Pool, false se destruye al chocar
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Proyectil | Object Pool")
	bool bUsaObjectPool;

	// Funciones de control cinético llamadas por la piscina de memoria
	void ActivarProyectil(FVector NuevaPosicion, FRotator NuevaRotacion, float Velocidad, float Gravedad);
	void DesactivarProyectil();

protected:
	virtual void BeginPlay() override;

	// Función de impacto blindada contra fuego amigo
	UFUNCTION()
	virtual void AlChocar(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// Macro de Unreal que abre una compuerta para que tus amigos pongan partículas/sonidos en Blueprint
	UFUNCTION(BlueprintImplementableEvent, Category = "Proyectil | Efectos")
	void AlImpactarEfectosVisuales();

private:
	FTimerHandle TemporizadorReciclaje;
};