#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProyectilBase.generated.h"

// Forward declarations para compilar más rápido
class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class DANTEUS_API AProyectilBase : public AActor
{
	GENERATED_BODY()

public:
	AProyectilBase();

	// 1. LA FÍSICA: La caja de colisión (Esfera) que detectará los impactos
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
	USphereComponent* EsferaColision;

	// 2. EL MOTOR: Este componente de Unreal calcula la velocidad, rebotes y gravedad
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
	UProjectileMovementComponent* ComponenteMovimiento;

	// 3. DISEÑO ORIENTADO A DATOS: Variable expuesta para que cada nivel ponga su propio daño
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Estadisticas")
	float DanoProyectil;

	// Función que actúa como "Sensor" cuando la bola choca contra algo
	UFUNCTION()
	void AlChocar(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	virtual void BeginPlay() override;
};