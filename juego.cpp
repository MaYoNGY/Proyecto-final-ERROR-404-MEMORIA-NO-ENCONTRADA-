#include "juego.h"

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
}

void Juego404::mostrarPantallaInicio() {
    std::cout << "===============================\n";
    std::cout << "  ERROR 404: Memoria no encontrada\n";
    std::cout << "===============================\n";
    std::cout << "Juego de memoria, estrategia y azar\n\n";
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
    
    std::cout << "Juego inicializado. Comenzando ronda " << (rondaActual + 1) << "\n";
    secuenciaActual->mostrarSecuencia();
}

void Juego404::actualizarEstado() {
    switch (estadoActual) {
        case INICIO:
            std::cout << "\n=== ESTADO: INICIO ===\n";
            estadoActual = DECISION_DIFICULTAD;
            break;
            
        case DECISION_DIFICULTAD:
            std::cout << "\n=== ESTADO: DECISIÓN DE DIFICULTAD ===\n";
            procesarTurnoJugador1();
            estadoActual = REPETICION_SECUENCIA;
            
            std::cout << "\nPresiona ENTER para continuar al turno del Jugador 2...";
            std::cin.ignore();
            std::cin.get();
            break;
            
        case REPETICION_SECUENCIA:
            std::cout << "\n=== ESTADO: REPETICIÓN DE SECUENCIA ===\n";
            procesarTurnoJugador2();
            estadoActual = VERIFICACION;
            
            std::cout << "\nPresiona ENTER para ver la verificación...";
            std::cin.ignore();
            std::cin.get();
            break;
            
        case VERIFICACION:
            std::cout << "\n=== ESTADO: VERIFICACIÓN ===\n";
            verificarResultado();
            estadoActual = FIN;
            break;
            
        case FIN:
            std::cout << "\n=== ESTADO: FIN DE RONDA ===\n";
            std::cout << "Ronda " << (rondaActual + 1) << " finalizada.\n";
            rondaActual++;
            
            if (rondaActual < 3) { // Jugar máximo 3 rondas
                std::cout << "\n¿Continuar con la ronda " << (rondaActual + 1) << "? (s/n): ";
                char continuar;
                std::cin >> continuar;
                
                if (continuar == 's' || continuar == 'S') {
                    estadoActual = DECISION_DIFICULTAD;
                    std::cout << "\n" << std::string(50, '=') << "\n";
                    std::cout << "INICIANDO RONDA " << (rondaActual + 1) << "\n";
                    std::cout << std::string(50, '=') << "\n";
                } else {
                    std::cout << "\nJuego terminado por decisión del jugador.\n";
                    mostrarEstadisticasJuego();
                    rondaActual = 3; // Forzar fin del juego
                }
            } else {
                std::cout << "\n🏁 ¡JUEGO TERMINADO! 🏁\n";
                mostrarEstadisticasJuego();
            }
            break;
            
        default:
            break;
    }
}

