#include "BossGuerra.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "TimerManager.h"
#include "NiagaraFunctionLibrary.h"
#include "ProyectilBase.h"
#include "GameFramework/CharacterMovementComponent.h"

ABossGuerra::ABossGuerra()
{
    PrimaryActorTick.bCanEverTick = true;
    SaludMaxima = 150.0f;
    Salud = SaludMaxima;

    // Distancias para que NO se pegue a Dante
    DistanciaAtaque = 1500.0f;
    DistanciaOptima = 1000.0f;
    DistanciaMinima = 500.0f;

    EstadoBoss = EEstadoBossGuerra::Rondando;
    ContadorGolpes = 0;
}

void ABossGuerra::BeginPlay()
{
    Super::BeginPlay();
    ObjetivoActual = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

void ABossGuerra::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (EstadoBoss == EEstadoBossGuerra::Muerto || EstadoBoss == EEstadoBossGuerra::Atacando || EstadoBoss == EEstadoBossGuerra::Retrocediendo) return;
    ActualizarMovimiento(DeltaTime);
}

void ABossGuerra::ActualizarMovimiento(float DeltaTime)
{
    if (!ObjetivoActual) return;
    AAIController* ControladorIA = Cast<AAIController>(GetController());
    if (!ControladorIA) return;

    float Distancia = FVector::Dist(GetActorLocation(), ObjetivoActual->GetActorLocation());

    if (Distancia < DistanciaMinima) {
        EstadoBoss = EEstadoBossGuerra::Retrocediendo;
        Retroceder();
    }
    else if (Distancia <= DistanciaAtaque) {
        EstadoBoss = EEstadoBossGuerra::Atacando;
        ControladorIA->StopMovement();
        AtacarJugador();
    }
    else {
        EstadoBoss = EEstadoBossGuerra::Rondando;
        ControladorIA->MoveToActor(ObjetivoActual, DistanciaOptima);
    }
}

void ABossGuerra::Retroceder()
{
    AAIController* ControladorIA = Cast<AAIController>(GetController());
    if (!ControladorIA || !ObjetivoActual) return;
    FVector Direccion = (GetActorLocation() - ObjetivoActual->GetActorLocation()).GetSafeNormal();
    ControladorIA->MoveToLocation(GetActorLocation() + Direccion * 1200.0f);
    GetWorldTimerManager().SetTimer(TimerRetroceder, this, &ABossGuerra::VolverARondear, 1.5f, false);
}

void ABossGuerra::VolverARondear() { EstadoBoss = EEstadoBossGuerra::Rondando; }

void ABossGuerra::AtacarJugador()
{
    if (MontageAtaque) PlayAnimMontage(MontageAtaque);
    FTimerHandle TimerDisparo;
    GetWorldTimerManager().SetTimer(TimerDisparo, this, &ABossGuerra::LanzarProyectil, 0.5f, false);
    GetWorldTimerManager().SetTimer(TimerRetroceder, this, &ABossGuerra::VolverARondear, 3.0f, false);
}

void ABossGuerra::LanzarProyectil()
{
    if (!ClaseProyectil || !ObjetivoActual) return;
    FVector Origen = GetActorLocation() + GetActorForwardVector() * 200.0f + FVector(0, 0, 60);
    FRotator Rot = (ObjetivoActual->GetActorLocation() - Origen).Rotation();
    AProyectilBase* P = GetWorld()->SpawnActor<AProyectilBase>(ClaseProyectil, Origen, Rot);
    if (P) P->Disparar(Rot.Vector(), 2000.0f, 0.0f);
}

float ABossGuerra::TakeDamage(float D, FDamageEvent const& E, AController* C, AActor* A)
{
    float Ap = Super::TakeDamage(D, E, C, A);
    if (++ContadorGolpes >= 5) { DetonarExplosionRechazo(); ContadorGolpes = 0; }
    return Ap;
}

void ABossGuerra::DetonarExplosionRechazo()
{
    if (FX_ExplosionCeniza) UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), FX_ExplosionCeniza, GetActorLocation());
    UGameplayStatics::ApplyRadialDamage(this, 15.0f, GetActorLocation(), 400.0f, UDamageType::StaticClass(), TArray<AActor*>(), this);
}