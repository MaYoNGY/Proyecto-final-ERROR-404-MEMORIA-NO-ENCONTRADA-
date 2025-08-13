#include "juego.h"
#include <conio.h>

// Función global para esperar solo ENTER
void esperarSoloEnterGlobal() {
    char tecla;
    while (true) {
        tecla = _getch(); // Leer tecla directamente
        
        // Solo aceptar ENTER (código ASCII 13 o '\r')
        if (tecla == 13 || tecla == '\r' || tecla == '\n') {
            std::cout << std::endl; // Mostrar salto de línea
            return;
        }
        // Si no es ENTER, simplemente ignorar la tecla sin hacer nada
    }
}
Juego404::Juego404() {
    estadoActual = INICIO;
    rondaActual = 0;
    nodoActual = 0;
    srand(time(0));

    // Inicializar componentes del juego
    grafoJuego = new GrafoJuego();
    secuenciaActual = new Secuencia();
    matrizGanancias = new MatrizGanancias();
    calculadorBayes = new CalculadorBayes();
    equilibrioNash = new EquilibrioNash(matrizGanancias);
    tablaScore = new TablaScore();

    // Crear jugadores
    jugadores.push_back(Jugador(1));
    jugadores.push_back(Jugador(2));

    // Configurar transiciones NFA
    transicionesNFA["memoria"] = {"REPETICION_SECUENCIA", "VERIFICACION"};
    transicionesNFA["adivinar"] = {"DECISION_DIFICULTAD", "VERIFICACION"};
    transicionesNFA["iniciar"] = {"DECISION_DIFICULTAD"};
    transicionesNFA["finalizar"] = {"FIN"};
}

Juego404::~Juego404() {
    delete grafoJuego;
    delete secuenciaActual;
    delete matrizGanancias;
    delete calculadorBayes;
    delete equilibrioNash;
    delete tablaScore;
}

void Juego404::configurarJugadores() {
    ColoresConsola::limpiarPantalla();
    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                 CONFIGURACION DE JUGADORES                 ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n";
    
    std::cout << "\n Ingresa el nombre del Jugador 1 (Estratega): ";
    std::getline(std::cin, nombreJ1);
    if (nombreJ1.empty()) nombreJ1 = "Jugador1";

    std::cout << "\n Ingresa el nombre del Jugador 2 (Ejecutor): ";
    std::getline(std::cin, nombreJ2);
    if (nombreJ2.empty()) nombreJ2 = "Jugador2";

    std::cout << "\n Configuracion completada:\n";
    ColoresConsola::establecerColor(VERDE);
    std::cout << nombreJ1 << " - Elegira la dificultad de las secuencias\n";
    ColoresConsola::establecerColor(AZUL);
    std::cout << nombreJ2 << " - Memorizara y repetira las secuencias\n";
    ColoresConsola::restablecerColor();
    
    std::cout << "\nPresiona ENTER para comenzar...";
    esperarSoloEnter();
}

void Juego404::mostrarPantallaInicio() {
    ColoresConsola::limpiarPantalla();
    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    ColoresConsola::establecerColor(ROJO);
    std::cout << "║              ERROR 404: Memoria no encontrada              ║\n";
    ColoresConsola::restablecerColor();
    std::cout << "╠════════════════════════════════════════════════════════════╣\n";
    std::cout << "║                                                            ║\n";
    std::cout << "║   Objetivo: Memorizar secuencias de colores                ║\n";
    std::cout << "║   Jugadores: 2 (Estratega y Ejecutor)                      ║\n";
    std::cout << "║   Rondas: Maximo 5 por partida                             ║\n";
    std::cout << "║                                                            ║\n";
    std::cout << "║     Reglas:                                                ║\n";
    std::cout << "║  • " << nombreJ1 << " elige la dificultad (FACIL/DIFICIL)  ║\n";
    std::cout << "║  • " << nombreJ2 << " debe memorizar y repetir la secuencia║\n";
    std::cout << "║  • Puntos segun matriz de ganancias Nash                   ║\n";
    std::cout << "║                                                            ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n";
    
    ColoresConsola::pausar();
}

void Juego404::inicializarJuego() {
    // Crear nodo inicial del grafo
    Nodo nodoInicial;
    nodoInicial.secuencia = {ROJO}; // Comenzar con color rojo
    nodoInicial.dificultad = FACIL;
    nodoInicial.puntajeJugador1 = 0;
    nodoInicial.puntajeJugador2 = 0;
    
    nodoActual = grafoJuego->agregarNodo(nodoInicial);
    
    // Establecer secuencia inicial
    secuenciaActual->agregarColor(ROJO);
    
    estadoActual = DECISION_DIFICULTAD;
    rondaActual = 1;
    
    ColoresConsola::limpiarPantalla();
    std::cout << "  Juego inicializado - Ronda " << rondaActual << " de 5\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n";
    secuenciaActual->mostrarSecuencia();
    
    ColoresConsola::pausar();
}

