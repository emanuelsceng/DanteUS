// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h"
#include "UIManagerFacade.generated.h"

UCLASS()
class DANTEUS_API AUIManagerFacade : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUIManagerFacade();


	// Referencia a la clase del Widget(editor)
	UPROPERTY(EditAnywhere, Category = "Dante | UI")
	TSubclassOf<UUserWidget> HUDWidgetClass;

	// Métodos simplificados que forman la Fachada
	void ActualizarBarraVida(float SaludActual, float SaludMaxima);
	void MostrarPantallaMuerte();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// El puntero al objeto Widget real en memoria
	UPROPERTY()
	UUserWidget* InstanciaHUD;

};
