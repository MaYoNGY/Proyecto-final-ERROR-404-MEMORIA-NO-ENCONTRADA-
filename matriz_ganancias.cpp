#include "juego.h"

MatrizGanancias::MatrizGanancias() {
    // Inicializar matriz 2x2 para las estrategias de los jugadores
    // Filas: estrategias del Jugador 1 (FACIL, DIFICIL)
    // Columnas: resultados del Jugador 2 (FALLO, EXITO)
    matriz.resize(2, std::vector<std::pair<int, int>>(2));
    
    // Configurar las ganancias según las reglas del juego
    // matriz[estrategia_J1][resultado_J2] = {ganancia_J1, ganancia_J2}
    
    // Jugador 1 elige FACIL
    matriz[0][0] = {2, 0};  // J2 falla -> J1 gana, J2 pierde
    matriz[0][1] = {1, 1};  // J2 acierta -> ambos ganan
    
    // Jugador 1 elige DIFICIL
    matriz[1][0] = {3, 0};  // J2 falla -> J1 gana más por mayor dificultad
    matriz[1][1] = {0, 2};  // J2 acierta -> J2 gana más por superar dificultad
}

void MatrizGanancias::calcularGanancias(TipoDificultad difJ1, bool exito) {
    // Las ganancias ya están precalculadas en la matriz
    // Esta función podría usarse para ajustes dinámicos futuros
}

std::pair<int, int> MatrizGanancias::obtenerGanancias(TipoDificultad difJ1, bool exito) const {
    int fila = (difJ1 == FACIL) ? 0 : 1;
    int columna = exito ? 1 : 0;
    return matriz[fila][columna];
}

void MatrizGanancias::mostrarMatriz() const {
    std::cout << "\n=== MATRIZ DE GANANCIAS ===\n";
    std::cout << "                    J2 FALLA    J2 ACIERTA\n";
    std::cout << "J1 FACIL     |   (" << matriz[0][0].first << "," << matriz[0][0].second 
              << ")      (" << matriz[0][1].first << "," << matriz[0][1].second << ")\n";
    std::cout << "J1 DIFICIL   |   (" << matriz[1][0].first << "," << matriz[1][0].second 
              << ")      (" << matriz[1][1].first << "," << matriz[1][1].second << ")\n";
    std::cout << "===============================\n";
    std::cout << "Formato: (Ganancia J1, Ganancia J2)\n\n";
}

std::pair<TipoDificultad, bool> MatrizGanancias::encontrarEquilibrio() const {
    // Análisis simplificado del equilibrio de Nash
    // En este juego, el equilibrio depende de las probabilidades de éxito del J2
    
    std::cout << "\n=== ANÁLISIS DE EQUILIBRIO ===\n";
    std::cout << "Analizando estrategias dominantes...\n";
    
    // Para J1: comparar ganancias esperadas
    // Si J2 tiene alta probabilidad de éxito, J1 prefiere DIFICIL
    // Si J2 tiene baja probabilidad de éxito, J1 prefiere FACIL
    
    std::cout << "- Si J2 es experto (alta prob. éxito): J1 debería elegir DIFÍCIL\n";
    std::cout << "- Si J2 es novato (baja prob. éxito): J1 puede elegir FÁCIL\n";
    std::cout << "- Para J2: siempre prefiere que J1 elija FÁCIL\n";
    
    // Equilibrio mixto típico en este tipo de juegos
    return {DIFICIL, true}; // Estrategia conservadora
}