bool Juego404::continuarJuego() {
    return estadoActual != FIN || rondaActual < 5;
}

void Juego404::actualizarEstado() {
    switch (estadoActual) {
        case INICIO:
            ColoresConsola::limpiarPantalla();
            std::cout << " INICIANDO JUEGO...\n";
            estadoActual = DECISION_DIFICULTAD;
            break;
            
        case DECISION_DIFICULTAD:
            ColoresConsola::limpiarPantalla();
            std::cout << "╔═══════════════════════════════════════════════════════════╗\n";
            ColoresConsola::establecerColor(VERDE);
            std::cout << "║                  FASE: DECISION ESTRATEGICA               ║\n";
            ColoresConsola::restablecerColor();
            std::cout << "╚═══════════════════════════════════════════════════════════╝\n";
            procesarTurnoJugador1();
            estadoActual = REPETICION_SECUENCIA;
            
            std::cout << "\n Cambiando turno...";
            std::cout << "\nPresiona ENTER para continuar al turno de " << nombreJ2 << "...";
            std::cin.ignore();
            esperarSoloEnter();
            break;
            
        case REPETICION_SECUENCIA:
            ColoresConsola::limpiarPantalla();
            std::cout << "╔═══════════════════════════════════════════════════════════╗\n";
            ColoresConsola::establecerColor(AZUL);
            std::cout << "║                    FASE: PRUEBA DE MEMORIA                ║\n";
            ColoresConsola::restablecerColor();
            std::cout << "╚═══════════════════════════════════════════════════════════╝\n";
            procesarTurnoJugador2();
            estadoActual = VERIFICACION;
            
            std::cout << "\n Verificando resultado...";
            std::cout << "\nPresiona ENTER para ver los resultados...";
            std::cin.ignore();
            esperarSoloEnter();
            break;
            
        case VERIFICACION:
            ColoresConsola::limpiarPantalla();
            std::cout << "╔═══════════════════════════════════════════════════════════╗\n";
            ColoresConsola::establecerColor(AMARILLO);
            std::cout << "║                  FASE: VERIFICACION                       ║\n";
            ColoresConsola::restablecerColor();
            std::cout << "╚═══════════════════════════════════════════════════════════╝\n";
            verificarResultado();
            
            if (rondaActual >= 5) {
                estadoActual = FIN;
            } else {
                rondaActual++;
                // NO reiniciar secuencia - mantener la secuencia acumulativa
                estadoActual = DECISION_DIFICULTAD;
            }
            break;
            
        case FIN:
            ColoresConsola::limpiarPantalla();
            std::cout << "╔═══════════════════════════════════════════════════════════╗\n";
            ColoresConsola::establecerColor(ROJO);
            std::cout << "║                    JUEGO TERMINADO                        ║\n";
            ColoresConsola::restablecerColor();
            std::cout << "╚═══════════════════════════════════════════════════════════╝\n";
            
            // Mostrar estadísticas detalladas de cada ronda primero
            mostrarEstadisticasDetalladas();
            
            // Luego mostrar estadísticas generales
            mostrarEstadisticasJuego();
            
            char verAnalisis;
            std::cout << "\nDeseas ver el analisis estrategico avanzado? (s/n): ";
            verAnalisis = leerSiNo();
            
            if (verAnalisis == 's' || verAnalisis == 'S') {
                analizarEstrategias();
            }
            break;
            
        default:
            break;
    }
}

int leerOpcionTeclado(int min, int max) {
    char eleccion;
    while (true) {
        eleccion = _getch();
        if (eleccion >= (min + '0') && eleccion <= (max + '0')) {
            std::cout << eleccion << std::endl;
            return eleccion - '0';
        }
        // Si la tecla no es válida, simplemente la ignoramos sin mostrar nada
    }
}

int Juego404::leerColorValido(int min, int max) {
    char tecla;
    while (true) {
        tecla = _getch(); // Leer tecla directamente sin mostrarla
        
        // Verificar si la tecla está en el rango válido
        if (tecla >= (min + '0') && tecla <= (max + '0')) {
            std::cout << tecla; // Mostrar solo si es válida
            std::cout.flush(); // Asegurar que se muestre inmediatamente
            return tecla - '0';
        }
        // Si la tecla no es válida, simplemente la ignoramos sin mostrar nada
    }
}

