// EnemyBase.cpp
// EnemyBase actúa como el "CONTEXTO" del patrón State, igual que
// OldSchoolSlotMachine.cpp del libro.
// Su responsabilidad: instanciar los estados, guardarlos, y DELEGAR
// el comportamiento al estado actual. Ya no decide nada con un switch.

#include "EnemyBase.h"

// Incluimos las 4 clases de estado concretas que crearemos en los pasos siguientes.
// Equivalente al libro que incluye:
// #include "NoDollarsState.h"
// #include "NoCoinState.h" etc.
// Por ahora generarán advertencia hasta que creemos esas clases — es normal.
#include "EstadoInactivo.h"
#include "EstadoPersiguiendo.h"
#include "EstadoAtacando.h"
#include "EstadoMuerto.h"

#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

// ─────────────────────────────────────────────────────────────────────────────
// CONSTRUCTOR
// ─────────────────────────────────────────────────────────────────────────────
AEnemyBase::AEnemyBase()
{
    PrimaryActorTick.bCanEverTick = true;

    // Valores base del enemigo — sin cambios respecto a tu versión original
    SaludMaxima = 20.0f;
    Salud = SaludMaxima;
    DanoAtaque = 2.0f;
    DistanciaAtaque = 120.0f;
    DesfaseZMuerte = 0.0f;

    // Creamos el componente de visión igual que antes
    SensorVision = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("SensorVision"));
    SensorVision->SightRadius = 1500.0f;
    SensorVision->SetPeripheralVisionAngle(45.0f);

    // Los estados NO se crean aquí sino en BeginPlay() con SpawnActor<>(),
    // igual que el libro usa Initialize() para hacer el SpawnActor<>.
    // En el constructor de Unreal no existe el mundo todavía (GetWorld() = null).
}

