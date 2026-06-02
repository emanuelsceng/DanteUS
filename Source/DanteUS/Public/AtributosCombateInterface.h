// AtributosCombateInterface.h
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AtributosCombateInterface.generated.h"

// Esta clase U es solo para que el motor de Unreal la reconozca (Reflection System)
UINTERFACE(MinimalAPI)
class UAtributosCombateInterface : public UInterface
{
	GENERATED_BODY()
};

// Esta es la clase I donde realmente programamos la lógica
class DANTEUS_API IAtributosCombateInterface
{
	GENERATED_BODY()

public:

	// Función virtual pura. Cualquiera que use esta interfaz ESTÁ OBLIGADO a decir cómo recibe daño.
	// No le ponemos "UFUNCTION" porque usaremos C++ puro para el patrón Decorator.
	virtual float CalcularDanioRecibido(float DanioEntrante) = 0;
};