void Juego404::esperarSoloEnter() {
    char tecla;
    while (true) {
        tecla = _getch(); // Leer tecla directamente
        
        // Solo aceptar ENTER (código ASCII 13 o '\r')
        if (tecla == 13 || tecla == '\r' || tecla == '\n') {
            std::cout << std::endl; // Mostrar salto de línea
            return;
        }
        // Si no es ENTER, simplemente ignorar la tecla sin hacer nada
    }
}

char Juego404::leerSiNo() {
    char tecla;
    while (true) {
        tecla = _getch(); // Leer tecla directamente
        
        // Solo aceptar S, s, N, n
        if (tecla == 'S' || tecla == 's' || tecla == 'N' || tecla == 'n') {
            std::cout << tecla << std::endl; // Mostrar la tecla válida
            return tecla;
        }
        // Si no es S/N, simplemente ignorar la tecla sin hacer nada
    }
}

void Juego404::procesarTurnoJugador1() {
    std::cout << "\n Turno de " << nombreJ1 << " (Estratega)\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n";
    
    std::cout << " Secuencia actual:\n";
    secuenciaActual->mostrarSecuencia();
    
    // Permitir al jugador elegir
    std::cout << " ELIGE TU ESTRATEGIA:\n";
    std::cout << "1. Modo FACIL (Mayor probabilidad de exito para " << nombreJ2 << ")\n";
    std::cout << "2. Modo DIFICIL (Mayor riesgo, mayor recompensa)\n";
    std::cout << "\nTu eleccion (1-2): ";
    char eleccion = leerOpcionTeclado(1, 2);
    
    TipoDificultad dificultadElegida = (eleccion == 1) ? FACIL : DIFICIL;
    secuenciaActual->establecerDificultad(dificultadElegida);
    
    std::cout << "\n " << nombreJ1 << " eligio: ";
    if (dificultadElegida == FACIL) {
        ColoresConsola::establecerColor(VERDE);
        std::cout << "MODO FACIL";
    } else {
        ColoresConsola::establecerColor(ROJO);
        std::cout << "MODO DIFICIL";
    }
    ColoresConsola::restablecerColor();
    std::cout << "\n\n";
    
    // Agregar nuevo color según la dificultad
    Color nuevoColor = secuenciaActual->generarSiguienteColor(dificultadElegida);
    secuenciaActual->agregarColor(nuevoColor);
    
    std::cout << " Nuevo color agregado: ";
    ColoresConsola::mostrarColorConTexto(nuevoColor, secuenciaActual->colorToString(nuevoColor));
    ColoresConsola::restablecerColor();
    std::cout << "\n\n Secuencia completa actualizada:\n";
    secuenciaActual->mostrarSecuencia();
    
    // Crear nuevo nodo en el grafo
    Nodo nuevoNodo;
    nuevoNodo.secuencia = secuenciaActual->getSecuencia();
    nuevoNodo.dificultad = dificultadElegida;
    nuevoNodo.puntajeJugador1 = jugadores[0].getPuntaje();
    nuevoNodo.puntajeJugador2 = jugadores[1].getPuntaje();
    
    int nuevoNodoId = grafoJuego->agregarNodo(nuevoNodo);
    
    // Agregar arista con utilidad calculada
    double utilidad = (dificultadElegida == FACIL) ? 0.8 : 0.4;
    grafoJuego->agregarArista(nodoActual, nuevoNodoId, utilidad, "elegir_dificultad");
    
    nodoActual = nuevoNodoId;

}

