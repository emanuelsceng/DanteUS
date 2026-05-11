// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyFactory.h" // Necesario para reconocer el tipo de enemigo (Enum)
#include "Components/BoxComponent.h"
#include "EnemySpawner.generated.h"

UCLASS()
class DANTEUS_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    //  La "zona invisible" que detecta a Dante
    UPROPERTY(VisibleAnywhere, Category = "Spawner")
    UBoxComponent* ZonaActivacion;

    // Variables configurables en el editor (Nivel 1, Nivel 2, etc.)
    UPROPERTY(EditAnywhere, Category = "Spawner")
    ETipoEnemigo TipoA_Spawnear;

    UPROPERTY(EditAnywhere, Category = "Spawner")
    int32 Cantidad = 3;
    

    // 3. Función que se activa cuando alguien "pisa" la caja
    UFUNCTION()
    void AlEntrarEnZona(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
    bool bYaSeActivo = false;
};
