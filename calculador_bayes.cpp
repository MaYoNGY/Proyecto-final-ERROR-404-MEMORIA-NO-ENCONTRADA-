#include "juego.h"

CalculadorBayes::CalculadorBayes() {
    // Probabilidades iniciales basadas en la dificultad
    probabilidadExitoFacil = 0.8;   // 80% de éxito en modo fácil
    probabilidadExitoDificil = 0.4; // 40% de éxito en modo difícil
}

double CalculadorBayes::calcularProbabilidadCondicional(const Jugador& jugador, TipoDificultad dificultad) const {
    // P(Éxito|Dificultad, Historial)
    
    double tasaExitoJugador = jugador.getTasaExito();
    std::vector<bool> historial = jugador.getHistorialAciertos();
    
    if (historial.empty()) {
        // Sin historial, usar probabilidades base
        return (dificultad == FACIL) ? probabilidadExitoFacil : probabilidadExitoDificil;
    }
    
    // Aplicar teorema de Bayes para ajustar según historial
    double probabilidadBase = (dificultad == FACIL) ? probabilidadExitoFacil : probabilidadExitoDificil;
    
    // Factor de ajuste basado en el rendimiento histórico del jugador
    double factorAjuste = tasaExitoJugador / 0.6; // 0.6 es la tasa promedio esperada
    
    // Calcular probabilidad posterior
    double probabilidadPosterior = probabilidadBase * factorAjuste;
    
    // Limitar entre 0.1 y 0.9 para evitar certezas absolutas
    if (probabilidadPosterior < 0.1) probabilidadPosterior = 0.1;
    if (probabilidadPosterior > 0.9) probabilidadPosterior = 0.9;
    
    return probabilidadPosterior;
}

void CalculadorBayes::actualizarProbabilidades(const std::vector<Jugador>& jugadores) {
    if (jugadores.empty()) return;
    
    double sumaExitoFacil = 0.0, sumaExitoDificil = 0.0;
    int contadorFacil = 0, contadorDificil = 0;
    
    // Recopilar estadísticas de todos los jugadores
    for (const Jugador& jugador : jugadores) {
        std::vector<bool> historial = jugador.getHistorialAciertos();
        
        // Para simplificar, asumir distribución equitativa de dificultades
        for (size_t i = 0; i < historial.size(); i++) {
            if (i % 2 == 0) { // Suponer que índices pares son fácil
                sumaExitoFacil += historial[i] ? 1.0 : 0.0;
                contadorFacil++;
            } else { // Índices impares son difícil
                sumaExitoDificil += historial[i] ? 1.0 : 0.0;
                contadorDificil++;
            }
        }
    }
    
    // Actualizar probabilidades con suavizado
    if (contadorFacil > 0) {
        double nuevaProbFacil = sumaExitoFacil / contadorFacil;
        probabilidadExitoFacil = 0.7 * probabilidadExitoFacil + 0.3 * nuevaProbFacil;
    }
    
    if (contadorDificil > 0) {
        double nuevaProbDificil = sumaExitoDificil / contadorDificil;
        probabilidadExitoDificil = 0.7 * probabilidadExitoDificil + 0.3 * nuevaProbDificil;
    }
}

double CalculadorBayes::calcularProbabilidadPosterior(double probabilidadPrevia, bool evidencia, TipoDificultad dificultad) const {
    // P(Hipótesis|Evidencia) = P(Evidencia|Hipótesis) * P(Hipótesis) / P(Evidencia)
    
    double probabilidadEvidencia = (dificultad == FACIL) ? probabilidadExitoFacil : probabilidadExitoDificil;
    
    if (evidencia) {
        // Evidencia positiva (éxito)
        return (probabilidadEvidencia * probabilidadPrevia) / probabilidadEvidencia;
    } else {
        // Evidencia negativa (fallo)
        double probFallo = 1.0 - probabilidadEvidencia;
        return (probFallo * probabilidadPrevia) / probFallo;
    }
}

void CalculadorBayes::mostrarProbabilidades() const {
    std::cout << "\n=== PROBABILIDADES BAYESIANAS ===\n";
    std::cout << "P(Éxito|Fácil) = " << (probabilidadExitoFacil * 100) << "%\n";
    std::cout << "P(Éxito|Difícil) = " << (probabilidadExitoDificil * 100) << "%\n";
    std::cout << "P(Fallo|Fácil) = " << ((1 - probabilidadExitoFacil) * 100) << "%\n";
    std::cout << "P(Fallo|Difícil) = " << ((1 - probabilidadExitoDificil) * 100) << "%\n";
    std::cout << "==================================\n";
}