void Juego404::procesarTurnoJugador2() {
    std::cout << "\n Turno de " << nombreJ2 << " (Ejecutor de Memoria)\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n";
    
    std::cout << " Es tu turno de repetir la secuencia completa!\n\n";
    
    // Mostrar información de la dificultad
    std::cout << " Dificultad elegida por " << nombreJ1 << ": ";
    if (secuenciaActual->getDificultad() == FACIL) {
        ColoresConsola::establecerColor(VERDE);
        std::cout << "MODO FACIL";
    } else {
        ColoresConsola::establecerColor(ROJO);
        std::cout << "MODO DIFICIL";
    }
    ColoresConsola::restablecerColor();
    std::cout << "\n\n";
    
    // Mostrar la secuencia por un tiempo limitado
    std::cout << "  ATENCION! La secuencia se mostrara por un tiempo limitado.\n";
    std::cout << " Concentrate y memoriza cada color y su orden.\n\n";
    std::cout << "Presiona ENTER para ver la secuencia: ";
    std::cin.ignore();
    esperarSoloEnter();
    
    // Mostrar secuencia con colores
    ColoresConsola::limpiarPantalla();
    std::cout << "╔═══════════════════════════════════════════════════════════╗\n";
    std::cout << "║                    SECUENCIA A MEMORIZAR                  ║\n";
    std::cout << "╚═══════════════════════════════════════════════════════════╝\n\n";
    
    secuenciaActual->mostrarSecuencia();
    
    // Tiempo diferente según dificultad
    int tiempoMemorizacion = (secuenciaActual->getDificultad() == FACIL) ? 4 : 3;
    std::cout << "\nTiempo de memorizacion: " << tiempoMemorizacion << " segundos\n";
    std::cout << "Memorizando";
    
    for (int i = 0; i < tiempoMemorizacion; i++) {
        std::cout << " ⏳";
        std::cout.flush();
        #ifdef _WIN32
            Sleep(1000);
        #else
            sleep(1);
        #endif
    }
    
    // Limpiar pantalla y preparar entrada
    ColoresConsola::limpiarPantalla();
    std::cout << "╔═══════════════════════════════════════════════════════════╗\n";
    ColoresConsola::establecerColor(ROJO);
    std::cout << "║                    TIEMPO TERMINADO                       ║\n";
    ColoresConsola::restablecerColor();
    std::cout << "╚═══════════════════════════════════════════════════════════╝\n\n";
    
    std::cout << " Ahora ingresa la secuencia que memorizaste:\n\n";
    
    // Obtener la secuencia del jugador
    std::vector<Color> secuenciaOriginal = secuenciaActual->getSecuencia();
    std::vector<Color> secuenciaJugador;
    
    std::cout << " Colores disponibles:\n";
    std::cout << "1. "; ColoresConsola::mostrarColorConTexto(ROJO, "ROJO"); std::cout << "\n";
    std::cout << "2. "; ColoresConsola::mostrarColorConTexto(VERDE, "VERDE"); std::cout << "\n";
    std::cout << "3. "; ColoresConsola::mostrarColorConTexto(AZUL, "AZUL"); std::cout << "\n";
    std::cout << "4. "; ColoresConsola::mostrarColorConTexto(AMARILLO, "AMARILLO"); std::cout << "\n\n";
    ColoresConsola::restablecerColor();

    std::cout << " Ingresa " << secuenciaOriginal.size() << " colores (usa los numeros 1-4):\n";

    for (size_t i = 0; i < secuenciaOriginal.size(); i++) {
        int eleccion;
        std::cout << "Posicion " << (i + 1) << "/" << secuenciaOriginal.size() << ": ";
        
        // Usar la función que bloquea teclas inválidas
        eleccion = leerColorValido(1, 4);
        std::cout << std::endl; // Salto de línea después de mostrar el número
        
        secuenciaJugador.push_back(static_cast<Color>(eleccion - 1));
        
        // Mostrar progreso
        std::cout << "   Agregado: ";
        ColoresConsola::mostrarColorConTexto(static_cast<Color>(eleccion - 1), secuenciaActual->colorToString(static_cast<Color>(eleccion - 1)));
        ColoresConsola::restablecerColor();
        std::cout << "\n";
    }
    
    // Verificar si la secuencia es correcta
    bool exito = secuenciaActual->validarSecuencia(secuenciaJugador);
    
    std::cout << "\n╔═══════════════════════════════════════════════════════════╗\n";
    std::cout << "║                        RESULTADO                          ║\n";
    std::cout << "╚═══════════════════════════════════════════════════════════╝\n\n";
    
    std::cout << " Secuencia original:  ";
    for (size_t i = 0; i < secuenciaOriginal.size(); i++) {
        ColoresConsola::mostrarColorConTexto(secuenciaOriginal[i], 
                                           secuenciaActual->colorToString(secuenciaOriginal[i]));
        if (i < secuenciaOriginal.size() - 1) {
            ColoresConsola::restablecerColor();
            std::cout << " -> ";
        }
    }
    ColoresConsola::restablecerColor();
    std::cout << "\n\n";
    
    std::cout << " Tu secuencia:        ";
    for (size_t i = 0; i < secuenciaJugador.size(); i++) {
        // Colorear correcto/incorrecto
        if (i < secuenciaOriginal.size() && secuenciaJugador[i] == secuenciaOriginal[i]) {
            ColoresConsola::mostrarColorConTexto(secuenciaJugador[i], 
                                               secuenciaActual->colorToString(secuenciaJugador[i]));
        } else {
            ColoresConsola::establecerColor(ROJO);
            std::cout << secuenciaActual->colorToString(secuenciaJugador[i]);
            ColoresConsola::restablecerColor();
        }
        
        if (i < secuenciaJugador.size() - 1) {
            ColoresConsola::restablecerColor();
            std::cout << " → ";
        }
    }
    ColoresConsola::restablecerColor();
    std::cout << "\n\n";
    
    if (exito) {
        ColoresConsola::establecerColor(VERDE);
        std::cout << " PERFECTO! " << nombreJ2 << " repitio la secuencia correctamente!\n";
        std::cout << " Memoria excepcional demostrada.\n";
    } else {
        ColoresConsola::establecerColor(ROJO);
        std::cout << " INCORRECTO. " << nombreJ2 << " no logro repetir la secuencia.\n";
        std::cout << " Mejor suerte en la proxima ronda.\n";
    }
    ColoresConsola::restablecerColor();
    
    // Registrar resultado para Bayes
    jugadores[1].agregarAcierto(exito);
    jugadores[1].agregarDificultad(secuenciaActual->getDificultad());
    
    std::cout << "\n Actualizando probabilidades bayesianas...\n";
    calculadorBayes->actualizarProbabilidades(jugadores);
}
    

