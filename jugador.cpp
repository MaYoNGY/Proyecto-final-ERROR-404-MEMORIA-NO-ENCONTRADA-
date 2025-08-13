#include "juego.h"

Jugador::Jugador(int idJugador) : id(idJugador), puntaje(0) {
    historialAciertos.clear();
    historialDificultades.clear();
}

void Jugador::agregarAcierto(bool acierto) {
    historialAciertos.push_back(acierto);
}

void Jugador::agregarDificultad(TipoDificultad dif) {
    historialDificultades.push_back(dif);
}

void Jugador::sumarPuntos(int puntos) {
    puntaje += puntos;
}

int Jugador::getPuntaje() const {
    return puntaje;
}

int Jugador::getId() const {
    return id;
}

double Jugador::getTasaExito() const {
    if (historialAciertos.empty()) {
        return 0.5; // Probabilidad neutral sin historial
    }
    
    int aciertos = 0;
    for (bool acierto : historialAciertos) {
        if (acierto) aciertos++;
    }
    
    return static_cast<double>(aciertos) / historialAciertos.size();
}

std::vector<bool> Jugador::getHistorialAciertos() const {
    return historialAciertos;
}

void Jugador::mostrarEstadisticas() const {
    std::cout << "\n=== Estadisticas Jugador " << id << " ===\n";
    std::cout << "Puntaje actual: " << puntaje << "\n";
    std::cout << "Partidas jugadas: " << historialAciertos.size() << "\n";
    
    if (!historialAciertos.empty()) {
        std::cout << "Tasa de exito: " << (getTasaExito() * 100) << "%\n";
        
        int aciertosFacil = 0, totalFacil = 0;
        int aciertosDificil = 0, totalDificil = 0;
        
        for (size_t i = 0; i < historialAciertos.size() && i < historialDificultades.size(); i++) {
            if (historialDificultades[i] == FACIL) {
                totalFacil++;
                if (historialAciertos[i]) aciertosFacil++;
            } else {
                totalDificil++;
                if (historialAciertos[i]) aciertosDificil++;
            }
        }
        
        if (totalFacil > 0) {
            std::cout << "Exito en modo facil: " << (static_cast<double>(aciertosFacil) / totalFacil * 100) << "%\n";
        }
        if (totalDificil > 0) {
            std::cout << "Exito en modo dificil: " << (static_cast<double>(aciertosDificil) / totalDificil * 100) << "%\n";
        }
    }
    std::cout << "========================\n";
}
