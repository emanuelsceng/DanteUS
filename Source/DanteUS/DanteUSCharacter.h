// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "AtributosCombateInterface.h"
#include "DanteUSCharacter.generated.h"



class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config = Game)
class ADanteUSCharacter : public ACharacter, public IAtributosCombateInterface
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

	/** Dodge Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* DodgeAction;

public:
	ADanteUSCharacter();

public:

	// Puntero polimórfico: Apuntará a Dante o al Decorador (Escudo) que lo envuelva
	IAtributosCombateInterface* AtributosActuales;


	// ESTADÍSTICAS DE DANTE

	// Salud actual de Dante
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante | Atributos")
	float Salud;

	// Salud máxima permitida
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante | Atributos")
	float SaludMaxima;
	//
	// Estado de vida
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante | Estado")
	bool bEstaMuerto;


	// Indica si Dante está en medio de una animación de ataque
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dante | Estado")
	bool bEstaAtacando;
	// Función para desbloquear el movimiento al terminar la animación
	UFUNCTION(BlueprintCallable, Category = "Dante | Combate")
	void FinalizarAtaque();


	// REFERENCIA Al facade DE UI
	// Usamos forward declaration con "class" para no incluir el .h completo aquí
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante | UI")
	class AUIManagerFacade* UIFacade;

	// --- AQUÍ VA EL DAÑO ---
	// Daño que hace el ataque básico de Dante
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante | Atributos")
	float DanoAtaque;
	// Función que se activa cuando Dante recibe cualquier tipo de daño
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// Función principal de ataque 
	UFUNCTION(BlueprintCallable, Category = "Dante | Combate")
	void Atacar();
	//Variable para guardar la animación del ataque 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante | Combate")
	UAnimMontage* MontageAtaque;

	// Distancia del ataque lineal (Corte frontal)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante | Combate")
	float AlcanceAtaque;

	// Evento para activar la animación en el Animation Blueprint
	UFUNCTION(BlueprintImplementableEvent, Category = "Dante | Eventos")
	void OnDanteDie();


	//
	// Componente de colisión para la espada (La Hitbox)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dante | Combate")
	class UBoxComponent* EspadaHitbox;

	// Funciones que llamaremos desde las Animaciones (Anim Notifies)
	UFUNCTION(BlueprintCallable, Category = "Dante | Combate")
	void ActivarEspada();

	UFUNCTION(BlueprintCallable, Category = "Dante | Combate")
	void DesactivarEspada();

	// Función que detecta el choque físico
	UFUNCTION()
	void AlGolpearEnemigo(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	/////
	// Implementación obligatoria de la interfaz
	virtual float CalcularDanioRecibido(float DanioEntrante) override;
	// Variable para recordar si tenemos el escudo puesto. 
	// El UPROPERTY() es VITAL para que Unreal borre el escudo de la memoria al desactivarlo (Garbage Collection).
	UPROPERTY()
	class UReliquiaEscudo* EscudoActivo;

	// Tu función que ya tenías (no le cambies el nombre)
	UFUNCTION(BlueprintCallable, Category = "Dante | Reliquias")
	void ActivarEscudo(float NivelDeProteccion);

	//Roll
	// Estado de la voltereta
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dante | Estado")
	bool bEstaEsquivando = false;

	// Montaje de animacion de dodge
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante | Combate")
	UAnimMontage* MontageDodge;

	// Funcion principal del dodge
	UFUNCTION(BlueprintCallable, Category = "Dante | Combate")
	void Esquivar();

	// Termina el dodge y restaura el estado
	UFUNCTION(BlueprintCallable, Category = "Dante | Combate")
	void FinalizarEsquiva();

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void Saltar();

	// Lógica interna de muerte
	void ProcesarMuerte();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// To add mapping context
	virtual void BeginPlay();
    
protected:
	// --- CONTROL DE TIEMPO DEL ESCUDO (RELIQUIA) ---

	// Almacena el estado de disponibilidad del escudo
	bool bPuedoActivarEscudo;

	// Manejador para controlar los 5 segundos de duración
	FTimerHandle Temporizador_DuracionEscudo;

	// Manejador para controlar los 3 minutos (180 segundos) de cooldown
	FTimerHandle Temporizador_CooldownEscudo;

	// Función que el motor llamará automáticamente a los 5 segundos para retirar el escudo
	void DesactivarEscudoPorTiempo();

	// Función que el motor llamará automáticamente a los 3 minutos para permitir un nuevo uso
	void ResetearCooldownEscudo();


public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }


	protected:
		// --- CONTROL DE TIEMPO DEL ESCUDO (RELIQUIA) ---

		// Almacena el estado de disponibilidad del escudo
		bool bPuedoActivarEscudo;

		// Manejador para controlar los 5 segundos de duración
		FTimerHandle Temporizador_DuracionEscudo;

		// Manejador para controlar los 3 minutos (180 segundos) de cooldown
		FTimerHandle Temporizador_CooldownEscudo;

		// Función que el motor llamará automáticamente a los 5 segundos para retirar el escudo
		void DesactivarEscudoPorTiempo();

		// Función que el motor llamará automáticamente a los 3 minutos para permitir un nuevo uso
		void ResetearCooldownEscudo();
};