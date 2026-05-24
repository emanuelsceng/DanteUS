#include "EnemigoEscupidor.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

AEnemigoEscupidor::AEnemigoEscupidor()
{
    // --- DISTANCIA (EL SENSOR) ---
    // DistanciaParaAtacar: El enemigo "ve" a Dante y ataca hasta a 2000 unidades.
    // DistanciaParaHuir: Si Dante se acerca a menos de 800, el enemigo retrocede.
    this->DistanciaParaAtacar = 2000.0f;
    this->DistanciaParaHuir = 800.0f;

    this->bHuyeDelJugador = true;
    this->TamanoPiscina = 5;

    // Estadísticas
    SaludMaxima = 15.0f;
    Salud = SaludMaxima;

    // Cooldown
    this->UltimoTiempoDisparo = 0.0f;
    this->TiempoEntreDisparos = 1.5f;
}

void AEnemigoEscupidor::AtacarJugador()
{
    // Cooldown
    float TiempoActual = GetWorld()->GetTimeSeconds();
    if (TiempoActual - UltimoTiempoDisparo < TiempoEntreDisparos) return;
    UltimoTiempoDisparo = TiempoActual;

    ACharacter* Dante = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (!Dante) return;

    // Mirar a Dante
    FVector DireccionHaciaDante = Dante->GetActorLocation() - GetActorLocation();
    FRotator RotacionMirada = FRotator(0.0f, DireccionHaciaDante.Rotation().Yaw, 0.0f);
    SetActorRotation(RotacionMirada);

    FVector Origen = GetActorLocation() + (GetActorForwardVector() * 100.0f) + FVector(0.0f, 0.0f, 60.0f);
    FRotator RotacionDisparo = (Dante->GetActorLocation() - Origen).Rotation();

    // --- VELOCIDAD DE LA BALA ---
    // Bajamos de 3000 a 1500 para que sea una bala rápida pero visible y esquivable.
    // Mantenemos 0.0f en gravedad para que sea recta.
    EjecutarDisparo(Origen, RotacionDisparo, 1500.0f, 0.0f);
}