// Dante: El Último Sello - Nivel 3
// Implementación del Siervo Vampírico

#include "SiervoVampirico.h"

ASiervoVampirico::ASiervoVampirico()
{
    SaludMaxima = 20.0f;
    Salud = SaludMaxima;
    DanoAtaque = 5.0f;
    DistanciaAtaque = 120.0f;
    RegeneracionPorGolpe = 2.0f;
}

void ASiervoVampirico::BeginPlay()
{
    Super::BeginPlay();
}

void ASiervoVampirico::EjecutarGolpeMelee()
{
    // El padre verifica la distancia y aplica el daño
    Super::EjecutarGolpeMelee();

    // Solo nos curamos si Dante sigue en rango (golpe conectado)
    if (ObjetivoActual)
    {
        float DistanciaADante = FVector::Dist(GetActorLocation(), ObjetivoActual->GetActorLocation());

        if (DistanciaADante <= (DistanciaAtaque + 50.0f))
        {
            // Golpe conectado - curacion vampirica
            Salud = FMath::Clamp(Salud + RegeneracionPorGolpe, 0.0f, SaludMaxima);

            if (GEngine)
            {
                GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red,
                    TEXT("Siervo Vampirico mordio a Dante y regenero vida!"));
            }
        }
    }
}