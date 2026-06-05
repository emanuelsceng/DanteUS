
#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EstadoEnemigo.generated.h"


UINTERFACE(MinimalAPI)
class UEstadoEnemigo : public UInterface
{
   
    GENERATED_BODY()
};

class DANTEUS_API IEstadoEnemigo
{
    GENERATED_BODY()

public:

    virtual void Ingresar() = 0;

    virtual void Ejecutar(float DeltaTime) = 0;

    virtual void Salir() = 0;

    virtual FString ToString() = 0;

    virtual void SetEnemigo(class AEnemyBase* Enemigo) = 0;
};