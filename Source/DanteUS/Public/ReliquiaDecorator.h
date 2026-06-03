#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AtributosCombateInterface.h" 
#include "ReliquiaDecorator.generated.h"

UCLASS(Blueprintable)
class DANTEUS_API UReliquiaDecorator : public UObject, public IAtributosCombateInterface
{
	GENERATED_BODY()

protected:
	// El objeto al que estamos envolviendo (Puede ser Dante, u otro escudo)
	IAtributosCombateInterface* ObjetoEnvuelto;

public:
	// Función para inyectarle a quién va a envolver
	void InicializarDecorador(IAtributosCombateInterface* Envoltura);

	// Implementación obligatoria de la interfaz
	virtual float CalcularDanioRecibido(float DanioEntrante) override;
};