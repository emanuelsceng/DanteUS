// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SubsistemaNivel.generated.h"

UCLASS(Abstract)
class DANTEUS_API ASubsistemaNivel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASubsistemaNivel();
	virtual void EjecutarTarea(const FString& Tarea) PURE_VIRTUAL(ASubsistemaNivel::EjecutarTarea, );
	virtual FString GetNombreSubsistema() PURE_VIRTUAL(ASubsistemaNivel::GetNombreSubsistema, return "";);
};
