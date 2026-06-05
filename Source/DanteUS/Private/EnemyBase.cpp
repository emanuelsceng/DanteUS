// Su responsabilidad: instanciar los estados, guardarlos, y DELEGAR
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

// CONSTRUCTOR
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

// BEGIN PLAY — equivalente al Initialize() del libro
void AEnemyBase::BeginPlay()
{
    Super::BeginPlay();

    // Instanciamos el Estado Inactivo
    AEstadoInactivo* ActorInactivo = GetWorld()->SpawnActor<AEstadoInactivo>(AEstadoInactivo::StaticClass());

    ActorInactivo->SetEnemigo(this);
    // Guardamos el puntero en el TScriptInterface para que Unreal lo proteja
    EstadoInactivo = ActorInactivo;

    AEstadoPersiguiendo* ActorPersiguiendo = GetWorld()->SpawnActor<AEstadoPersiguiendo>(AEstadoPersiguiendo::StaticClass());
    ActorPersiguiendo->SetEnemigo(this);
    EstadoPersiguiendo = ActorPersiguiendo;
    AEstadoAtacando* ActorAtacando = GetWorld()->SpawnActor<AEstadoAtacando>(AEstadoAtacando::StaticClass());
    ActorAtacando->SetEnemigo(this);
    EstadoAtacando = ActorAtacando;

    AEstadoMuerto* ActorMuerto = GetWorld()->SpawnActor<AEstadoMuerto>(AEstadoMuerto::StaticClass());
    ActorMuerto->SetEnemigo(this);
    EstadoMuerto = ActorMuerto;

    //Estado inicial: Inactivo
    SetEstado(EstadoInactivo);

    // Vinculamos el sensor de visión
    if (SensorVision)
    {
        SensorVision->OnSeePawn.AddDynamic(this, &AEnemyBase::AlVerJugador);
    }
}

// SET ESTADO ciclo de vida
void AEnemyBase::SetEstado(TScriptInterface<IEstadoEnemigo> NuevoEstado)
{
    if (EstadoActual)
    {
        EstadoActual->Salir();
    }

    EstadoActual = NuevoEstado;

    if (EstadoActual)
    {
        EstadoActual->Ingresar();
    }
}

// TICK
void AEnemyBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

0    if (EstadoActual)
    {
        EstadoActual->Ejecutar(DeltaTime);
    }
}

// AL VER JUGADOR 
void AEnemyBase::AlVerJugador(APawn* JugadorVisto)
{
    if (JugadorVisto != nullptr)
    {
        // Guardamos la referencia al jugador para que los estados la usen
        ObjetivoActual = JugadorVisto;

        if (EstadoActual.GetObject() == EstadoInactivo.GetObject())
        {
            SetEstado(EstadoPersiguiendo);
        }
    }
}


// FINALIZAR ATAQUE — vuelve a perseguir
// Transiciona de Atacando → Persiguiendo si el enemigo sigue vivo.
void AEnemyBase::FinalizarAtaque()
{
    // Verificamos que no esté muerto antes de volver a perseguir
    if (EstadoActual.GetObject() != EstadoMuerto.GetObject())
    {
        SetEstado(EstadoPersiguiendo);
    }
}


// TAKE DAMAGE — recibe daño y transiciona a Muerto 
float AEnemyBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
    AController* EventInstigator, AActor* DamageCauser)
{
    // Si ya está muerto, ignoramos el daño extra
    if (EstadoActual.GetObject() == EstadoMuerto.GetObject()) return 0.0f;

    float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    Salud -= DamageToApply;

    if (Salud <= 0.0f)
    {
        // Transición al estado Muerto 
        SetEstado(EstadoMuerto);
    }

    return DamageToApply;
}

// MORIR — destruye el actor después del TiempoDesaparicion
void AEnemyBase::Morir()
{
    Destroy();
}

void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

//pra compilar esta mrd
void AEnemyBase::JugadorDerrotado()
{
    // Implementación base vacía
    // Los hijos (MiniBoss, etc.) pueden sobrescribir esto
}

// ATACAR JUGADOR — lo usan los estados de combate
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