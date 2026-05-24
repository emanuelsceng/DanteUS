#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProyectilBase.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;

UCLASS()
class DANTEUS_API AProyectilBase : public AActor
{
	GENERATED_BODY()

public:
	AProyectilBase();

	// Función genérica para lanzar la bala con cualquier velocidad y arco de gravedad
	UFUNCTION(BlueprintCallable, Category = "Dante | Proyectil")
	virtual void Disparar(FVector Direccion, float Velocidad, float Gravedad);

	UFUNCTION(BlueprintCallable, Category = "Dante | Proyectil | Pool")
	virtual void DesactivarProyectil();

protected:
	virtual void BeginPlay() override;

	// Componentes
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dante | Componentes")
	USphereComponent* Colision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dante | Componentes")
	UStaticMeshComponent* MallaVisual;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dante | Componentes")
	UProjectileMovementComponent* ComponenteMovimiento;

	// Variables universales
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante | Balance")
	float DanoAtaque;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante | Pool")
	bool bUsaObjectPool;

	UFUNCTION()
	void AlChocar(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// EVENTO MÁGICO: Esto permite que cada bala haga algo distinto al chocar desde el Blueprint (Ej. Crear charco de veneno)
	UFUNCTION(BlueprintImplementableEvent, Category = "Dante | Eventos")
	void OnImpactoEspecial(AActor* ActorGolpeado, FVector UbicacionImpacto);


};