// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SubsistemaNivel.h"
#include "SubsistemaAudio.generated.h"

/**
 * 
 */
UCLASS()
class DANTEUS_API ASubsistemaAudio : public ASubsistemaNivel
{
	GENERATED_BODY()
public:
	ASubsistemaAudio();
	virtual void EjecutarTarea(const FString& Tarea) override;
	virtual FString GetNombreSubsistema() override;
private:
	// Variable para guardar nuestra pista de música
	UPROPERTY()
	class USoundBase* MusicaGameOver;
	
};
