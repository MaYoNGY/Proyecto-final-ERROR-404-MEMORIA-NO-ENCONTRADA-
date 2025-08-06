#include "juego.h"

Secuencia::Secuencia() : dificultadActual(FACIL) {
    colores.clear();
}

void Secuencia::agregarColor(Color color) {
    colores.push_back(color);
}

void Secuencia::establecerDificultad(TipoDificultad dif) {
    dificultadActual = dif;
}

std::vector<Color> Secuencia::getSecuencia() const {
    return colores;
}

TipoDificultad Secuencia::getDificultad() const {
    return dificultadActual;
}

Color Secuencia::generarSiguienteColor(TipoDificultad dif) const {
    std::random_device rd;
    std::mt19937 gen(rd());
    
    if (dif == FACIL) {
        // Patrón fácil: repetir el último color o seguir un patrón simple
        if (!colores.empty()) {
            return colores.back(); // Repetir último color
        } else {
            std::uniform_int_distribution<> dis(0, 1);
            return static_cast<Color>(dis(gen)); // Solo rojo o verde
        }
    } else {
        // Patrón difícil: color completamente aleatorio
        std::uniform_int_distribution<> dis(0, 3);
        return static_cast<Color>(dis(gen)); // Cualquier color
    }
}

std::string Secuencia::colorToString(Color color) const {
    switch (color) {
        case ROJO: return "ROJO";
        case VERDE: return "VERDE";
        case AZUL: return "AZUL";
        case AMARILLO: return "AMARILLO";
        default: return "DESCONOCIDO";
    }
}

void Secuencia::mostrarSecuencia() const {
    std::cout << "Secuencia actual: ";
    for (size_t i = 0; i < colores.size(); i++) {
        std::cout << colorToString(colores[i]);
        if (i < colores.size() - 1) std::cout << " -> ";
    }
    std::cout << "\nDificultad: " << (dificultadActual == FACIL ? "FÁCIL" : "DIFÍCIL") << "\n";
}

bool Secuencia::validarSecuencia(const std::vector<Color>& intento) const {
    if (intento.size() != colores.size()) {
        return false;
    }
    
    for (size_t i = 0; i < colores.size(); i++) {
        if (intento[i] != colores[i]) {
            return false;
        }
    }
    
    return true;
}
