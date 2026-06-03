#pragma once

#include "CoreMinimal.h"
#include "ReliquiaDecorator.h"
#include "ReliquiaEscudo.generated.h"

UCLASS(Blueprintable)
class DANTEUS_API UReliquiaEscudo : public UReliquiaDecorator
{
	GENERATED_BODY()

public:
	// 0.0 = No reduce nada | 0.90 = Reduce el 90% del daño | 1.0 = Invulnerabilidad total
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante | Reliquias")
	float PorcentajeReduccion = 1.0f;

	// Sobrescribimos la función para aplicar la reducción matemática
	virtual float CalcularDanioRecibido(float DanioEntrante) override;
};