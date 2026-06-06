// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SubsistemaMenu.h"
#include "ControladorUIMenu.generated.h"

/**
 * 
 */
UCLASS()
class DANTEUS_API AControladorUIMenu : public ASubsistemaMenu
{
	GENERATED_BODY()
public:
	// Cumplimos con el contrato estricto sobrescribiendo (override) todo el ciclo de vida
	virtual void InicializarSubsistema() override;
	virtual void EjecutarComandoMenu(const FString& Comando) override;
	virtual void ApagarSubsistema() override;
	virtual FString ObtenerNombreSubsistema() override;
	
};
