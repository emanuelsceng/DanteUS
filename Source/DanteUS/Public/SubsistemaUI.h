// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SubsistemaNivel.h"
#include "SubsistemaUI.generated.h"

/**
 * 
 */
UCLASS()
class DANTEUS_API ASubsistemaUI : public ASubsistemaNivel
{
	GENERATED_BODY()
public:
	virtual void EjecutarTarea(const FString& Tarea) override;
	virtual FString GetNombreSubsistema() override;
};
