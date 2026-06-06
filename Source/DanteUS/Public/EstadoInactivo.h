#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "EstadoEnemigo.h"

#include "EstadoInactivo.generated.h"

UCLASS()
class DANTEUS_API AEstadoInactivo : public AActor, public IEstadoEnemigo
{
    GENERATED_BODY()

public:
    AEstadoInactivo();

private:
    // Puntero al enemigo dueño de este estado
    UPROPERTY()
    class AEnemyBase* Enemigo;

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    //Implementación de la interfaz IEstadoEnemigo
    virtual void Ingresar() override;
    virtual void Ejecutar(float DeltaTime) override;

    virtual void Salir() override;

    virtual FString ToString() override;

    virtual void SetEnemigo(class AEnemyBase* MiEnemigo) override;
};