void Juego404::verificarResultado() {
    std::vector<Color> secuenciaOriginal = secuenciaActual->getSecuencia();
    bool exito = jugadores[1].getHistorialAciertos().back(); // Último resultado
    
    // Calcular puntos según matriz de ganancias
    auto ganancias = matrizGanancias->obtenerGanancias(secuenciaActual->getDificultad(), exito);
    
    // Mostrar probabilidades
        // Mostrar análisis estratégico usando Bayes
    double probabilidadExitoJ2 = calculadorBayes->calcularProbabilidadCondicional(jugadores[1], FACIL);
    double probabilidadExitoJ2Dificil = calculadorBayes->calcularProbabilidadCondicional(jugadores[1], DIFICIL);
    std::cout << "\nProbabilidad esperada para ronda " << rondaActual << ":\n";
    std::cout << "\n ANALISIS ESTRATEGICO (Teorema de Bayes):\n";
    std::cout << "┌─────────────────────────────────────────────────────────┐\n";
    std::cout << "│ P(Exito de " << nombreJ2 << " | Modo FACIL):    " << std::fixed << std::setprecision(1) << (probabilidadExitoJ2 * 100) << "% │\n";
    std::cout << "│ P(Exito de " << nombreJ2 << " | Modo DIFICIL):  " << (probabilidadExitoJ2Dificil * 100) << "% │\n";
    std::cout << "└─────────────────────────────────────────────────────────┘\n\n";
    
    // Mostrar matriz de ganancias (Equilibrio de Nash)
    std::cout << " MATRIZ DE GANANCIAS (Equilibrio de Nash):\n";
    std::cout << "┌─────────────┬─────────────┬─────────────┐\n";
    std::cout << "│             │" << nombreJ2 << " FALLA │ " << nombreJ2 << " ACIERTA │\n";
    std::cout << "├─────────────┼─────────────┼─────────────┤\n";
    
    auto gananciasFacilFallo = matrizGanancias->obtenerGanancias(FACIL, false);
    auto gananciasFacilExito = matrizGanancias->obtenerGanancias(FACIL, true);
    auto gananciasDificilFallo = matrizGanancias->obtenerGanancias(DIFICIL, false);
    auto gananciasDificilExito = matrizGanancias->obtenerGanancias(DIFICIL, true);
    
    std::cout << "│ " << nombreJ1 << " FACIL   │     ";
    ColoresConsola::establecerColor(VERDE);
    std::cout << "(" << gananciasFacilFallo.first << "," << gananciasFacilFallo.second << ")";
    ColoresConsola::restablecerColor();
    std::cout << "     │     ";
    ColoresConsola::establecerColor(AZUL);
    std::cout << "(" << gananciasFacilExito.first << "," << gananciasFacilExito.second << ")";
    ColoresConsola::restablecerColor();
    std::cout << "     │\n";
    
    std::cout << "│ " << nombreJ1 << " DIFICIL │     ";
    ColoresConsola::establecerColor(AMARILLO);
    std::cout << "(" << gananciasDificilFallo.first << "," << gananciasDificilFallo.second << ")";
    ColoresConsola::restablecerColor();
    std::cout << "     │     ";
    ColoresConsola::establecerColor(ROJO);
    std::cout << "(" << gananciasDificilExito.first << "," << gananciasDificilExito.second << ")";
    ColoresConsola::restablecerColor();
    std::cout << "     │\n";
    
    std::cout << "└─────────────┴─────────────┴─────────────┘\n";
    std::cout << "Formato: (Puntos " << nombreJ1 << ", Puntos " << nombreJ2 << ")\n\n";

    // Guardar estadísticas de esta ronda
    EstadisticasRonda estadisticaRonda;
    estadisticaRonda.numeroRonda = rondaActual;
    estadisticaRonda.dificultadElegida = secuenciaActual->getDificultad();
    estadisticaRonda.secuenciaCompleta = secuenciaOriginal;
    estadisticaRonda.exitoMemorizacion = exito;
    estadisticaRonda.puntosGanadosJ1 = ganancias.first;
    estadisticaRonda.puntosGanadosJ2 = ganancias.second;
    estadisticaRonda.probabilidadEsperadaExito = calculadorBayes->calcularProbabilidadCondicional(
        jugadores[1], secuenciaActual->getDificultad());
    estadisticaRonda.estrategiaRecomendada = (estadisticaRonda.probabilidadEsperadaExito > 0.6) ? "FACIL" : "DIFICIL";
    
    estadisticasRondas.push_back(estadisticaRonda);
    
    std::cout << "\n APLICANDO MATRIZ DE GANANCIAS:\n";
    std::cout << "┌─────────────────────────────────────────┐\n";
    std::cout << "│ Dificultad: " << (secuenciaActual->getDificultad() == FACIL ? "FACIL" : "DIFICIL") << "                      │\n";
    std::cout << "│ Resultado: " << (exito ? "EXITO" : "FALLO") << "                       │\n";
    std::cout << "│                                         │\n";
    std::cout << "│ " << nombreJ1 << " recibe: +" << ganancias.first << " puntos           │\n";
    std::cout << "│ " << nombreJ2 << " recibe: +" << ganancias.second << " puntos           │\n";
    std::cout << "└─────────────────────────────────────────┘\n\n";
    
    // Aplicar puntos
    jugadores[0].sumarPuntos(ganancias.first);
    jugadores[1].sumarPuntos(ganancias.second);

    std::cout << " PUNTUACION ACTUAL:\n";
    std::cout << "┌──────────────────┬────────────┐\n";
    std::cout << "│ Jugador          │ Puntos     │\n";
    std::cout << "├──────────────────┼────────────┤\n";
    std::cout << "│ " << std::setw(16) << std::left << nombreJ1 << " │ " 
              << std::setw(10) << jugadores[0].getPuntaje() << " │\n";
    std::cout << "│ " << std::setw(16) << std::left << nombreJ2 << " │ " 
              << std::setw(10) << jugadores[1].getPuntaje() << " │\n";
    std::cout << "└──────────────────┴────────────┘\n\n";
    
    // Mostrar líder actual
    if (jugadores[0].getPuntaje() > jugadores[1].getPuntaje()) {
        ColoresConsola::establecerColor(VERDE);
        std::cout << nombreJ1 << " lidera la partida!!\n";
    } else if (jugadores[1].getPuntaje() > jugadores[0].getPuntaje()) {
        ColoresConsola::establecerColor(AZUL);
        std::cout << nombreJ2 << " lidera la partida!!\n";
    } else {
        ColoresConsola::establecerColor(AMARILLO);
        std::cout << " Empate en la puntuacion\n";
    }
    ColoresConsola::restablecerColor();
    
    std::cout << "\n Progreso: Ronda " << rondaActual << " de 5 completada\n";
    
    if (rondaActual < 5) {
        std::cout << "\n ⚠️  SOLO se aceptara la tecla ENTER - otras teclas seran ignoradas";
        std::cout << "\nPresiona ENTER para continuar a la siguiente ronda...";
        std::cin.ignore();
        esperarSoloEnter();
    }
}

