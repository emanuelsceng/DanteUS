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

	// 1. LA BASE MATEMÁTICA (Lo que todo proyectil en el juego necesita)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
	USphereComponent* Colision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
	UProjectileMovementComponent* Movimiento;

	// 2. EL DATO VARIABLE (Lo que cada compañero cambiará en su nivel)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuracion")
	float DanoAtaque;

	// 3. LA FLEXIBILIDAD (¡El truco para tu defensa!)
	// Esta función NO se programa en C++, se programa en el Blueprint visual.
	// Permite que cada proyectil haga algo distinto al chocar (sonidos, partículas, charcos de ácido).
	UFUNCTION(BlueprintImplementableEvent, Category = "Eventos")
	void AlImpactarEfectosVisuales();

	// La función matemática interna de C++
	UFUNCTION()
	void AlChocar(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};