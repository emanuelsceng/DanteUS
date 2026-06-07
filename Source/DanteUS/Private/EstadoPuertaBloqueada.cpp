// Fill out your copyright notice in the Description page of Project Settings.


#include "EstadoPuertaBloqueada.h"
#include "PuertaPasillo.h"


void UEstadoPuertaBloqueada::AlPisarTrigger()
{
    // Estado bloqueado — Dante pisa el trigger pero no pasa nada
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red,
        TEXT("Puerta: Bloqueada. Derrota a todos los enemigos primero."));
}

void UEstadoPuertaBloqueada::AlRecibirAviso()
{
    // El ArenaManager avisó que la arena está limpia
    // Transicionamos al estado abierto
    if (MiPuerta)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
            TEXT("Puerta: Arena limpia — desbloqueando pasillo."));
        MiPuerta->CambiarAEstadoAbierto();
    }
}