void Juego404::procesarTurnoJugador1() {
    std::cout << "\n--- Turno del Jugador 1 ---\n";
    std::cout << "Secuencia actual: ";
    secuenciaActual->mostrarSecuencia();
    
    // Mostrar análisis estratégico
    double probabilidadExitoJ2 = calculadorBayes->calcularProbabilidadCondicional(jugadores[1], FACIL);
    std::cout << "\nAnálisis estratégico:\n";
    std::cout << "- Probabilidad de éxito de J2 en modo FÁCIL: " << (probabilidadExitoJ2 * 100) << "%\n";
    
    double probabilidadExitoJ2Dificil = calculadorBayes->calcularProbabilidadCondicional(jugadores[1], DIFICIL);
    std::cout << "- Probabilidad de éxito de J2 en modo DIFÍCIL: " << (probabilidadExitoJ2Dificil * 100) << "%\n";
    
    // Mostrar ganancias esperadas
    auto gananciasFacilFallo = matrizGanancias->obtenerGanancias(FACIL, false);
    auto gananciasFacilExito = matrizGanancias->obtenerGanancias(FACIL, true);
    auto gananciasDificilFallo = matrizGanancias->obtenerGanancias(DIFICIL, false);
    auto gananciasDificilExito = matrizGanancias->obtenerGanancias(DIFICIL, true);
    
    std::cout << "\nGanancias esperadas:\n";
    std::cout << "FÁCIL  - Si J2 falla: J1=+" << gananciasFacilFallo.first << ", J2=+" << gananciasFacilFallo.second << "\n";
    std::cout << "FÁCIL  - Si J2 acierta: J1=+" << gananciasFacilExito.first << ", J2=+" << gananciasFacilExito.second << "\n";
    std::cout << "DIFÍCIL - Si J2 falla: J1=+" << gananciasDificilFallo.first << ", J2=+" << gananciasDificilFallo.second << "\n";
    std::cout << "DIFÍCIL - Si J2 acierta: J1=+" << gananciasDificilExito.first << ", J2=+" << gananciasDificilExito.second << "\n";
    
    // Permitir al jugador elegir
    int eleccion;
    do {
        std::cout << "\nJugador 1, elige la dificultad para el próximo color:\n";
        std::cout << "1. FÁCIL (patrón predecible)\n";
        std::cout << "2. DIFÍCIL (patrón aleatorio)\n";
        std::cout << "Ingresa tu elección (1 o 2): ";
        std::cin >> eleccion;
        
        if (eleccion != 1 && eleccion != 2) {
            std::cout << "Opción inválida. Por favor, elige 1 o 2.\n";
        }
    } while (eleccion != 1 && eleccion != 2);
    
    TipoDificultad dificultadElegida = (eleccion == 1) ? FACIL : DIFICIL;
    secuenciaActual->establecerDificultad(dificultadElegida);
    
    std::cout << "\nJ1 eligió: " << (dificultadElegida == FACIL ? "FÁCIL" : "DIFÍCIL") << "\n";
    
    // Agregar nuevo color según la dificultad
    Color nuevoColor = secuenciaActual->generarSiguienteColor(dificultadElegida);
    secuenciaActual->agregarColor(nuevoColor);
    
    std::cout << "Nuevo color agregado: " << secuenciaActual->colorToString(nuevoColor) << "\n";
    std::cout << "\nSecuencia completa actualizada:\n";
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
    std::cout << "\n--- Turno del Jugador 2 ---\n";
    std::cout << "¡Es tu turno de repetir la secuencia completa!\n\n";
    
    // Mostrar información de la dificultad
    std::cout << "Dificultad elegida por J1: " << (secuenciaActual->getDificultad() == FACIL ? "FÁCIL" : "DIFÍCIL") << "\n";
    
    // Mostrar la secuencia por un tiempo limitado
    std::cout << "\n¡MEMORIZA LA SECUENCIA!\n";
    std::cout << "La secuencia se mostrará por 3 segundos...\n";
    std::cout << "\nPresiona ENTER para ver la secuencia: ";
    std::cin.ignore();
    std::cin.get();
    
    // Mostrar secuencia
    std::cout << "\n*** SECUENCIA A MEMORIZAR ***\n";
    secuenciaActual->mostrarSecuencia();
    
    // Esperar 3 segundos
    std::cout << "\nMemorizando";
    for (int i = 0; i < 3; i++) {
        std::cout << ".";
        std::cout.flush();
        #ifdef _WIN32
            Sleep(1000);
        #else
            sleep(1);
        #endif
    }
    
    // Limpiar pantalla (simulado)
    std::cout << "\n\n\n\n\n\n\n\n\n\n";
    std::cout << "*** TIEMPO TERMINADO ***\n";
    std::cout << "Ahora ingresa la secuencia que memorizaste:\n\n";
    
    // Obtener la secuencia del jugador
    std::vector<Color> secuenciaOriginal = secuenciaActual->getSecuencia();
    std::vector<Color> secuenciaJugador;
    
    std::cout << "Colores disponibles:\n";
    std::cout << "1. ROJO\n";
    std::cout << "2. VERDE\n";
    std::cout << "3. AZUL\n";
    std::cout << "4. AMARILLO\n\n";
    
    std::cout << "Ingresa " << secuenciaOriginal.size() << " colores (usa los números 1-4):\n";
    
    for (size_t i = 0; i < secuenciaOriginal.size(); i++) {
        int colorElegido;
        do {
            std::cout << "Color " << (i + 1) << ": ";
            std::cin >> colorElegido;
            
            if (colorElegido < 1 || colorElegido > 4) {
                std::cout << "Número inválido. Usa 1-4.\n";
            }
        } while (colorElegido < 1 || colorElegido > 4);
        
        secuenciaJugador.push_back(static_cast<Color>(colorElegido - 1));
    }
    
    // Verificar si la secuencia es correcta
    bool exito = secuenciaActual->validarSecuencia(secuenciaJugador);
    
    std::cout << "\n*** RESULTADO ***\n";
    std::cout << "Secuencia original: ";
    for (size_t i = 0; i < secuenciaOriginal.size(); i++) {
        std::cout << secuenciaActual->colorToString(secuenciaOriginal[i]);
        if (i < secuenciaOriginal.size() - 1) std::cout << " -> ";
    }
    std::cout << "\n";
    
    std::cout << "Tu secuencia:       ";
    for (size_t i = 0; i < secuenciaJugador.size(); i++) {
        std::cout << secuenciaActual->colorToString(secuenciaJugador[i]);
        if (i < secuenciaJugador.size() - 1) std::cout << " -> ";
    }
    std::cout << "\n";
    
    if (exito) {
        std::cout << "\n🎉 ¡CORRECTO! Repetiste la secuencia perfectamente!\n";
    } else {
        std::cout << "\n❌ INCORRECTO. La secuencia no coincide.\n";
    }
    
    // Registrar el resultado en el historial
    jugadores[1].agregarAcierto(exito);
    jugadores[1].agregarDificultad(secuenciaActual->getDificultad());
    
    // Calcular y asignar ganancias
    auto ganancias = matrizGanancias->obtenerGanancias(secuenciaActual->getDificultad(), exito);
    jugadores[0].sumarPuntos(ganancias.first);
    jugadores[1].sumarPuntos(ganancias.second);
    
    std::cout << "\nGanancias de esta ronda:\n";
    std::cout << "Jugador 1: +" << ganancias.first << " puntos\n";
    std::cout << "Jugador 2: +" << ganancias.second << " puntos\n";
    
    std::cout << "\nPuntajes totales:\n";
    std::cout << "Jugador 1: " << jugadores[0].getPuntaje() << " puntos\n";
    std::cout << "Jugador 2: " << jugadores[1].getPuntaje() << " puntos\n";
    
    // Crear nodo de resultado
    Nodo nodoResultado;
    nodoResultado.secuencia = secuenciaActual->getSecuencia();
    nodoResultado.dificultad = secuenciaActual->getDificultad();
    nodoResultado.puntajeJugador1 = jugadores[0].getPuntaje();
    nodoResultado.puntajeJugador2 = jugadores[1].getPuntaje();
    
    int nodoResultadoId = grafoJuego->agregarNodo(nodoResultado);
    
    std::string accion = exito ? "secuencia_correcta" : "secuencia_incorrecta";
    double utilidadResultado = exito ? 1.0 : 0.0;
    grafoJuego->agregarArista(nodoActual, nodoResultadoId, utilidadResultado, accion);
    
    nodoActual = nodoResultadoId;
}

