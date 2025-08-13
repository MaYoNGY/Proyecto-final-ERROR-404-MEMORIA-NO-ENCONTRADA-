#include "juego.h"
#include "conio.h"
MenuPrincipal::MenuPrincipal() {
    tablaScore = new TablaScore();
    ColoresConsola::configurarConsola();
}

MenuPrincipal::~MenuPrincipal() {
    delete tablaScore;
}

void MenuPrincipal::mostrarMenuPrincipal() {
    ColoresConsola::limpiarPantalla();
    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                                                            ║\n";
    ColoresConsola::establecerColor(ROJO);
    std::cout << "║               ERROR 404: MEMORIA NO ENCONTRADA             ║\n";
    ColoresConsola::restablecerColor();
    std::cout << "║                                                            ║\n";
    std::cout << "║                Juego de Memoria y Estrategia               ║\n";
    std::cout << "║                                                            ║\n";
    std::cout << "╠════════════════════════════════════════════════════════════╣\n";
    std::cout << "║                        MENU PRINCIPAL                      ║\n";
    std::cout << "╠════════════════════════════════════════════════════════════╣\n";
    std::cout << "║                                                            ║\n";
    std::cout << "║  1. Iniciar Partida                                        ║\n";
    std::cout << "║                                                            ║\n";
    std::cout << "║  2. Ver Estadisticas y Teoremas                            ║\n";
    std::cout << "║                                                            ║\n";
    std::cout << "║  3. Tabla de Puntuaciones                                  ║\n";
    std::cout << "║                                                            ║\n";
    std::cout << "║  4. Salir                                                  ║\n";
    std::cout << "║                                                            ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n";
    std::cout << "\nIngrese su opcion (1-4): ";
}

void MenuPrincipal::mostrarMenuEstadisticas() {
    ColoresConsola::limpiarPantalla();
    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                 ESTADISTICAS Y TEOREMAS                    ║\n";
    std::cout << "╠════════════════════════════════════════════════════════════╣\n";
    std::cout << "║                                                            ║\n";
    std::cout << "║  1. Teorema de Bayes                                       ║\n";
    std::cout << "║                                                            ║\n";
    std::cout << "║  2. Grafos Dirigidos Ponderados                            ║\n";
    std::cout << "║                                                            ║\n";
    std::cout << "║  3. Equilibrio de Nash                                     ║\n";
    std::cout << "║                                                            ║\n";
    std::cout << "║  4. Top 10 Mejores Jugadores                               ║\n";
    std::cout << "║                                                            ║\n";
    std::cout << "║  5. Volver al Menu Principal                               ║\n";
    std::cout << "║                                                            ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n";
    std::cout << "\nIngrese su opcion (1-5): ";
}

void MenuPrincipal::mostrarTeoremasBayes() {
    ColoresConsola::limpiarPantalla();
    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    ColoresConsola::establecerColor(VERDE);
    std::cout << "║                    TEOREMA DE BAYES                        ║\n";
    ColoresConsola::restablecerColor();
    std::cout << "╠════════════════════════════════════════════════════════════╣\n";
    std::cout << "║                                                            ║\n";
    std::cout << "║ El teorema de Bayes es fundamental en este juego porque    ║\n";
    std::cout << "║ nos permite calcular probabilidades condicionales basadas  ║\n";
    std::cout << "║ en la evidencia previa (historial del jugador).            ║\n";
    std::cout << "║                                                            ║\n";
    std::cout << "║ Formula: P(A|B) = P(B|A) * P(A) / P(B)                     ║\n";
    std::cout << "║                                                            ║\n";
    std::cout << "║ En nuestro contexto:                                       ║\n";
    std::cout << "║ • P(Exito|Dificultad) = Probabilidad de exito dada         ║\n";
    std::cout << "║   la dificultad elegida                                    ║\n";
    std::cout << "║ • Se actualiza segun el historial de aciertos              ║\n";
    std::cout << "║ • Influye en las decisiones estrategicas del J1            ║\n";
    std::cout << "║                                                            ║\n";
    std::cout << "║ Aplicacion practica:                                       ║\n";
    std::cout << "║ • Modo FACIL: 80% probabilidad base de exito               ║\n";
    std::cout << "║ • Modo DIFICIL: 40% probabilidad base de exito             ║\n";
    std::cout << "║ • Se ajusta segun rendimiento historico                    ║\n";
    std::cout << "║                                                            ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n";
    std::cout << "\nPresiona ENTER para continuar...";
    std::cin.ignore();
    esperarSoloEnterGlobal();
}

