// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "DanteUSCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config = Game)
class ADanteUSCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

public:
	ADanteUSCharacter();

public:
	// ESTADÍSTICAS DE DANTE

	// Salud actual de Dante
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante | Atributos")
	float Salud;

	// Salud máxima permitida
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante | Atributos")
	float SaludMaxima;

	// Estado de vida
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante | Estado")
	bool bEstaMuerto;

	// --- AQUÍ VA EL DAÑO ---
	// Daño que hace el ataque básico de Dante
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante | Atributos")
	float DanoAtaque;

	// Función que se activa cuando Dante recibe cualquier tipo de daño
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// Función principal de ataque 
	UFUNCTION(BlueprintCallable, Category = "Dante | Combate")
	void Atacar();

	// Distancia del ataque lineal (Corte frontal)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante | Combate")
	float AlcanceAtaque;

	// Evento para activar la animación en el Animation Blueprint
	UFUNCTION(BlueprintImplementableEvent, Category = "Dante | Eventos")
	void OnDanteDie();

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	// Lógica interna de muerte
	void ProcesarMuerte();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// To add mapping context
	virtual void BeginPlay();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};