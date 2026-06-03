// Dante: El Ultimo Sello - Nivel 4

#include "ProyectilVeneno.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

AProyectilVeneno::AProyectilVeneno()
{
    // Desactivamos el tiempo de vida automatico del padre
    // El veneno se destruye solo cuando termina de aplicar todos los ticks
    InitialLifeSpan = 0.0f;

    // Configuracion del veneno
    CantidadTicks = 4;
    IntervaloTick = 0.7f;



    // NUEVO: Evitar que el componente destruya el actor al detenerse// para los ticks
    MovimientoProyectil->bShouldBounce = false;
    //MovimientoProyectil->bSimulationEnabled = false; (esto hacia que el proyectil se trabe al lanzarlo, lo cambie a if (MovimientoProyectil))

    // Estado inicial
    bYaImpactoVeneno = false;
    DanteEnvenenado = nullptr;
    TicksRestantes = 0;
    DanoPorTick = 0.0f;
}

void AProyectilVeneno::BeginPlay()
{
    // Llamamos al padre para que configure todo el proyectil
    Super::BeginPlay();

    // Forzamos en runtime que nunca se destruya automaticamente
    // Esto sobreescribe cualquier valor que tenga el Blueprint guardado para que complete los 4 ticks del veneno y no se interrumpa
    SetLifeSpan(0.0f);



    // Detecta QUIEN destruye el actor
    OnDestroyed.AddDynamic(this, &AProyectilVeneno::AlSerDestruido);//verificar borrar luego

    // Quitamos el overlap del padre y ponemos el nuestro
    // Esto evita que AProyectilPocion aplique su logica de impacto
    if (ColisionEsfera)
    {
        ColisionEsfera->OnComponentBeginOverlap.RemoveAll(this);
        ColisionEsfera->OnComponentBeginOverlap.AddDynamic(
            this, &AProyectilVeneno::AlSuperponerseVeneno);
    }
}

void AProyectilVeneno::AlSuperponerseVeneno(
    UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    // Filtros de seguridad (igual que el padre)
    if (bYaImpactoVeneno)                                     return;
    if (OtherActor == GetOwner())                             return;
    if (OtherActor && OtherActor->IsA(AProyectilPocion::StaticClass())) return;

    // Si choca con pared o suelo, se destruye normal
    if (!OtherActor || !OtherActor->IsA(ACharacter::StaticClass()))
    {
        Destroy();
        return;
    }

    // --- IMPACTO CONTRA DANTE ---
    bYaImpactoVeneno = true;
    DanteEnvenenado = OtherActor;

    // Calculamos el dano: mitad inmediata, mitad en ticks
    float DanoInmediato = Dano / 2.0f;
    DanoPorTick = DanoInmediato / CantidadTicks;
    TicksRestantes = CantidadTicks;

    // Aplicamos el dano inmediato
    UGameplayStatics::ApplyDamage(
        DanteEnvenenado, DanoInmediato,
        nullptr, this, UDamageType::StaticClass());

    UE_LOG(LogTemp, Warning,
        TEXT("VENENO: Impacto! Dano inmediato=%.1f | DanoPorTick=%.1f | Ticks=%d"),
        DanoInmediato, DanoPorTick, CantidadTicks);

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green,
            FString::Printf(TEXT("PROYECTIL VENENO! Dante recibe %.0f de danio inmediato"), DanoInmediato));
    }

    // Apagamos el proyectil visualmente (ya no se ve ni colisiona)
    if (ColisionEsfera)
        ColisionEsfera->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    if (MeshPocion)
        MeshPocion->SetVisibility(false);



    // NUEVO: Detenemos completamente el movimiento para que no destruya el actor// para los ticks
    if (MovimientoProyectil)
    {
        MovimientoProyectil->StopMovementImmediately();
        MovimientoProyectil->Deactivate();
        MovimientoProyectil->SetComponentTickEnabled(false); // NUEVO
        MovimientoProyectil->bSimulationEnabled = false; // Solo aquí, post impacto
    }



    // Iniciamos el timer del veneno progresivo
    GetWorldTimerManager().SetTimer(
        TimerVeneno,
        this,
        &AProyectilVeneno::AplicarTickVeneno,
        IntervaloTick,
        true   // Se repite automaticamente cada IntervaloTick segundos
    );
}

void AProyectilVeneno::AplicarTickVeneno()
{
    // Si Dante murio o ya no hay ticks, terminamos
    if (!IsValid(DanteEnvenenado) || TicksRestantes <= 0)     // IsValid() detecta punteros nulos Y actores pendientes de destruccion(antes era solo danteEnvenenado)
    {
        GetWorldTimerManager().ClearTimer(TimerVeneno);
        Destroy();
        return;
    }

    // Aplicamos el dano progresivo
    UGameplayStatics::ApplyDamage(
        DanteEnvenenado, DanoPorTick,
        nullptr, this, UDamageType::StaticClass());

    TicksRestantes--;

    UE_LOG(LogTemp, Warning,
        TEXT("VENENO: Tick aplicado! Dano=%.1f | TicksRestantes=%d"),
        DanoPorTick, TicksRestantes);

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor(0, 160, 0),
            FString::Printf(TEXT("Dante sufre veneno... (-%.0f HP) [%d restantes]"),
                DanoPorTick, TicksRestantes));
    }

    // Si era el ultimo tick, destruimos el actor
    if (TicksRestantes <= 0)
    {
        GetWorldTimerManager().ClearTimer(TimerVeneno);
        Destroy();
    }
}



//eliminar luego
void AProyectilVeneno::AlSerDestruido(AActor* ActorDestruido)
{
    UE_LOG(LogTemp, Error,
        TEXT("=== VENENO DESTRUIDO === TicksRestantes=%d | TimerActivo=%d"),
        TicksRestantes,
        GetWorldTimerManager().IsTimerActive(TimerVeneno));
}