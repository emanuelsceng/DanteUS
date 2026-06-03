// EstadoPersiguiendo.cpp
// El estado más "activo" de la IA: mueve al enemigo hacia Dante cada frame.
// Equivalente a NoCoinState.cpp del libro — el estado que reacciona
// a la acción principal (InsertCoin → aquí: ver al jugador y perseguir).

#include "EstadoPersiguiendo.h"

// Incluimos EnemyBase solo en el .cpp, nunca en el .h,
#include "EnemyBase.h"
#include "GameFramework/CharacterMovementComponent.h"
// AIController: el cerebro de IA de Unreal que mueve al personaje.
// MoveToActor() le dice al controlador que persiga un Actor específico.
#include "AIController.h"

AEstadoPersiguiendo::AEstadoPersiguiendo()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AEstadoPersiguiendo::BeginPlay()
{
    Super::BeginPlay();
}

void AEstadoPersiguiendo::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    // Igual que EstadoInactivo: este Tick() de AActor se deja vacío.
    // Toda la lógica va en Ejecutar(), llamado desde EnemyBase::Tick().
}

// SET ENEMIGO
// 
void AEstadoPersiguiendo::SetEnemigo(class AEnemyBase* MiEnemigo)
{
    // Guardamos la referencia al dueño.
    // A partir de aquí podemos acceder a todo lo de EnemyBase:
    // Enemigo->ObjetivoActual, Enemigo->DistanciaAtaque, etc.
    Enemigo = MiEnemigo;
}

// INGRESAR
// 
// Se llama cuando el enemigo ENTRA a perseguir.
// Aquí activamos el movimiento del CharacterMovement por si estaba detenido
// (por ejemplo si venía del estado Muerto o Atacando que lo frenó).
void AEstadoPersiguiendo::Ingresar()
{
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow,
        TEXT("[Estado] Persiguiendo: Enemigo activado, buscando a Dante"));

    // Reactivamos el movimiento por si algún estado anterior lo desactivó.
    // GetCharacterMovement() viene de ACharacter, que es la clase base de AEnemyBase.
    // Como Enemigo es AEnemyBase* (que hereda de ACharacter), podemos llamarlo.
    if (Enemigo && Enemigo->GetCharacterMovement())
    {
        Enemigo->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
    }
}

// EJECUTAR
//
// Este es el método más importante de este estado.
// Se llama CADA FRAME desde EnemyBase::Tick().
// Contiene toda la lógica que antes estaba en el case Persiguiendo del switch.
// La diferencia clave: ahora está ENCAPSULADA en su propia clase.
void AEstadoPersiguiendo::Ejecutar(float DeltaTime)
{
    // Verificación de seguridad: si no tenemos dueño o no hay objetivo, salimos.
    // Esto evita crashes si el jugador fue destruido o algo salió mal.
    if (!Enemigo || !Enemigo->ObjetivoActual) return;

    // Calculamos la distancia entre el enemigo y Dante en este frame.
    // FVector::Dist(): función estática de Unreal que calcula distancia 3D.
    // GetActorLocation(): posición actual del Actor en el mundo.
    float Distancia = FVector::Dist(
        Enemigo->GetActorLocation(),
        Enemigo->ObjetivoActual->GetActorLocation()
    );

    // Obtenemos el controlador de IA del enemigo.
    // Cast<AAIController>: conversión de tipo segura de Unreal.
    // GetController() devuelve AController*, lo casteamos a AAIController*
    // para acceder a MoveToActor() y StopMovement().
    AAIController* ControladorIA = Cast<AAIController>(Enemigo->GetController());

    // ¿Llegamos a rango de ataque?
    //
    if (Distancia <= Enemigo->DistanciaAtaque)
    {
        // Transición de estado: Persiguiendo → Atacando
        // Equivalente al libro donde NoCoinState::InsertCoin() llama:
        // "OldSchoolSlotMachine->SetState(OldSchoolSlotMachine->GetCoinInsertedState())"
        // Aquí: le pedimos al enemigo que cambie al estado Atacando.
        if (ControladorIA)
        {
            // Detenemos el movimiento antes de atacar
            ControladorIA->StopMovement();
        }

        // SetEstado() llama a Salir() de este estado e Ingresar() del Atacando.
        // GetEstadoAtacando() es el getter que pusimos en EnemyBase.h
        Enemigo->SetEstado(Enemigo->GetEstadoAtacando());
    }
    else
    {
        // SI Todavía lejos: seguimos persiguiendo 
        // MoveToActor(): le dice al NavMesh que mueva al enemigo hacia el objetivo.
        // 15.0f: radio de aceptación (se considera "llegado" a 15 unidades).
        if (ControladorIA)
        {
            ControladorIA->MoveToActor(Enemigo->ObjetivoActual, 15.0f);
        }
    }
}

// SALIR
//
// Se llama cuando el enemigo DEJA de perseguir (pasa a Atacar o muere).
void AEstadoPersiguiendo::Salir()
{
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow,
        TEXT("[Estado] Persiguiendo: Saliendo, objetivo alcanzado o muerto"));
}

// TO STRING
// 
FString AEstadoPersiguiendo::ToString()
{
    return TEXT("Estado: Persiguiendo");
}