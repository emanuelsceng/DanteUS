// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EstadoPuerta.h"
#include "EstadoPuertaBloqueada.generated.h"

/**
 * 
 */
UCLASS()
class DANTEUS_API UEstadoPuertaBloqueada : public UEstadoPuerta
{
	GENERATED_BODY()
public:
	virtual void AlPisarTrigger() override;
	virtual void AlRecibirAviso() override;
};
