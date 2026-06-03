// Fill out your copyright notice in the Description page of Project Settings.


// Dante: El Último Sello - Nivel 3
// Implementación del Mini-Boss del Sello del Hambre

#include "MiniBossHambre.h"
#include "Animation/AnimInstance.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

static bool bInvocoFase1 = false;
static bool bInvocoFase2 = false;
static bool bInvocoFase3 = false;

AMiniBossHambre::AMiniBossHambre()
{
    PrimaryActorTick.bCanEverTick = true;

    // Estadisticas segun documento Nivel 3
    SaludMaxima = 75.0f;
    Salud = SaludMaxima;
    DanoAtaque = 15.0f;

    // El ataque es a distancia, rango amplio
    DistanciaAtaque = 600.0f;

    RegeneracionPorGolpe = 5.0f;

    // Estado inicial
    EstadoMiniBoss = EEstadoMiniBoss::Rondando;

    GetCharacterMovement()->MaxWalkSpeed = 350.0f;
}

void AMiniBossHambre::BeginPlay()
{
    Super::BeginPlay();


    // Buscamos a Dante directamente al iniciar
    // porque AlVerJugador de EnemyBase no funciona con nuestro enum
    APawn* Dante = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (Dante)
    {
        ObjetivoActual = Dante;

    }
    // Animacion de aparicion al iniciar
    UAnimInstance* AnimInstancia = GetMesh()->GetAnimInstance();
    if (AnimInstancia && MontajeAparicion)
    {
        AnimInstancia->Montage_Play(MontajeAparicion);
    }
}

void AMiniBossHambre::Tick(float DeltaTime)
{
    AActor::Tick(DeltaTime);

    if (EstadoMiniBoss == EEstadoMiniBoss::Muerto) return;

    // Si esta atacando o invocando no actualizamos el movimiento
    // El Mini-Boss se queda quieto durante esas acciones
    if (EstadoMiniBoss == EEstadoMiniBoss::Atacando ||
        EstadoMiniBoss == EEstadoMiniBoss::Invocando) return;
    UE_LOG(LogTemp, Warning, TEXT("TICK - Estado: %d ObjetivoActual: %s"),
        (int32)EstadoMiniBoss,
        ObjetivoActual ? TEXT("SI") : TEXT("NO"));

    ActualizarMovimiento(DeltaTime);
}

void AMiniBossHambre::ActualizarMovimiento(float DeltaTime)
{
    if (!ObjetivoActual) return;

    AAIController* ControladorIA = Cast<AAIController>(GetController());
    if (!ControladorIA) return;

    float DistanciaADante = FVector::Dist(GetActorLocation(), ObjetivoActual->GetActorLocation());

    if (DistanciaADante < DistanciaMinima)
    {
        // Dante MUY CERCA: retrocedemos
        EstadoMiniBoss = EEstadoMiniBoss::Retrocediendo;
        Retroceder();
    }
    else if (DistanciaADante <= DistanciaAtaque)

    {
        UE_LOG(LogTemp, Warning, TEXT("EN RANGO! Estado actual: %d"), (int32)EstadoMiniBoss);

        // Dante en rango de ataque
        // Solo atacamos si estamos Rondando — evita llamar AtacarJugador cada frame
        if (EstadoMiniBoss == EEstadoMiniBoss::Rondando)
        {
            EstadoMiniBoss = EEstadoMiniBoss::Atacando;
            ControladorIA->StopMovement();
            AtacarJugador();
        }
    }
    else
    {
        // Dante lejos: nos acercamos
        EstadoMiniBoss = EEstadoMiniBoss::Rondando;
        ControladorIA->MoveToActor(ObjetivoActual, DistanciaOptima);
    }
}

void AMiniBossHambre::AtacarJugador()
{
    // Girar hacia Dante antes de atacar
    FVector DireccionADante = ObjetivoActual->GetActorLocation() - GetActorLocation();
    DireccionADante.Z = 0;
    SetActorRotation(DireccionADante.Rotation());
    if (!ObjetivoActual) return;

    // Primero reproducimos la animacion
    UAnimInstance* AnimInstancia = GetMesh()->GetAnimInstance();
    if (AnimInstancia && MontajeAtaque)
    {
        AnimInstancia->Montage_Play(MontajeAtaque);
    }

    // Lanzamos el proyectil despues de 0.5 segundos
    // para que coincida con el momento del lanzamiento en la animacion
    FTimerHandle TimerProyectil;
    GetWorldTimerManager().SetTimer(
        TimerProyectil,
        this,
        &AMiniBossHambre::LanzarProyectil,
        0.5f,
        false
    );

    Salud = FMath::Clamp(Salud + RegeneracionPorGolpe, 0.0f, SaludMaxima);

    GetWorldTimerManager().SetTimer(
        TemporizadorAtaque,
        this,
        &AMiniBossHambre::VolverARondear,
        2.0f,
        false
    );
}
void AMiniBossHambre::LanzarProyectil()
{
    if (!ClaseProyectil || !ObjetivoActual) return;
    UE_LOG(LogTemp, Warning, TEXT("LanzarProyectil llamado - ClaseProyectil: %s - ObjetivoActual: %s"),
        ClaseProyectil ? TEXT("SI") : TEXT("NO"),
        ObjetivoActual ? TEXT("SI") : TEXT("NO"));

    // Posicion de spawn frente al Mini-Boss a altura del pecho
    FVector PosicionSpawn = GetActorLocation() +
        GetActorForwardVector() * 50.0f +
        FVector(0, 0, 50.0f);

    // Apuntamos hacia Dante
    FVector Direccion = ObjetivoActual->GetActorLocation() - PosicionSpawn;
    Direccion.Normalize();
    FRotator Rotacion = Direccion.Rotation();

    FActorSpawnParameters Params;
    Params.Owner = this;
    Params.Instigator = GetInstigator();
    Params.SpawnCollisionHandlingOverride =
        ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    GetWorld()->SpawnActor<AActor>(ClaseProyectil, PosicionSpawn, Rotacion, Params);

    UE_LOG(LogTemp, Log, TEXT("MiniBossHambre: Proyectil lanzado hacia Dante"));
}

