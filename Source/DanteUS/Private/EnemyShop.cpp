// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyShop.h"
#include "EnemyBase.h"    // <-- Añádelo aquí para que conozca la clase que va a spawnear

AEnemyShop::AEnemyShop(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
}

