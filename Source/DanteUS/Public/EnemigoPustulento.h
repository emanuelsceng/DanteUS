#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h" // 1. HERENCIA: Traemos toda la lógica del enemigo base
#include "EnemigoPustulento.generated.h"

UCLASS()
class DANTEUS_API AEnemigoPustulento : public AEnemyBase
{
	GENERATED_BODY()

public:
	AEnemigoPustulento();

	// Componente de colisión para la mecánica de riesgo (Escudo de Hedor)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Peste | Componentes")
	class USphereComponent* AuraVeneno;

	// 2. POLIMORFISMO: Sobreescribimos la forma en que este enemigo ataca
	virtual void AtacarJugador() override;

protected:
	virtual void BeginPlay() override;

private:
	// 3. ENCAPSULAMIENTO: Estas variables y funciones son privadas. 
	// Solo el Pustulento sabe cómo funciona su veneno.

	// Patrón Observador: Funciones que "escuchan" cuando Dante entra o sale
	UFUNCTION()
	void AlEntrarAlAura(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void AlSalirDelAura(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Función privada que hace el daño real
	void AplicarVeneno();

	// Herramientas internas
	FTimerHandle TemporizadorVeneno;
	AActor* DanteEnvenenado;
};