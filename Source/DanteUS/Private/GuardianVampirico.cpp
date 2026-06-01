#include "GuardianVampirico.h"
#include "AIController.h"

AGuardianVampirico::AGuardianVampirico()
{
    SaludMaxima = 30.0f;
    Salud = SaludMaxima;
    DanoAtaque = 5.0f;
    DistanciaAtaque = 120.0f;
    RegeneracionPorGolpe = 4.0f;
}

void AGuardianVampirico::BeginPlay()
{
    Super::BeginPlay();
}

void AGuardianVampirico::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Detener movimiento cuando esta atacando
    if (EstadoActual == EEstadoEnemigo::Atacando)
    {
        AAIController* ControladorIA = Cast<AAIController>(GetController());
        if (ControladorIA)
        {
            ControladorIA->StopMovement();
        }
    }
}

void AGuardianVampirico::EjecutarGolpeMelee()
{
    // El padre verifica la distancia y aplica el daño
    Super::EjecutarGolpeMelee();

    // Solo nos curamos si Dante sigue en rango (golpe conectado)
    if (ObjetivoActual)
    {
        float DistanciaADante = FVector::Dist(GetActorLocation(), ObjetivoActual->GetActorLocation());

        if (DistanciaADante <= (DistanciaAtaque + 50.0f))
        {
            // Golpe conectado - curacion vampirica mejorada
            Salud = FMath::Clamp(Salud + RegeneracionPorGolpe, 0.0f, SaludMaxima);

            if (GEngine)
            {
                GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Purple,
                    TEXT("Guardian Vampirico: Golpe pesado conectado, vida regenerada!"));
            }
        }
    }
}