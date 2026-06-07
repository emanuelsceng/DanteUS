// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EstadoPuerta.generated.h"

/**
 * 
 */
 // Forward declaration — evita include circular con PuertaPasillo
class APuertaPasillo;
UCLASS()
class DANTEUS_API UEstadoPuerta : public UObject
{
	GENERATED_BODY()
public:
    // Referencia al contexto — equivalente a SetMachine() del libro
    void SetPuerta(APuertaPasillo* Puerta) { MiPuerta = Puerta; }

    // Contrato del State — los hijos implementan esto
    virtual void AlPisarTrigger() PURE_VIRTUAL(UEstadoPuerta::AlPisarTrigger, );
    virtual void AlRecibirAviso() PURE_VIRTUAL(UEstadoPuerta::AlRecibirAviso, );

protected:
    // Puntero al contexto sin UPROPERTY porque UObject no admite
    // punteros a AActor con UPROPERTY de la misma forma que AActor
    APuertaPasillo* MiPuerta = nullptr;
	
};
