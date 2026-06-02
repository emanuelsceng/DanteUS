#include "ReliquiaEscudo.h"

float UReliquiaEscudo::CalcularDanioRecibido(float DanioEntrante)
{
	// 1. Calculamos cuánto daño absorbe el escudo (Ej: 30 de daño * 1.0 = 30 absorbidos)
	float DanioAbsorbido = DanioEntrante * PorcentajeReduccion;

	// 2. Calculamos cuánto daño logró traspasar (Ej: 30 - 30 = 0 daño restante)
	float DanioRestante = DanioEntrante - DanioAbsorbido;

	// 3. Le pasamos el daño restante a la capa de abajo (Dante)
	if (ObjetoEnvuelto)
	{
		return ObjetoEnvuelto->CalcularDanioRecibido(DanioRestante);
	}

	return DanioRestante;
}