void Juego404::guardarPartida() {
    tablaScore->agregarScore(nombreJ1, nombreJ2, 
                            jugadores[0].getPuntaje(), 
                            jugadores[1].getPuntaje(), 
                            rondaActual);
    
    std::cout << "\n Partida guardada en la tabla de puntuaciones.\n";
}


void Juego404::jugarRonda() {
    while (estadoActual != FIN || rondaActual < 3) {
        actualizarEstado();
        std::cout << "\n";
    }
}

void Juego404::analizarEstrategias() const {
    ColoresConsola::limpiarPantalla();
    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    ColoresConsola::establecerColor(VERDE);
    std::cout << "║                 ANALISIS ESTRATEGICO AVANZADO              ║\n";
    ColoresConsola::restablecerColor();
    std::cout << "╚════════════════════════════════════════════════════════════╝\n\n";
    
    equilibrioNash->analizarEstrategias();
    
    std::cout << "\n ANALISIS DEL GRAFO DE ESTADOS:\n";
    grafoJuego->mostrarGrafo();
    
    std::cout << "\n SIMULACIONES NFA (Automata Finito No-Determinista):\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    simularNFA("memoria");
    simularNFA("adivinar");
    simularNFA("iniciar");
    simularNFA("finalizar");
    
    std::cout << "\nPresiona ENTER para finalizar...";
    std::cin.ignore();
    esperarSoloEnterGlobal();
}