void MenuPrincipal::mostrarGrafosDirigidos() {
    ColoresConsola::limpiarPantalla();
    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    ColoresConsola::establecerColor(AZUL);
    std::cout << "║               GRAFOS DIRIGIDOS PONDERADOS                  ║\n";
    ColoresConsola::restablecerColor();
    std::cout << "╠════════════════════════════════════════════════════════════╣\n";
    std::cout << "║                                                            ║\n";
    std::cout << "║ Los grafos dirigidos modelan el flujo de estados del       ║\n";
    std::cout << "║ juego y las transiciones posibles entre ellos.             ║\n";
    std::cout << "║                                                            ║\n";
    std::cout << "║ Componentes:                                               ║\n";
    std::cout << "║ • NODOS: Estados del juego (secuencias, puntuaciones)      ║\n";
    std::cout << "║ • ARISTAS: Transiciones con utilidades/probabilidades      ║\n";
    std::cout << "║ • PESOS: Representan la utilidad o probabilidad            ║\n";
    std::cout << "║                                                            ║\n";
    std::cout << "║ Estados principales:                                       ║\n";
    std::cout << "║ 1. INICIO -> Configuración inicial                         ║\n";
    std::cout << "║ 2. DECISION_DIFICULTAD -> J1 elige estrategia              ║\n";
    std::cout << "║ 3. REPETICION_SECUENCIA -> J2 ejecuta                      ║\n";
    std::cout << "║ 4. VERIFICACION -> Evaluacion de resultados                ║\n";
    std::cout << "║ 5. FIN -> Actualización de puntuaciones                    ║\n";
    std::cout << "║                                                            ║\n";
    std::cout << "║ Utilidad de las aristas:                                   ║\n";
    std::cout << "║ • Probabilidad de exito segun dificultad                   ║\n";
    std::cout << "║ • Permite encontrar caminos optimos                        ║\n";
    std::cout << "║                                                            ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n";
    std::cout << "\nPresiona ENTER para continuar...";
    std::cin.ignore();
    esperarSoloEnterGlobal();
}

void MenuPrincipal::mostrarEquilibrioNash() {
    ColoresConsola::limpiarPantalla();
    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    ColoresConsola::establecerColor(AMARILLO);
    std::cout << "║                   EQUILIBRIO DE NASH                       ║\n";
    ColoresConsola::restablecerColor();
    std::cout << "╠════════════════════════════════════════════════════════════╣\n";
    std::cout << "║                                                            ║\n";
    std::cout << "║ El Equilibrio de Nash es un concepto de la teoria de       ║\n";
    std::cout << "║ juegos donde ningun jugador tiene incentivo para cambiar   ║\n";
    std::cout << "║ su estrategia unilateralmente.                             ║\n";
    std::cout << "║                                                            ║\n";
    std::cout << "║ Matriz de Ganancias (J1, J2):                              ║\n";
    std::cout << "║                    J2 FALLA    J2 ACIERTA                  ║\n";
    std::cout << "║ J1 FACIL      │    (2,0)         (1,1)                     ║\n";
    std::cout << "║ J1 DIFICIL    │    (3,0)         (0,2)                     ║\n";
    std::cout << "║                                                            ║\n";
    std::cout << "║ Interpretacion:                                            ║\n";
    std::cout << "║ • Si J1 elige FACIL y J2 falla -> J1 gana 2, J2 gana 0     ║\n";
    std::cout << "║ • Si J1 elige FACIL y J2 acierta -> ambos ganan 1          ║\n";
    std::cout << "║ • Si J1 elige DIFICIL y J2 falla -> J1 gana 3, J2 gana 0   ║\n";
    std::cout << "║ • Si J1 elige DIFICIL y J2 acierta -> J1 gana 0, J2 gana 2 ║\n";
    std::cout << "║                                                            ║\n";
    std::cout << "║ Estrategia optima:                                         ║\n";
    std::cout << "║ J1 debe considerar la habilidad historica de J2            ║\n";
    std::cout << "║ para elegir entre riesgo alto/recompensa alta o            ║\n";
    std::cout << "║ estrategia conservadora con ganancia garantizada.          ║\n";
    std::cout << "║                                                            ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n";
    std::cout << "\nPresiona ENTER para continuar...";
    std::cin.ignore();
    esperarSoloEnterGlobal();
}

