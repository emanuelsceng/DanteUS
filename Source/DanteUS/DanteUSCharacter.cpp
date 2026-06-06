// Copyright Epic Games, Inc. All Rights Reserved.

#include "DanteUSCharacter.h"
#include "UIManagerFacade.h" 
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyBase.h"
#include "Components/BoxComponent.h"
#include "SistemaJuegoFacade.h"
#include "ReliquiaEscudo.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ADanteUSCharacter

ADanteUSCharacter::ADanteUSCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	//vida dante
	SaludMaxima = 100.0f;
	Salud = SaludMaxima;
	bEstaMuerto = false;
	
	bEstaAtacando = false;
	//DAÑO DE DANTE
	DanoAtaque = 5.0f; // Los 5 puntos de daño lineal de dante
	AlcanceAtaque = 400.0f; // El largo de tu "espada" o rayo láser invisible

	//  dentro del constructor
	EspadaHitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("EspadaHitbox"));
	// Lo pegamos a la mano derecha (Asegúrate de que el socket se llame igual en tu esqueleto)
	EspadaHitbox->SetupAttachment(GetMesh(), TEXT("Dante_Sword"));
	EspadaHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision); // Empieza apagada
	EspadaHitbox->SetCollisionResponseToAllChannels(ECR_Ignore);
	EspadaHitbox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // Solo reacciona a enemigos


}

void ADanteUSCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
// Al inicio del juego, Dante no tiene reliquias, así que él calcula su propio daño base
	AtributosActuales = this;

	

	//  Buscamos automáticamente el facade de UI en el mundo
	AActor* FachadaEncontrada = UGameplayStatics::GetActorOfClass(GetWorld(), AUIManagerFacade::StaticClass());

	if (FachadaEncontrada)
	{
		// Si la encuentra, la guardamos en nuestra variable
		UIFacade = Cast<AUIManagerFacade>(FachadaEncontrada);

		//  Le avisamos que acabamos de nacer para que pinte la barra verde al 100%
		UIFacade->ActualizarBarraVida(Salud, SaludMaxima);
	}
	//
	EspadaHitbox->OnComponentBeginOverlap.AddDynamic(this, &ADanteUSCharacter::AlGolpearEnemigo);
}

//////////////////////////////////////////////////////////////////////////
// Input

void ADanteUSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ADanteUSCharacter::Saltar);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADanteUSCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADanteUSCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ADanteUSCharacter::Move(const FInputActionValue& Value)
{
	// Si está muerto O está atacando, ignoramos el teclado/mando
	if (bEstaMuerto || bEstaAtacando) return;
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ADanteUSCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}
float ADanteUSCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (bEstaMuerto) return 0.0f;
	////////////
	// 1. EL DECORADOR INTERCEPTA EL DAÑO: 
	// Antes era DamageAmount directo. Ahora le preguntamos a la Reliquia (o a Dante) cuánto daño pasa realmente.
	float DanioModificado = AtributosActuales->CalcularDanioRecibido(DamageAmount);

	// 2. APLICAMOS EL DAÑO FILTRADO:
	// Fíjate que ahora le pasamos 'DanioModificado' al Super en lugar de DamageAmount
	float DamageToApply = Super::TakeDamage(DanioModificado, DamageEvent, EventInstigator, DamageCauser);
	// Restamos el daño a la salud actual
	Salud -= DamageToApply;
	//
	// aqui entra el facade Le avisamos que actualice la barra
	if (UIFacade)
	{
		UIFacade->ActualizarBarraVida(Salud, SaludMaxima);
	}

	// Evitamos que la salud sea menor a 0
	if (Salud <= 0.0f)
	{
		Salud = 0.0f;

		// Le avisamos a la fachada que ponga la pantalla de Game Over
		if (UIFacade)
		{
			UIFacade->MostrarPantallaMuerte();
		}

		// Mensaje en la consola de Unreal para avisar que Dante cayó
		ProcesarMuerte();
	}

	return DamageToApply;
}