void Juego404::simularNFA(std::string entrada) const {
    std::cout << "\n Transiciones NFA para '" << entrada << "':\n";
    if (transicionesNFA.find(entrada) != transicionesNFA.end()) {
        for (const std::string& estado : transicionesNFA.at(entrada)) {
            std::cout << "   -> " << estado << "\n";
        }
    } else {
        std::cout << "   -> No hay transiciones definidas\n";
    }
}

void Juego404::mostrarMatrizGanancias() {
    matrizGanancias->mostrarMatriz();
}

void Juego404::mostrarEstadisticasJuego() const {
    ColoresConsola::limpiarPantalla();
    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    ColoresConsola::establecerColor(AMARILLO);
    std::cout << "║                   ESTADISTICAS FINALES                    ║\n";
    ColoresConsola::restablecerColor();
    std::cout << "╠════════════════════════════════════════════════════════════╣\n";
    
    // Mostrar estadísticas de jugadores
    for (const Jugador& jugador : jugadores) {
        jugador.mostrarEstadisticas();
    }
    
    // Determinar y mostrar ganador
    std::cout << "\n╔════════════════════════════════════════════════════════════╗\n";
    if (jugadores[0].getPuntaje() > jugadores[1].getPuntaje()) {
        ColoresConsola::establecerColor(VERDE);
        std::cout << "║                   " << nombreJ1 << " ES EL GANADOR!               ║\n";
        std::cout << "║              Estrategia superior demostrada                ║\n";
    } else if (jugadores[1].getPuntaje() > jugadores[0].getPuntaje()) {
        ColoresConsola::establecerColor(AZUL);
        std::cout << "║                   " << nombreJ2 << " ES EL GANADOR!               ║\n";
        std::cout << "║               Memoria excepcional demostrada               ║\n";
    } else {
        ColoresConsola::establecerColor(AMARILLO);
        std::cout << "║                       EMPATE!                             ║\n";
        std::cout << "║              Ambos jugadores son excelentes               ║\n";
    }
    ColoresConsola::restablecerColor();
    std::cout << "╚════════════════════════════════════════════════════════════╝\n\n";
    
    // Mostrar análisis bayesiano
    calculadorBayes->mostrarProbabilidades();
    
    std::cout << "\nPresiona ENTER para continuar...";
    std::cin.ignore();
    esperarSoloEnterGlobal();
}