int MenuPrincipal::procesarOpcionMenu() {
    char opcion;
    while (true) {
        opcion = _getch();
        if (opcion >= '1' && opcion <= '4') {
            std::cout << opcion << std::endl; // Muestra la opción elegida
            return opcion - '0'; // Convierte el carácter a número
        }
        // Si no es numérico entre 1 y 5, no hace nada (bloquea la opcion)

    }
}

int MenuPrincipal::procesarOpcionMenuEstadisticas() {
    char opcion;
    while (true) {
        opcion = _getch();
        if (opcion >= '1' && opcion <= '5') {
            std::cout << opcion << std::endl; 
            return opcion - '0'; // Convierte el carácter a número
        }
        // Si no es numérico entre 1 y 5, simplemente ignora la tecla
    } 
}

void MenuPrincipal::ejecutarJuego() {
    Juego404 juego;
    juego.configurarJugadores();
    juego.mostrarPantallaInicio();
    juego.inicializarJuego();
    
    // Bucle principal del juego
    while (juego.continuarJuego()) {
        juego.actualizarEstado();
    }
    
    // Guardar partida al finalizar
    juego.guardarPartida();
}

void MenuPrincipal::manejarEstadisticas() {
    int opcion;
    do {
        mostrarMenuEstadisticas();
        opcion = procesarOpcionMenu();
        
        switch (opcion) {
            case 1:
                mostrarTeoremasBayes();
                break;
            case 2:
                mostrarGrafosDirigidos();
                break;
            case 3:
                mostrarEquilibrioNash();
                break;
            case 4:
                tablaScore->mostrarTop10();
                std::cout << "\nPresiona ENTER para continuar...";
                std::cin.ignore();
                esperarSoloEnterGlobal();
                break;
            case 5:
                return;
            default:
                std::cout << "Opcion invalida. Intenta de nuevo.\n";
                ColoresConsola::pausar();
                break;
        }
    } while (opcion != 5);
}

void MenuPrincipal::salir() {
    ColoresConsola::limpiarPantalla();
    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                                                            ║\n";
    std::cout << "║                    Gracias por jugar!                      ║\n";
    std::cout << "║                                                            ║\n";
    std::cout << "║             ERROR 404: MEMORIA NO ENCONTRADA               ║\n";
    std::cout << "║                                                            ║\n";
    std::cout << "║    Un juego que combina memoria, estrategia y teoria       ║\n";
    std::cout << "║              de juegos para desafiar tu mente              ║\n";
    std::cout << "║                                                            ║\n";
    std::cout << "║                   Desarrollado con Amor                    ║\n";
    std::cout << "║                                                            ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n";
    std::cout << "\n";
    ColoresConsola::pausar();
}

void MenuPrincipal::run() {
    int opcion;
    
    do {
        mostrarMenuPrincipal();
        opcion = procesarOpcionMenu();
        
        switch (opcion) {
            case 1:
                ejecutarJuego();
                break;
            case 2:
                manejarEstadisticas();
                break;
            case 3:
                tablaScore->mostrarTablaScore();
                std::cout << "\nPresiona ENTER para continuar...";
                std::cin.ignore();
                esperarSoloEnterGlobal();
                break;
            case 4:
                salir();
                break;
            default:
                std::cout << "\nOpcion invalida. Por favor, elige una opcion entre 1-4.\n";
                ColoresConsola::pausar();
                break;
        }
    } while (opcion != 4);
}
