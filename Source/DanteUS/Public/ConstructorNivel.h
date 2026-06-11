// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ConstructorNivel.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UConstructorNivel : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DANTEUS_API IConstructorNivel
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    // Pasos de construcción del nivel
    virtual void ConstruirConfiguracionBase() = 0;
    virtual void ConstruirEnemigosComunes() = 0;
    virtual void ConstruirMiniBoss() = 0;
    virtual void ConstruirBoss() = 0;

    // Retorna el producto construido 
    virtual class ANivelDante* ObtenerNivel() = 0;
public:
};