void Juego404::mostrarEstadisticasDetalladas() const {
    ColoresConsola::limpiarPantalla();
    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    ColoresConsola::establecerColor(VERDE);
    std::cout << "║              ESTADISTICAS DETALLADAS POR RONDA             ║\n";
    ColoresConsola::restablecerColor();
    std::cout << "╚════════════════════════════════════════════════════════════╝\n\n";
    
    for (const auto& stats : estadisticasRondas) {
        std::cout << "╔═══════════════ RONDA " << stats.numeroRonda << " ═══════════════╗\n";
        
        // Mostrar secuencia de la ronda
        std::cout << "║ Secuencia: ";
        for (size_t i = 0; i < stats.secuenciaCompleta.size(); i++) {
            std::string colorStr;
            switch (stats.secuenciaCompleta[i]) {
                case ROJO: colorStr = "ROJO"; break;
                case VERDE: colorStr = "VERDE"; break;
                case AZUL: colorStr = "AZUL"; break;
                case AMARILLO: colorStr = "AMARILLO"; break;
            }
            std::cout << colorStr;
            if (i < stats.secuenciaCompleta.size() - 1) std::cout << " → ";
        }
        std::cout << " ║\n";
        
        // Mostrar información estratégica
        std::cout << "║ Dificultad elegida: " << (stats.dificultadElegida == FACIL ? "FACIL" : "DIFICIL") << "                      ║\n";
        std::cout << "║ Resultado: " << (stats.exitoMemorizacion ? "EXITO ✓" : "FALLO ✗") << "                            ║\n";
        std::cout << "║ Probabilidad esperada: " << std::fixed << std::setprecision(1) 
                  << (stats.probabilidadEsperadaExito * 100) << "%                    ║\n";
        std::cout << "║ Estrategia recomendada: " << stats.estrategiaRecomendada << "                     ║\n";
        
        // Mostrar matriz de ganancias aplicada
        std::cout << "╠═══════════════════════════════════════════════════════════╣\n";
        std::cout << "║ MATRIZ DE GANANCIAS APLICADA:                             ║\n";
        std::cout << "║ " << nombreJ1 << " ganó: +" << stats.puntosGanadosJ1 << " puntos                          ║\n";
        std::cout << "║ " << nombreJ2 << " ganó: +" << stats.puntosGanadosJ2 << " puntos                          ║\n";
        
        // Análisis estratégico
        std::cout << "╠═══════════════════════════════════════════════════════════╣\n";
        std::cout << "║ ANALISIS ESTRATEGICO:                                     ║\n";
        
        if (stats.dificultadElegida == FACIL && stats.probabilidadEsperadaExito > 0.7) {
            std::cout << "║  Decision estrategica acertada                           ║\n";
        } else if (stats.dificultadElegida == DIFICIL && stats.probabilidadEsperadaExito < 0.5) {
            std::cout << "║  Decision arriesgada pero justificada                   ║\n";
        } else {
            std::cout << "║  Decision suboptima según probabilidades                ║\n";
        }
        
        if (stats.exitoMemorizacion) {
            ColoresConsola::establecerColor(VERDE);
            std::cout << "║ Memoria exitosa - Recompensa obtenida                  ║\n";
            ColoresConsola::restablecerColor();
        } else {
            ColoresConsola::establecerColor(ROJO);
            std::cout << "║ Fallo de memoria - Oportunidad perdida                ║\n";
            ColoresConsola::restablecerColor();
        }
        
        std::cout << "╚═══════════════════════════════════════════════════════════╝\n\n";
    }
    
    // Resumen final con matriz de ganancias
    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    ColoresConsola::establecerColor(AMARILLO);
    std::cout << "║                    RESUMEN MATRIZ DE NASH                 ║\n";
    ColoresConsola::restablecerColor();
    std::cout << "╠════════════════════════════════════════════════════════════╣\n";
    
    // Mostrar la matriz final con los resultados
    std::cout << "║                    " << nombreJ2 << " FALLA │ " << nombreJ2 << " ACIERTA ║\n";
    std::cout << "╠════════════════════════════════════════════════════════════╣\n";
    
    auto gananciasFacilFallo = matrizGanancias->obtenerGanancias(FACIL, false);
    auto gananciasFacilExito = matrizGanancias->obtenerGanancias(FACIL, true);
    auto gananciasDificilFallo = matrizGanancias->obtenerGanancias(DIFICIL, false);
    auto gananciasDificilExito = matrizGanancias->obtenerGanancias(DIFICIL, true);
    
    std::cout << "║ " << nombreJ1 << " FACIL:     ";
    ColoresConsola::establecerColor(VERDE);
    std::cout << "(" << gananciasFacilFallo.first << "," << gananciasFacilFallo.second << ")";
    ColoresConsola::restablecerColor();
    std::cout << "   │   ";
    ColoresConsola::establecerColor(AZUL);
    std::cout << "(" << gananciasFacilExito.first << "," << gananciasFacilExito.second << ")";
    ColoresConsola::restablecerColor();
    std::cout << "   ║\n";
    
    std::cout << "║ " << nombreJ1 << " DIFICIL:   ";
    ColoresConsola::establecerColor(AMARILLO);
    std::cout << "(" << gananciasDificilFallo.first << "," << gananciasDificilFallo.second << ")";
    ColoresConsola::restablecerColor();
    std::cout << "   │   ";
    ColoresConsola::establecerColor(ROJO);
    std::cout << "(" << gananciasDificilExito.first << "," << gananciasDificilExito.second << ")";
    ColoresConsola::restablecerColor();
    std::cout << "   ║\n";
    
    std::cout << "║ Formato: (Puntos " << nombreJ1 << ", Puntos " << nombreJ2 << ")                 ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n\n";
    
    std::cout << "Presiona ENTER para continuar con las estadisticas finales...";
    std::cin.ignore();
    esperarSoloEnterGlobal();
}

int main() {
    MenuPrincipal menu;
    menu.run();
    return 0;
}
