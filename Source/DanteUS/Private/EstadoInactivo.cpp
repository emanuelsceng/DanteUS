// EstadoInactivo.cpp
// Implementación del estado Inactivo.
// Equivalente a NoDollarsState.cpp del libro.
// Este estado es el más simple: el enemigo no hace nada hasta ser detectado.

#include "EstadoInactivo.h"

// Incluimos EnemyBase para poder llamar a sus métodos y acceder a sus datos.
// Equivalente al libro: "#include OldSchoolSlotMachine.h"
// Nota: NO lo incluimos en el .h para evitar dependencia circular.
// Solo se incluye en el .cpp donde realmente lo necesitamos.
#include "EnemyBase.h"

AEstadoInactivo::AEstadoInactivo()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AEstadoInactivo::BeginPlay()
{
    Super::BeginPlay();
}

void AEstadoInactivo::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    // El Tick del Actor de estado NO se usa.
    // La lógica va en Ejecutar(), que llama EnemyBase::Tick().
    // Este Tick() existe porque heredamos de AActor, pero lo dejamos vacío.
}
// SET ENEMIGO
// 
// Guarda la referencia al enemigo dueño.
// Equivalente exacto al libro:
// "void ANoDollarsState::SetSlotMachine(AOldSchoolSlotMachine* SlotMachine)
//  { OldSchoolSlotMachine = SlotMachine; }"
void AEstadoInactivo::SetEnemigo(class AEnemyBase* MiEnemigo)
{
    Enemigo = MiEnemigo;
}

// INGRESAR
// 
// Se llama cuando el enemigo entra al estado Inactivo.
// Por ejemplo: cuando acaba de spawnear o cuando vuelve a patrullar.
// En este caso solo logueamos para debug, igual que el libro loguea mensajes.
void AEstadoInactivo::Ingresar()
{
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::White,
        TEXT("[Estado] Inactivo: Enemigo en espera"));
}

// EJECUTAR
//
// Llamado cada frame desde EnemyBase::Tick() mientras este sea el estado actual.
// En Inactivo no hacemos nada — el sensor de visión (PawnSensingComponent)
// disparará AlVerJugador() cuando detecte a Dante, y ESO cambiará el estado.
// Equivalente al libro donde NoDollarsState::InsertCoin() solo loguea
// que no se puede insertar moneda porque no hay dinero.
void AEstadoInactivo::Ejecutar(float DeltaTime)
{
    // Intencionalmente vacío.
    // El enemigo espera. El sensor hace el trabajo de detección.
}

// SALIR
// 
// Se llama cuando el enemigo DEJA este estado (cuando ve al jugador).
// No necesita limpiar nada en Inactivo.
void AEstadoInactivo::Salir()
{
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::White,
        TEXT("[Estado] Inactivo: Saliendo, enemigo activado"));
}

// TO STRING
//
// Retorna el nombre del estado para debug.
// Equivalente exacto al libro:
// "FString ANoDollarsState::ToString() { return "machine is out of cash"; }"
FString AEstadoInactivo::ToString()
{
    return TEXT("Estado: Inactivo");
}