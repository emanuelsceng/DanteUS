#include "EnemigoEscupidor.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h" // Necesario para buscar a Dante

AEnemigoEscupidor::AEnemigoEscupidor()
{
    SaludMaxima = 15.0f; // Cañón de cristal: menos vida que el zombie común
    Salud = SaludMaxima;
    DanoAtaque = 3.0f;
    DistanciaAtaque = 1500.0f; // ¡Aumento masivo de distancia!
}
void AEnemigoEscupidor::AtacarJugador()
{
    // Verificamos que el Blueprint tenga un proyectil asignado
    if (ClaseProyectilVomito)
    {
        // 1. Declaramos las variables de posición
        FVector UbicacionCentro = GetActorLocation();
        FVector HaciaAdelante = GetActorForwardVector();

        // 2. Calculamos el punto de origen de la bala desplazado hacia adelante
        FVector PuntoDeDisparo = UbicacionCentro + (HaciaAdelante * 100.0f) + FVector(0.0f, 0.0f, 50.0f);

        // 3. Buscamos a Dante en el nivel
        ACharacter* Dante = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
        FRotator RotacionDisparo = GetActorRotation(); // Rotación por defecto de seguridad

        // 4. Si Dante existe, calculamos la puntería exacta (Pitch y Yaw)
        if (Dante)
        {
            FVector UbicacionDante = Dante->GetActorLocation();
            RotacionDisparo = (UbicacionDante - PuntoDeDisparo).Rotation();
        }

        // 5. Instanciamos el proyectil
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.Instigator = GetInstigator();

        GetWorld()->SpawnActor<AActor>(ClaseProyectilVomito, PuntoDeDisparo, RotacionDisparo, SpawnParams);
    }

    // -------------------------------------------------------------------------
    // LA SOLUCIÓN MÁGICA DE LA MÁQUINA DE ESTADOS
    // Usamos el temporizador de la clase padre para llamar a FinalizarAtaque.
    // Le puse 2.0 segundos de recarga porque el Escupidor ataca a distancia.
    // -------------------------------------------------------------------------
    GetWorldTimerManager().SetTimer(TemporizadorAtaque, this, &AEnemyBase::FinalizarAtaque, 5.0f, false);
}