void Juego404::verificarResultado() {
    std::cout << "\n--- Verificación de Resultado ---\n";
    std::cout << "Puntajes actuales:\n";
    std::cout << "Jugador 1: " << jugadores[0].getPuntaje() << " puntos\n";
    std::cout << "Jugador 2: " << jugadores[1].getPuntaje() << " puntos\n";
    
    // Actualizar probabilidades bayesianas
    calculadorBayes->actualizarProbabilidades(jugadores);
    
    std::cout << "Probabilidades actualizadas.\n";
}

void Juego404::mostrarMatrizGanancias() {
    matrizGanancias->mostrarMatriz();
}

void Juego404::simularNFA(std::string entrada) {
    std::cout << "\nTransiciones NFA para '" << entrada << "':\n";
    if (transicionesNFA.find(entrada) != transicionesNFA.end()) {
        for (const std::string& estado : transicionesNFA[entrada]) {
            std::cout << "→ " << estado << "\n";
        }
    } else {
        std::cout << "→ No hay transiciones definidas\n";
    }
}

void Juego404::jugarRonda() {
    while (estadoActual != FIN || rondaActual < 3) {
        actualizarEstado();
        std::cout << "\n";
    }
}

void Juego404::mostrarEstadisticasJuego() const {
    std::cout << "\n=== ESTADÍSTICAS FINALES ===\n";
    
    for (const Jugador& jugador : jugadores) {
        jugador.mostrarEstadisticas();
    }
    
    // Mostrar análisis bayesiano
    calculadorBayes->mostrarProbabilidades();
    
    // Mostrar análisis del grafo
    grafoJuego->mostrarGrafo();
    
    // Determinar ganador
    if (jugadores[0].getPuntaje() > jugadores[1].getPuntaje()) {
        std::cout << "\n🏆 ¡Jugador 1 es el ganador!\n";
    } else if (jugadores[1].getPuntaje() > jugadores[0].getPuntaje()) {
        std::cout << "\n🏆 ¡Jugador 2 es el ganador!\n";
    } else {
        std::cout << "\n🤝 ¡Empate!\n";
    }
}