// Función que se llama cuando la salud de Dante llega a 0, para manejar la lógica de muerte (desactivar movimiento, entrada, etc)
void ADanteUSCharacter::ProcesarMuerte()
{
	if (bEstaMuerto) return;
	bEstaMuerto = true;

	UE_LOG(LogTemplateCharacter, Warning, TEXT("Dante ha muerto"));

	// Detenemos el movimiento del CharacterMovementComponent
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->StopMovementImmediately();

	// Desactivamos la entrada del PlayerController
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		DisableInput(PC);
	}

	// Llamamos al evento que dispara la animación en el Blueprint
	OnDanteDie();
	//LÓGICA DEL MEGÁFONO A LOS ENEMIGOS
	// Buscamos a TODOS los enemigos en el nivel
	TArray<AActor*> EnemigosEnMapa;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyBase::StaticClass(), EnemigosEnMapa);

	// Le avisamos a cada uno que Dante ya fue derrotado
	for (AActor* EnemigoActor : EnemigosEnMapa)
	{
		AEnemyBase* Enemigo = Cast<AEnemyBase>(EnemigoActor);
		if (Enemigo)
		{
			Enemigo->JugadorDerrotado();
		}
	}
	// Buscamos el facade del sistema de juego para avisarle que Dante fue derrotado (y así centralizar la llamada a UI, Audio, etc)
	AActor* FachadaActor = UGameplayStatics::GetActorOfClass(GetWorld(), ASistemaJuegoFacade::StaticClass());
	ASistemaJuegoFacade* Fachada = Cast<ASistemaJuegoFacade>(FachadaActor);

	if (Fachada)
	{
		// Centralizamos la llamada. Ocultamos toda la complejidad de la UI y el Audio.
		Fachada->DanteDerrotado();
	}
}



void ADanteUSCharacter::Atacar()
{
	// 1. Si está muerto o YA está atacando, no hacemos nada
	if (bEstaMuerto || bEstaAtacando) return;

	// 2. Cerramos el candado de movimiento
	bEstaAtacando = true;

	// 3. Reproducimos la animación
	if (MontageAtaque)
	{
		PlayAnimMontage(MontageAtaque);
	}
}

void ADanteUSCharacter::FinalizarAtaque()
{
	// Abrimos el candado para que Dante pueda volver a caminar
	bEstaAtacando = false;
}

// Funciones para activar y desactivar la hitbox de la espada, que se llamarán desde los Anim Notifies en las animaciones de ataque
void ADanteUSCharacter::ActivarEspada()
{
	EspadaHitbox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ADanteUSCharacter::DesactivarEspada()
{
	EspadaHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ADanteUSCharacter::AlGolpearEnemigo(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && OtherActor->IsA(AEnemyBase::StaticClass()))
	{
		// Aplicamos el daño general que ya definiste (5.0f)
		UGameplayStatics::ApplyDamage(OtherActor, DanoAtaque, GetController(), this, UDamageType::StaticClass());

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("Hitbox impactada"));
		}

		// Apagamos la colisión para no golpear 20 veces en un solo swing
		DesactivarEspada();
	}
}
void ADanteUSCharacter::Saltar()
{
	// Si está muerto o ya está atacando, bloqueamos el salto
	if (bEstaMuerto || bEstaAtacando) return;

	// Si el componente de movimiento dice que YA está en el aire (IsFalling), bloqueamos el spam
	if (GetCharacterMovement() && GetCharacterMovement()->IsFalling()) return;

	// Si pasa todas las pruebas, permitimos el salto físico de Unreal
	Jump();
}

void ADanteUSCharacter::ActivarEscudo(float NivelDeProteccion)
{
	// ¿Tenemos el escudo prendido? ¡Entonces toca APAGARLO!
	if (EscudoActivo != nullptr)
	{
		// 1. Dante vuelve a ser el dueño de su propio daño (nos quitamos el envoltorio)
		this->AtributosActuales = this;

		// 2. Vaciamos la variable. El motor de Unreal se encargará de destruir el escudo viejo.
		EscudoActivo = nullptr;

		// Mensaje en rojo para avisar que somos vulnerables
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("¡Reliquia Desactivada! Eres vulnerable."));
		}
	}
	else // ¿El escudo está apagado? ¡Entonces toca PRENDERLO!
	{
		// 1. Creamos el escudo dinámicamente en la memoria y lo guardamos en nuestra variable
		EscudoActivo = NewObject<UReliquiaEscudo>(this);

		// 2. Le asignamos la protección
		EscudoActivo->PorcentajeReduccion = NivelDeProteccion;

		// 3. El escudo "envuelve" a Dante
		EscudoActivo->InicializarDecorador(this->AtributosActuales);

		// 4. Le decimos a Dante que su nueva defensa frontal es este escudo
		this->AtributosActuales = EscudoActivo;

		// Mensaje en verde para avisar que estamos protegidos
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("¡Reliquia Activada! Daño bloqueado."));
		}
	}
}
float ADanteUSCharacter::CalcularDanioRecibido(float DanioEntrante)
{
	// Al ser el Dante base (sin decoradores encima), recibe el 100% del daño original
	return DanioEntrante;
}