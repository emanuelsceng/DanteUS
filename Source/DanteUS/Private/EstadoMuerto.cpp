// EstadoMuerto.cpp
// El estado más importante en términos de limpieza de memoria y recursos.
// Equivalente a WonDollarsState.cpp del libro — el estado donde
// se ejecuta la acción final (EmitDollars → aquí: muerte del enemigo).
// Todo el código que antes estaba disperso en TakeDamage() cuando
// Salud <= 0 ahora está ENCAPSULADO aquí en Ingresar().
// Esto es exactamente lo que el patrón State busca:
// localizar el comportamiento específico de cada estado en su propia clase.

#include "EstadoMuerto.h"
#include "EnemyBase.h"

// Necesitamos CharacterMovementComponent para detener el movimiento.
// Es el componente de Unreal que controla física y movimiento del Character.
#include "GameFramework/CharacterMovementComponent.h"

AEstadoMuerto::AEstadoMuerto()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AEstadoMuerto::BeginPlay()
{
    Super::BeginPlay();
}

void AEstadoMuerto::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    // Vacío: un enemigo muerto no necesita lógica por frame.
}

// SET ENEMIGO
//
void AEstadoMuerto::SetEnemigo(class AEnemyBase* MiEnemigo)
{
    Enemigo = MiEnemigo;
}

// INGRESAR
// 
// El método más importante de todo el patrón en nuestro juego.
// Se llama UNA VEZ cuando TakeDamage() detecta Salud <= 0 y llama
// SetEstado(EstadoMuerto).
//
// Todo lo que antes estaba en TakeDamage() cuando Salud <= 0
// ahora vive aquí, correctamente encapsulado en su estado.
// Equivalente al libro donde WonDollarsState::Payout() ejecuta
// EmitDollars() y cambia el estado de la máquina.
void AEstadoMuerto::Ingresar()
{
    // Verificación de seguridad: si no hay dueño, no hacemos nada.
    if (!Enemigo) return;

    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple,
        TEXT("[Estado] Muerto: Enemigo eliminado"));

    // 1. Detenemos todos los montajes de animación
    // StopAllMontages(0.1f): para cualquier animación en curso con un
    // blend out de 0.1 segundos para que no se corte abruptamente.
    // GetMesh(): el SkeletalMeshComponent del Character (el modelo 3D).
    // GetAnimInstance(): el controlador de animaciones (Animation Blueprint).
    if (Enemigo->GetMesh() && Enemigo->GetMesh()->GetAnimInstance())
    {
        Enemigo->GetMesh()->GetAnimInstance()->StopAllMontages(0.1f);
    }

    // 2. Detenemos la IA y el movimiento físico
    // DisableMovement(): congela al personaje en su lugar.
    // Sin esto, el cadáver podría seguir deslizándose por inercia.
    if (Enemigo->GetCharacterMovement())
    {
        Enemigo->GetCharacterMovement()->DisableMovement();
    }

    // 3. Desactivamos las colisiones
    // SetActorEnableCollision(false): el cadáver ya no bloquea al jugador
    // ni recibe más eventos de daño o solapamiento.
    // Esto también evita que TakeDamage() se llame de nuevo post-muerte.
    Enemigo->SetActorEnableCollision(false);

    // 4. Ajustamos la altura visual de la malla
    // DesfaseZMuerte: variable configurable desde el Blueprint del enemigo.
    // Sirve para que el modelo no atraviese el suelo al caer.
    // AddLocalOffset(): mueve la malla en coordenadas locales del Actor.
    if (Enemigo->GetMesh())
    {
        Enemigo->GetMesh()->AddLocalOffset(
            FVector(0.0f, 0.0f, Enemigo->DesfaseZMuerte)
        );
    }

    // 5. Programamos la destrucción del Actor 
    // //
    // Equivalente al libro donde el estado final programa la siguiente acción:
    // "SetTimer → Morir()" que llama Destroy().
    // TiempoDesaparicion: configurable desde Blueprint (por defecto 15 segundos).
    // SetTimer(): programa una llamada a Enemigo->Morir() una vez pasado el tiempo.
    // false: el timer NO se repite (se ejecuta solo una vez).
    FTimerHandle TimerMuerte;
    Enemigo->GetWorldTimerManager().SetTimer(
        TimerMuerte,
        Enemigo,
        &AEnemyBase::Morir,
        Enemigo->TiempoDesaparicion,
        false
    );
}

// EJECUTAR
// 
// Vacío: un enemigo muerto no hace nada por frame.
// TakeDamage() en EnemyBase ya verifica que si EstadoActual es EstadoMuerto
// ignora cualquier daño extra, así que este Ejecutar() nunca causará problemas.
void AEstadoMuerto::Ejecutar(float DeltaTime)
{
    // Intencionalmente vacío.
    // El enemigo está muerto, no hay lógica de frame.
}

// SALIR
//
// En teoría nunca se llama — Muerto es un estado terminal.
// Lo implementamos igual porque la interfaz lo exige (es virtual puro).
// Si por algún bug se llamara, el log nos avisaría.
void AEstadoMuerto::Salir()
{
    // Estado terminal: no debería salir nunca.
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple,
        TEXT("[Estado] Muerto: ADVERTENCIA - Salir() llamado en estado terminal"));
}

// TO STRING
//
// Equivalente exacto al libro:
// "FString AWonDollarsState::ToString() { return "machine paid out"; }"
FString AEstadoMuerto::ToString()
{
    return TEXT("Estado: Muerto");
}