void AMiniBossHambre::Retroceder()
{
    AAIController* ControladorIA = Cast<AAIController>(GetController());
    if (!ControladorIA || !ObjetivoActual) return;

    // Direccion opuesta a Dante
    FVector DireccionRetroceso = GetActorLocation() - ObjetivoActual->GetActorLocation();
    DireccionRetroceso.Normalize();

    FVector PuntoRetroceso = GetActorLocation() + DireccionRetroceso * 400.0f;
    ControladorIA->MoveToLocation(PuntoRetroceso, 50.0f);

    // Vuelve a rondar despues de retroceder
    GetWorldTimerManager().SetTimer(
        TimerRetroceder,
        this,
        &AMiniBossHambre::VolverARondear,
        1.0f,
        false
    );

    UE_LOG(LogTemp, Log, TEXT("MiniBossHambre: Dante demasiado cerca, retrocediendo..."));
}

void AMiniBossHambre::VolverARondear()
{
    if (EstadoMiniBoss != EEstadoMiniBoss::Muerto)
    {
        EstadoMiniBoss = EEstadoMiniBoss::Rondando;
    }
}

void AMiniBossHambre::InvocarEnemigos()
{
    if (EstadoMiniBoss == EEstadoMiniBoss::Muerto) return;
    if (!ClaseEnemigoInvocado) return;

    EstadoMiniBoss = EEstadoMiniBoss::Invocando;

    UE_LOG(LogTemp, Log, TEXT("MiniBossHambre: Invocando %d enemigos..."), CantidadInvocados);

    for (int32 i = 0; i < CantidadInvocados; i++)
    {
        // Aparecen en arco alrededor del Mini-Boss (teletransporte)
        float Angulo = (360.0f / CantidadInvocados) * i;
        FVector Offset = FVector(
            FMath::Cos(FMath::DegreesToRadians(Angulo)) * 300.0f,
            FMath::Sin(FMath::DegreesToRadians(Angulo)) * 300.0f,
            0.0f
        );

        FVector PosicionSpawn = GetActorLocation() + Offset;

        FActorSpawnParameters Params;
        Params.SpawnCollisionHandlingOverride =
            ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        GetWorld()->SpawnActor<AEnemyBase>(
            ClaseEnemigoInvocado,
            PosicionSpawn,
            FRotator::ZeroRotator,
            Params
        );
    }

    // Vuelve a rondar despues de invocar
    GetWorldTimerManager().SetTimer(
        TimerRetroceder,
        this,
        &AMiniBossHambre::VolverARondear,
        1.5f,
        false
    );
}

void AMiniBossHambre::Morir()
{
    EstadoMiniBoss = EEstadoMiniBoss::Muerto;

    // Cancelamos todos los timers
    GetWorldTimerManager().ClearTimer(TimerInvocacion);
    GetWorldTimerManager().ClearTimer(TimerRetroceder);
    GetWorldTimerManager().ClearTimer(TemporizadorAtaque);

    UE_LOG(LogTemp, Log, TEXT("MiniBossHambre: Derrotado."));

    // TODO: Spawnar el Caliz de Abundancia aqui

    // Animacion de muerte
    UAnimInstance* AnimInstanciaMuerte = GetMesh()->GetAnimInstance();
    if (AnimInstanciaMuerte && MontajeMuerte)
    {
        AnimInstanciaMuerte->Montage_Play(MontajeMuerte);
    }

    // Esperamos que termine la animacion antes de destruir
    GetWorldTimerManager().SetTimer(
        TimerRetroceder,
        [this]() { Super::Morir(); },
        2.0f,
        false
    );
    return; // No llamamos Super::Morir() todavia

    Super::Morir();
}
float AMiniBossHambre::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    // 1. Dejamos que la clase base reste la salud y maneje la muerte matemática
    float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    // Si el golpe lo mató, no hace falta que spawnee nada
    if (EstadoMiniBoss == EEstadoMiniBoss::Muerto) return DamageApplied;

    // 2. Revisamos los hitos de vida basados en su Salud Maxima (75 HP)
    // Fase 1: Menos del 75% de vida (56.25 HP)
    if (Salud <= (SaludMaxima * 0.75f) && !bInvocoFase1)
    {
        bInvocoFase1 = true;
        InvocarEnemigos();
    }
    // Fase 2: Menos del 50% de vida (37.5 HP)
    else if (Salud <= (SaludMaxima * 0.50f) && !bInvocoFase2)
    {
        bInvocoFase2 = true;
        InvocarEnemigos();
    }
    // Fase 3: Menos del 25% de vida (18.75 HP)
    else if (Salud <= (SaludMaxima * 0.25f) && !bInvocoFase3)
    {
        bInvocoFase3 = true;
        InvocarEnemigos();
    }

    return DamageApplied;
}