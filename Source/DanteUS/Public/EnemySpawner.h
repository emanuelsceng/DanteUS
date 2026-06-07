// Fill out your copyright notice in the Description page of Project Settings.

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h" 
#include "EnemyShop.h"
#include "Components/BoxComponent.h"
#include "EnemySpawner.generated.h"

//para el delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnArenaLimpia);

class AEnemyShop; // Forward declaration de seguridad

UCLASS()
class DANTEUS_API AEnemySpawner : public AActor
{
	GENERATED_BODY()

public:
	AEnemySpawner();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Spawner")
	UBoxComponent* ZonaActivacion;

	// Puntero genérico a la interfaz de la tienda
	UPROPERTY(EditAnywhere, Category = "Spawner | Config")
	AEnemyShop* TiendaLocal;

	// El rol que pedirá el spawner
	UPROPERTY(EditAnywhere, Category = "Spawner | Config")
	ERolEnemigo RolA_Spawnear;

	UPROPERTY(EditAnywhere, Category = "Spawner | Config")
	int32 Cantidad = 3;

	UFUNCTION()
	void AlEntrarEnZona(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

private:
	bool bYaSeActivo = false;
	//anadi esto, contador y delegate
	int32 EnemigosVivos = 0;

public:
	UPROPERTY(BlueprintAssignable, Category = "Spawner | Observer")
	FOnArenaLimpia OnArenaLimpia;

	void NotificarEnemyMuerto();
};