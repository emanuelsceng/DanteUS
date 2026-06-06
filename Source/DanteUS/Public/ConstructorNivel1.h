// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ConstructorNivel.h"
#include "ConstructorNivel1.generated.h"

UCLASS()
class DANTEUS_API AConstructorNivel1 : public AActor, public IConstructorNivel
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AConstructorNivel1();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	UPROPERTY()
	class ANivelDante* Nivel;

public:	
	virtual void ConstruirConfiguracionBase() override;
	virtual void ConstruirEnemigosComunes()    override;
	virtual void ConstruirMiniBoss()           override;
	virtual void ConstruirBoss()               override;
	virtual ANivelDante* ObtenerNivel()        override;

};
