#pragma once

#include "CoreMinimal.h"
#include "EnemyRanged.h"
#include "MiniBossPeste.generated.h"

UCLASS()
class DANTEUS_API AMiniBossPeste : public AEnemyRanged
{
	GENERATED_BODY()

public:
	AMiniBossPeste();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void AtacarJugador() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Dante | Boss | Eventos")
	void OnAtaqueCortaDistanciaBlueprint();

	UFUNCTION(BlueprintImplementableEvent, Category = "Dante | Boss | Eventos")
	void OnPrepararVomitoBlueprint();

protected:
	virtual void BeginPlay() override;

	// Balanceo
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante | Boss | Balance")
	float VelocidadMovimientoJefe;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante | Boss | Balance")
	int32 LimiteGolpesParaVomitar;

	// Estados
	int32 ContadorGolpesRecibidos;
	bool bEstaEjecutandoHabilidad;

	// Timers
	FTimerHandle TemporizadorRadarIA;
	FTimerHandle TemporizadorPreparacion;
	FTimerHandle TemporizadorRecuperacion;
	FTimerHandle TemporizadorZarpazo;

	// Lógica de combate
	void IniciarPreparacionVomito();
	void EjecutarVomitoAbanico();
	void FinalizarRecuperacion();
	void RutinaRadarPersecucion();
	void DespertarCerebro();
};