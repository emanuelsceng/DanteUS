// Fill out your copyright notice in the Description page of Project Settings.


#include "UIManagerFacade.h"
#include "Blueprint/UserWidget.h"
#include "Misc/OutputDeviceNull.h"

// Sets default values
AUIManagerFacade::AUIManagerFacade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AUIManagerFacade::BeginPlay()
{
	Super::BeginPlay();

    // Al iniciar, la fachada crea el HUD y lo pone en pantalla
    if (HUDWidgetClass)
    {
        InstanciaHUD = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
        if (InstanciaHUD)
        {
            InstanciaHUD->AddToViewport();
        }
    }

}

// Called every frame
void AUIManagerFacade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUIManagerFacade::ActualizarBarraVida(float SaludActual, float SaludMaxima)
{
    if (InstanciaHUD)
    {
        
        // Usamos una función genérica para que el C++ no dependa del diseño visual
        float Porcentaje = SaludActual / SaludMaxima;

        // Llamada dinámica a una función de Blueprint llamada "UpdateHealthBar"
        FOutputDeviceNull ar;
        InstanciaHUD->CallFunctionByNameWithArguments(*FString::Printf(TEXT("UpdateHealthBar %f"), Porcentaje), ar, NULL, true);
    }
}

void AUIManagerFacade::MostrarPantallaMuerte()
{
    if (InstanciaHUD)
    {
        // La fachada le ordena al HUD mostrar el Game Over
        FOutputDeviceNull ar;
        InstanciaHUD->CallFunctionByNameWithArguments(TEXT("ShowGameOverScreen"), ar, NULL, true);
    }
}