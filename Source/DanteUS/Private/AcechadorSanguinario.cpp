// Fill out your copyright notice in the Description page of Project Settings.


// Dante: El Último Sello - Nivel 3
// Implementación del Acechador Sanguinario

#include "AcechadorSanguinario.h"

AAcechadorSanguinario::AAcechadorSanguinario()
{
    SaludMaxima = 20.0f;
    Salud = SaludMaxima;
    DanoAtaque = 3.0f;
    DistanciaAtaque = 120.0f;
    RegeneracionPorGolpe = 2.0f;
}

void AAcechadorSanguinario::BeginPlay()
{
    Super::BeginPlay();
}

void AAcechadorSanguinario::EjecutarGolpeMelee()
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
                GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange,
                    TEXT("Acechador: Zarpazo exitoso, vida regenerada!"));
            }
        }
    }
}
