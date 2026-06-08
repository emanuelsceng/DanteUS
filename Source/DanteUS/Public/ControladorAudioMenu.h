// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SubsistemaMenu.h"
#include "ControladorAudioMenu.generated.h"

/**
 * 
 */
UCLASS()
class DANTEUS_API AControladorAudioMenu : public ASubsistemaMenu
{
	GENERATED_BODY()
public:
	AControladorAudioMenu();
	virtual void InicializarSubsistema() override;
	virtual void EjecutarComandoMenu(const FString& Comando) override;
	virtual void ApagarSubsistema() override;
	virtual FString ObtenerNombreSubsistema() override;
	// Variable para guardar la pista de audio
private:
	UPROPERTY()
	class USoundBase* MusicaMenuFondo;
};