// ─────────────────────────────────────────────────────────────────────────────
// BEGIN PLAY — equivalente al Initialize() del libro
// ─────────────────────────────────────────────────────────────────────────────
// En el libro:
// "NoDollarsState = GetWorld()->SpawnActor<ANoDollarsState>(...)"
// "NoDollarsState->SetSlotMachine(this);"
// Hacemos exactamente lo mismo para los 4 estados del enemigo.
void AEnemyBase::BeginPlay()
{
    Super::BeginPlay();

    // ── Instanciamos el Estado Inactivo ──────────────────────────────────────
    // SpawnActor<AEstadoInactivo>: crea un Actor de tipo AEstadoInactivo en el mundo.
    // Es idéntico al libro: "GetWorld()->SpawnActor<ANoDollarsState>(ANoDollarsState::StaticClass())"
    // StaticClass(): método estático que devuelve la clase en tiempo de ejecución (reflexión de Unreal).
    AEstadoInactivo* ActorInactivo = GetWorld()->SpawnActor<AEstadoInactivo>(AEstadoInactivo::StaticClass());
    // SetEnemigo(this): le pasamos al estado quién es su dueño (este enemigo).
    // Equivalente al libro: "NoDollarsState->SetSlotMachine(this);"
    ActorInactivo->SetEnemigo(this);
    // Guardamos el puntero en el TScriptInterface para que Unreal lo proteja
    EstadoInactivo = ActorInactivo;

    // ── Instanciamos el Estado Persiguiendo ─────────────────────────────────
    AEstadoPersiguiendo* ActorPersiguiendo = GetWorld()->SpawnActor<AEstadoPersiguiendo>(AEstadoPersiguiendo::StaticClass());
    ActorPersiguiendo->SetEnemigo(this);
    EstadoPersiguiendo = ActorPersiguiendo;

    // ── Instanciamos el Estado Atacando ──────────────────────────────────────
    AEstadoAtacando* ActorAtacando = GetWorld()->SpawnActor<AEstadoAtacando>(AEstadoAtacando::StaticClass());
    ActorAtacando->SetEnemigo(this);
    EstadoAtacando = ActorAtacando;

    // ── Instanciamos el Estado Muerto ────────────────────────────────────────
    AEstadoMuerto* ActorMuerto = GetWorld()->SpawnActor<AEstadoMuerto>(AEstadoMuerto::StaticClass());
    ActorMuerto->SetEnemigo(this);
    EstadoMuerto = ActorMuerto;

    // ── Estado inicial: Inactivo ─────────────────────────────────────────────
    // El enemigo arranca inactivo, igual que el libro arranca en NoCoinState
    // cuando hay dinero: "if (NumberOfDollars > 0) State = NoCoinState;"
    // Usamos SetEstado() para que llame a Ingresar() correctamente.
    SetEstado(EstadoInactivo);

    // ── Vinculamos el sensor de visión ───────────────────────────────────────
    if (SensorVision)
    {
        SensorVision->OnSeePawn.AddDynamic(this, &AEnemyBase::AlVerJugador);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// SET ESTADO — el corazón del patrón State
// ─────────────────────────────────────────────────────────────────────────────
// Equivalente al libro: "void SetState(IState* myState) { State = myState; }"
// Nosotros lo mejoramos llamando a Salir() e Ingresar() para el ciclo de vida.
void AEnemyBase::SetEstado(TScriptInterface<IEstadoEnemigo> NuevoEstado)
{
    // Si ya hay un estado activo, le avisamos que va a terminar
    // Salir() limpia timers, animaciones, etc. del estado anterior
    if (EstadoActual)
    {
        EstadoActual->Salir();
    }

    // Cambiamos el puntero al nuevo estado
    // Equivalente exacto al libro: "State = myState;"
    EstadoActual = NuevoEstado;

    // Le avisamos al nuevo estado que empieza
    // Ingresar() arranca animaciones, lógica inicial, etc.
    if (EstadoActual)
    {
        EstadoActual->Ingresar();
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// TICK — delega al estado actual, sin switch
// ─────────────────────────────────────────────────────────────────────────────
// Antes tenías un switch con toda la lógica aquí.
// Ahora es UNA sola línea de delegación, igual al libro:
// "State->InsertCoin()" delega la acción al estado actual.
// Aquí: "EstadoActual->Ejecutar(DeltaTime)" delega el frame completo.
void AEnemyBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Si hay un estado activo, le pasamos el control total del frame
    // Cada estado sabe qué hacer (perseguir, atacar, no hacer nada, etc.)
    if (EstadoActual)
    {
        EstadoActual->Ejecutar(DeltaTime);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// AL VER JUGADOR — transición de estado por evento del sensor
// ─────────────────────────────────────────────────────────────────────────────
// Cuando el PawnSensingComponent detecta a Dante, este método se dispara.
// Le pasamos el control al estado actual para que decida qué hacer.
// Solo el EstadoInactivo reaccionará (los demás ignorarán el evento).
void AEnemyBase::AlVerJugador(APawn* JugadorVisto)
{
    if (JugadorVisto != nullptr)
    {
        // Guardamos la referencia al jugador para que los estados la usen
        ObjetivoActual = JugadorVisto;

        // El estado actual decide si reacciona o no.
        // EstadoInactivo::Ingresar() hará la transición a Persiguiendo.
        // Forzamos la transición directamente aquí, como hace el libro
        // cuando llama a una acción y el estado cambia internamente.
        if (EstadoActual.GetObject() == EstadoInactivo.GetObject())
        {
            SetEstado(EstadoPersiguiendo);
        }
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// ATACAR JUGADOR — lo usan los estados de combate
// ─────────────────────────────────────────────────────────────────────────────
// Este método sigue aquí en el contexto porque AcechadorSanguinario
// lo sobrescribe (override) para agregar regeneración vampírica.
// Los estados llaman a Enemigo->AtacarJugador() y el polimorfismo
// hace que se ejecute la versión correcta del hijo.
void AEnemyBase::AtacarJugador()
{
    if (MontageAtaque)
    {
        PlayAnimMontage(MontageAtaque);
    }

    GetWorldTimerManager().SetTimer(
        TemporizadorAtaque,
        this,
        &AEnemyBase::FinalizarAtaque,
        1.5f,
        false
    );
}

// ─────────────────────────────────────────────────────────────────────────────
// FINALIZAR ATAQUE — vuelve a perseguir
// ─────────────────────────────────────────────────────────────────────────────
// El timer de AtacarJugador() llama a esto cuando termina la animación.
// Transiciona de Atacando → Persiguiendo si el enemigo sigue vivo.
void AEnemyBase::FinalizarAtaque()
{
    // Verificamos que no esté muerto antes de volver a perseguir
    if (EstadoActual.GetObject() != EstadoMuerto.GetObject())
    {
        SetEstado(EstadoPersiguiendo);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// EJECUTAR GOLPE MELEE — llamado desde la animación (Animation Notify)
// ─────────────────────────────────────────────────────────────────────────────
void AEnemyBase::EjecutarGolpeMelee()
{
    if (ObjetivoActual)
    {
        float Distancia = FVector::Dist(GetActorLocation(), ObjetivoActual->GetActorLocation());

        if (Distancia <= (DistanciaAtaque + 50.0f))
        {
            UGameplayStatics::ApplyDamage(
                ObjetivoActual,
                DanoAtaque,
                GetController(),
                this,
                UDamageType::StaticClass()
            );
            GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Enemigo: Toma un golpe!"));
        }
        else
        {
            GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, TEXT("Enemigo: Fallo el golpe!"));
        }
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// TAKE DAMAGE — recibe daño y transiciona a Muerto si la salud llega a 0
// ─────────────────────────────────────────────────────────────────────────────
float AEnemyBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
    AController* EventInstigator, AActor* DamageCauser)
{
    // Si ya está muerto, ignoramos el daño extra
    if (EstadoActual.GetObject() == EstadoMuerto.GetObject()) return 0.0f;

    float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    Salud -= DamageToApply;

    if (Salud <= 0.0f)
    {
        // Transición al estado Muerto — el estado se encarga del resto
        // Equivalente al libro cuando una condición cambia el State:
        // "OldSchoolSlotMachine->SetState(OldSchoolSlotMachine->GetNoDollarsState())"
        SetEstado(EstadoMuerto);
    }

    return DamageToApply;
}

// ─────────────────────────────────────────────────────────────────────────────
// MORIR — destruye el actor después del TiempoDesaparicion
// ─────────────────────────────────────────────────────────────────────────────
// Este método lo llama EstadoMuerto con un timer después de TiempoDesaparicion.
void AEnemyBase::Morir()
{
    Destroy();
}

void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}