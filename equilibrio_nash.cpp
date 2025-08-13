#include "juego.h"

EquilibrioNash::EquilibrioNash(MatrizGanancias* matriz) : matrizGanancias(matriz) {}

std::pair<TipoDificultad, bool> EquilibrioNash::encontrarEquilibrio() const {
    std::cout << "\n=== ANALISIS EQUILIBRIO DE NASH ===\n";
    
    // Analizar todas las combinaciones de estrategias
    std::vector<std::pair<TipoDificultad, bool>> estrategias = {
        {FACIL, false},   // J1: Facil, J2: Falla
        {FACIL, true},    // J1: Facil, J2: Acierta
        {DIFICIL, false}, // J1: Dificil, J2: Falla
        {DIFICIL, true}   // J1: Dificil, J2: Acierta
    };
    
    std::vector<std::pair<TipoDificultad, bool>> equilibrios;
    
    for (const auto& estrategia : estrategias) {
        if (esEquilibrio(estrategia.first, estrategia.second)) {
            equilibrios.push_back(estrategia);
            std::cout << "Equilibrio encontrado: J1=" 
                      << (estrategia.first == FACIL ? "FACIL" : "DIFICIL")
                      << ", J2=" << (estrategia.second ? "EXITO" : "FALLO") << "\n";
        }
    }
    
    if (equilibrios.empty()) {
        std::cout << "No se encontro equilibrio de Nash puro.\n";
        std::cout << "Sugiriendo estrategia mixta...\n";
        return {DIFICIL, true}; // Estrategia por defecto
    }
    
    return equilibrios[0]; // Retornar el primer equilibrio encontrado
}

bool EquilibrioNash::esEquilibrio(TipoDificultad estrategiaJ1, bool estrategiaJ2) const {
    // Verificar si ningún jugador tiene incentivo para cambiar su estrategia
    
    auto gananciaActual = matrizGanancias->obtenerGanancias(estrategiaJ1, estrategiaJ2);
    
    // Verificar si J1 quiere cambiar su estrategia
    TipoDificultad otraEstrategiaJ1 = (estrategiaJ1 == FACIL) ? DIFICIL : FACIL;
    auto gananciaAlternativaJ1 = matrizGanancias->obtenerGanancias(otraEstrategiaJ1, estrategiaJ2);
    
    if (gananciaAlternativaJ1.first > gananciaActual.first) {
        return false; // J1 prefiere cambiar
    }
    
    // Verificar si J2 quiere cambiar su estrategia
    bool otraEstrategiaJ2 = !estrategiaJ2;
    auto gananciaAlternativaJ2 = matrizGanancias->obtenerGanancias(estrategiaJ1, otraEstrategiaJ2);
    
    if (gananciaAlternativaJ2.second > gananciaActual.second) {
        return false; // J2 prefiere cambiar
    }
    
    return true; // Es un equilibrio de Nash
}

void EquilibrioNash::analizarEstrategias() const {
    std::cout << "\n=== ANALISIS DETALLADO DE ESTRATEGIAS ===\n";
    
    // Mostrar la matriz de ganancias
    matrizGanancias->mostrarMatriz();
    
    std::cout << "\nAnalisis por jugador:\n";
    std::cout << "\nJUGADOR 1 (Elige dificultad):\n";
    std::cout << "- Si elige FACIL: Ganancia garantizada pero limitada\n";
    std::cout << "- Si elige DIFICIL: Mayor riesgo pero mayor recompensa potencial\n";
    
    std::cout << "\nJUGADOR 2 (Ejecuta secuencia):\n";
    std::cout << "- Siempre prefiere que J1 elija FACIL\n";
    std::cout << "- Su éxito depende de su habilidad y la dificultad elegida\n";
    
    std::cout << "\nRecomendaciones estrategicas:\n";
    std::cout << "- J1 debe considerar la habilidad historica de J2\n";
    std::cout << "- J2 debe mejorar su memoria y concentracion\n";
    std::cout << "- El equilibrio favorece estrategias mixtas\n";
    
    // Encontrar y mostrar equilibrios
    encontrarEquilibrio();
}
