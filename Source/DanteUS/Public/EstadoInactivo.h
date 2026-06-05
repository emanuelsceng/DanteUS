

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// Incluimos la interfaz del Paso 1.
// Sin esto, el compilador no sabe qué es IEstadoEnemigo.
#include "EstadoEnemigo.h"

#include "EstadoInactivo.generated.h"

UCLASS()
class DANTEUS_API AEstadoInactivo : public AActor, public IEstadoEnemigo
{
    GENERATED_BODY()

public:
    AEstadoInactivo();

private:
 
    UPROPERTY()
    class AEnemyBase* Enemigo;

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    virtual void Ingresar() override;

    virtual void Ejecutar(float DeltaTime) override;

    virtual void Salir() override;

    virtual FString ToString() override;

    virtual void SetEnemigo(class AEnemyBase* MiEnemigo) override;
};