void Juego404::analizarEstrategias() const {
    equilibrioNash->analizarEstrategias();
}

int main() {
    Juego404 juego;
    juego.mostrarPantallaInicio();
    
    std::cout << "¡Bienvenidos al juego!\n\n";
    std::cout << "INSTRUCCIONES:\n";
    std::cout << "- Este es un juego para 2 jugadores\n";
    std::cout << "- Jugador 1: Elige la dificultad de cada ronda\n";
    std::cout << "- Jugador 2: Debe memorizar y repetir la secuencia de colores\n";
    std::cout << "- Se jugarán hasta 3 rondas\n";
    std::cout << "- El jugador con más puntos al final gana\n\n";
    
    std::cout << "¿Están listos para comenzar? (s/n): ";
    char comenzar;
    std::cin >> comenzar;
    
    if (comenzar != 's' && comenzar != 'S') {
        std::cout << "¡Hasta luego!\n";
        return 0;
    }
    
    juego.inicializarJuego();
    
    // Ejecutar el juego
    juego.jugarRonda();
    
    // Preguntar si quieren ver análisis adicional
    std::cout << "\n¿Quieren ver el análisis estratégico detallado? (s/n): ";
    char verAnalisis;
    std::cin >> verAnalisis;
    
    if (verAnalisis == 's' || verAnalisis == 'S') {
        std::cout << "\n=== ANÁLISIS ESTRATÉGICO AVANZADO ===\n";
        juego.analizarEstrategias();
        
        std::cout << "\n=== SIMULACIONES NFA ===\n";
        juego.simularNFA("memoria");
        juego.simularNFA("adivinar");
        juego.simularNFA("iniciar");
        juego.simularNFA("finalizar");
    }
    
    std::cout << "\n¡Gracias por jugar ERROR 404: Memoria no encontrada!\n";
    std::cout << "Presiona ENTER para salir...";
    std::cin.ignore();
    std::cin.get();

    return 0;
}
