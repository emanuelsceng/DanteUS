#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "RecolectorAlmas.generated.h"

UCLASS()
class DANTEUS_API ARecolectorAlmas : public AEnemyBase
{
	GENERATED_BODY()

public:
	ARecolectorAlmas();

	// Sobrescribimos el Tick para manejar la visión y la huida
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:
	// Sistema de 7 golpes
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// Magia Oscura
	virtual void AtacarJugador() override;

protected:
	// --- HABILIDAD DE INVOCACIÓN ---
	UPROPERTY(EditAnywhere, Category = "Recolector | Habilidades")
	TSubclassOf<AActor> ClaseFantasmaErrante;

	bool bYaInvocoFantasmas;
	int32 ContadorGolpesRecibidos;

	void InvocarFantasmasErrantes();

	// --- ATAQUE A DISTANCIA Y COMPORTAMIENTO ---
	UPROPERTY(EditAnywhere, Category = "Recolector | Magia Oscura")
	TSubclassOf<AActor> ClaseProyectilMagia;

	UPROPERTY(EditAnywhere, Category = "Recolector | Magia Oscura")
	float DistanciaHuir;

	// Control interno para saber si está escapando
	bool bEstaHuyendo;

	void LanzarMagiaOscura();
};