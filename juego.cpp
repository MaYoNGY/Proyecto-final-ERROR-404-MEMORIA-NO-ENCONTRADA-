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
            
            std::cout << "\nPresiona cualquier tecla para continuar al turno del Jugador 2...";
            getchMultiplataforma();
            limpiarPantalla();
            break;
            
        case REPETICION_SECUENCIA:
            // Limpiar pantalla antes del turno del Jugador 2
            limpiarPantalla();
            
            std::cout << "\n=== ESTADO: REPETICIÓN DE SECUENCIA ===\n";
            procesarTurnoJugador2();
            estadoActual = VERIFICACION;
            
            std::cout << "\nPresiona cualquier tecla para ver la verificación...";
            getchMultiplataforma();
            limpiarPantalla();
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
                std::cout << "\n¿Continuar con la ronda " << (rondaActual + 1) << "? (S/N): ";
                bool continuar = validarEntradaSiNo("");
                
                if (continuar) {
                    estadoActual = DECISION_DIFICULTAD;
                    limpiarPantalla();
                    std::cout << "\n" << std::string(50, '=') << "\n";
                    std::cout << "INICIANDO RONDA " << (rondaActual + 1) << "\n";
                    std::cout << std::string(50, '=') << "\n";
                } else {
                    limpiarPantalla();
                    std::cout << "\nJuego terminado por decisión del jugador.\n";
                    mostrarMenuFinal();
                    rondaActual = 3; // Forzar fin del juego
                }
            } else {
                limpiarPantalla();
                std::cout << "\n🏁 ¡JUEGO TERMINADO! 🏁\n";
                mostrarMenuFinal();
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
    
    // Permitir al jugador elegir sin mostrar análisis estratégico
    int eleccion;
    do {
        std::cout << "\nJugador 1, elige la dificultad para el próximo color:\n";
        std::cout << "1. FÁCIL (patrón predecible)\n";
        std::cout << "2. DIFÍCIL (patrón aleatorio)\n";
        
        eleccion = validarEntradaNumerica("Ingresa tu elección (1 o 2): ", 1, 2);
        
        if (eleccion == -1) {
            // Si se canceló, usar FACIL por defecto
            eleccion = 1;
            std::cout << "Usando dificultad FÁCIL por defecto.\n";
        }
        
    } while (eleccion != 1 && eleccion != 2);
    
    TipoDificultad dificultadElegida = (eleccion == 1) ? FACIL : DIFICIL;
    secuenciaActual->establecerDificultad(dificultadElegida);
    
    // Almacenar la decisión para el resumen final
    historialDificultades.push_back(dificultadElegida);
    
    std::cout << "\nJ1 eligió: " << (dificultadElegida == FACIL ? "FÁCIL" : "DIFÍCIL") << "\n";
    
    // Limpiar la pantalla después de la elección
    limpiarPantalla();
    
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
    std::cout << "\nPresiona cualquier tecla para ver la secuencia: ";
    
    // Limpiar buffer de entrada y esperar cualquier tecla
    getchMultiplataforma();
    
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
    
    // Limpiar pantalla
    limpiarPantalla();
    
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
            colorElegido = validarEntradaNumerica("Color " + std::to_string(i + 1) + " (1-4): ", 1, 4);
            
            if (colorElegido == -1) {
                // Si se canceló, usar ROJO por defecto
                colorElegido = 1;
                std::cout << "Usando ROJO por defecto.\n";
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
    
    // Almacenar el resultado para el resumen final
    historialResultados.push_back(exito);
    
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

void Juego404::limpiarPantalla() const {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

char Juego404::getchMultiplataforma() const {
    #ifdef _WIN32
        char ch = _getch();
        // En Windows, filtrar teclas especiales que pueden causar problemas
        if (ch == 0 || ch == -32) { // Teclas especiales como flechas, F1-F12, etc.
            _getch(); // Leer el segundo byte y descartarlo
            return '\0'; // Retornar caracter nulo para indicar tecla especial
        }
        
        // Filtrar caracteres de control adicionales (excepto ESC y Enter)
        if ((ch < 32 && ch != 27) || ch == 127) { // Caracteres de control
            return '\0'; // Ignorar caracteres de control
        }
        
        return ch;
    #else
        struct termios oldt, newt;
        char ch;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        
        // En Linux, filtrar secuencias de escape y caracteres de control
        if (ch == 27) {
            // Verificar si hay más caracteres en el buffer (secuencia de escape)
            fd_set readfds;
            struct timeval timeout;
            FD_ZERO(&readfds);
            FD_SET(STDIN_FILENO, &readfds);
            timeout.tv_sec = 0;
            timeout.tv_usec = 1000; // 1ms timeout
            
            if (select(STDIN_FILENO + 1, &readfds, NULL, NULL, &timeout) > 0) {
                // Hay más caracteres, es una secuencia de escape
                char buffer[10];
                read(STDIN_FILENO, buffer, sizeof(buffer)); // Leer y descartar
                return '\0'; // Retornar caracter nulo para indicar tecla especial
            }
            // Si no hay más caracteres, es ESC puro
            return ch;
        }
        
        // Filtrar otros caracteres de control (excepto Enter y ESC)
        if ((ch < 32 && ch != 27) || ch == 127) {
            return '\0'; // Ignorar caracteres de control
        }
        
        return ch;
    #endif
}

int Juego404::validarEntradaNumerica(const std::string& mensaje, int minimo, int maximo) const {
    char tecla;
    int numero = -1;
    
    std::cout << mensaje;
    std::cout.flush();
    
    while (true) {
        tecla = getchMultiplataforma();
        
        // Ignorar completamente teclas especiales (retornadas como '\0')
        if (tecla == '\0') {
            continue; // Simplemente ignorar y continuar, sin sonido ni mensaje
        }
        
        // Solo procesar dígitos válidos
        if (tecla >= '0' && tecla <= '9') {
            numero = tecla - '0';
            
            // Verificar si está en el rango permitido
            if (numero >= minimo && numero <= maximo) {
                std::cout << tecla << std::endl;
                return numero;
            }
            // Si está fuera del rango, ignorar completamente (sin sonido)
        } else if (tecla == 27) { // Tecla ESC
            std::cout << "\nOperación cancelada.\n";
            return -1;
        }
        // Para cualquier otra tecla, no hacer absolutamente nada
        // No emitir sonido, no mostrar mensaje, simplemente ignorar
    }
}

bool Juego404::validarEntradaSiNo(const std::string& mensaje) const {
    char tecla;
    
    std::cout << mensaje;
    std::cout.flush();
    
    while (true) {
        tecla = getchMultiplataforma();
        
        // Ignorar completamente teclas especiales (retornadas como '\0')
        if (tecla == '\0') {
            continue; // Simplemente ignorar y continuar, sin sonido ni mensaje
        }
        
        // Convertir a minúscula para facilitar la comparación
        char teclaLower = tolower(tecla);
        
        // Solo procesar S o N
        if (teclaLower == 's') {
            std::cout << tecla << " (Sí)" << std::endl;
            return true;
        } else if (teclaLower == 'n') {
            std::cout << tecla << " (No)" << std::endl;
            return false;
        } else if (tecla == 27) { // Tecla ESC
            std::cout << "\nOperación cancelada. Asumiendo 'No'.\n";
            return false;
        }
        // Para cualquier otra tecla, no hacer absolutamente nada
        // No emitir sonido, no mostrar mensaje, simplemente ignorar
    }
}

void Juego404::mostrarMenuFinal() {
    mostrarEstadisticasJuego();
    
    std::cout << "\n" << std::string(50, '=') << "\n";
    std::cout << "               MENÚ FINAL\n";
    std::cout << std::string(50, '=') << "\n";
    std::cout << "1. Ver resumen de análisis estratégico\n";
    std::cout << "2. Ver análisis detallado de equilibrios Nash\n";
    std::cout << "3. Ver simulaciones NFA\n";
    std::cout << "4. Salir\n";
    std::cout << std::string(50, '=') << "\n";
    
    int opcion;
    do {
        opcion = validarEntradaNumerica("\nSelecciona una opción (1-4): ", 1, 4);
        
        if (opcion == -1) {
            // Si se canceló, salir
            opcion = 4;
            std::cout << "Saliendo...\n";
        }
        
        switch (opcion) {
            case 1:
                limpiarPantalla();
                mostrarResumenEstrategico();
                break;
            case 2:
                limpiarPantalla();
                std::cout << "\n=== ANÁLISIS ESTRATÉGICO AVANZADO ===\n";
                analizarEstrategias();
                break;
            case 3:
                limpiarPantalla();
                std::cout << "\n=== SIMULACIONES NFA ===\n";
                simularNFA("memoria");
                simularNFA("adivinar");
                simularNFA("iniciar");
                simularNFA("finalizar");
                break;
            case 4:
                limpiarPantalla();
                std::cout << "\n¡Gracias por jugar ERROR 404: Memoria no encontrada!\n";
                return;
            default:
                std::cout << "Opción inválida. Selecciona 1-4.\n";
                std::cout << "Presiona cualquier tecla para continuar...";
                getchMultiplataforma();
                limpiarPantalla();
                mostrarEstadisticasJuego();
                std::cout << "\n" << std::string(50, '=') << "\n";
                std::cout << "               MENÚ FINAL\n";
                std::cout << std::string(50, '=') << "\n";
                std::cout << "1. Ver resumen de análisis estratégico\n";
                std::cout << "2. Ver análisis detallado de equilibrios Nash\n";
                std::cout << "3. Ver simulaciones NFA\n";
                std::cout << "4. Salir\n";
                std::cout << std::string(50, '=') << "\n";
                break;
        }
        
        if (opcion != 4) {
            opcion = validarEntradaNumerica("\n¿Deseas ver otra opción? (1-3 para opciones, 4 para salir): ", 1, 4);
            if (opcion == -1) opcion = 4; // Si se cancela, salir
        }
        
    } while (opcion != 4);
}

void Juego404::mostrarResumenEstrategico() const {
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "           RESUMEN DE ANÁLISIS ESTRATÉGICO\n";
    std::cout << std::string(60, '=') << "\n";
    
    // Análisis de decisiones del Jugador 1
    int decisionesFaciles = 0;
    int decisionesDificiles = 0;
    
    for (const TipoDificultad& dif : historialDificultades) {
        if (dif == FACIL) {
            decisionesFaciles++;
        } else {
            decisionesDificiles++;
        }
    }
    
    std::cout << "\n📊 ANÁLISIS DE ESTRATEGIA DEL JUGADOR 1:\n";
    std::cout << "• Total de decisiones tomadas: " << historialDificultades.size() << "\n";
    std::cout << "• Decisiones FÁCIL: " << decisionesFaciles << " (" << 
                 (historialDificultades.empty() ? 0 : (decisionesFaciles * 100 / historialDificultades.size())) << "%)\n";
    std::cout << "• Decisiones DIFÍCIL: " << decisionesDificiles << " (" <<
                 (historialDificultades.empty() ? 0 : (decisionesDificiles * 100 / historialDificultades.size())) << "%)\n";
    
    // Determinar patrón estratégico
    std::cout << "\n🎯 PATRÓN ESTRATÉGICO IDENTIFICADO:\n";
    if (decisionesFaciles > decisionesDificiles) {
        std::cout << "• Jugador 1 adoptó una estrategia CONSERVADORA\n";
        std::cout << "• Prefirió dar ventaja al oponente para obtener ganancias moderadas\n";
        std::cout << "• Riesgo: Bajo | Recompensa: Moderada\n";
    } else if (decisionesDificiles > decisionesFaciles) {
        std::cout << "• Jugador 1 adoptó una estrategia AGRESIVA\n";
        std::cout << "• Priorizó maximizar sus ganancias dificultando al oponente\n";
        std::cout << "• Riesgo: Alto | Recompensa: Alta\n";
    } else {
        std::cout << "• Jugador 1 adoptó una estrategia EQUILIBRADA\n";
        std::cout << "• Balanceó entre cooperación y competencia\n";
        std::cout << "• Riesgo: Moderado | Recompensa: Variable\n";
    }
    
    // Análisis de rendimiento del Jugador 2
    int aciertos = 0;
    int fallos = 0;
    
    for (bool resultado : historialResultados) {
        if (resultado) {
            aciertos++;
        } else {
            fallos++;
        }
    }
    
    std::cout << "\n🎮 ANÁLISIS DE RENDIMIENTO DEL JUGADOR 2:\n";
    std::cout << "• Secuencias intentadas: " << historialResultados.size() << "\n";
    std::cout << "• Aciertos: " << aciertos << "\n";
    std::cout << "• Fallos: " << fallos << "\n";
    std::cout << "• Tasa de éxito: " << (historialResultados.empty() ? 0 : (aciertos * 100 / historialResultados.size())) << "%\n";
    
    // Análisis de correlación (datos estáticos basados en decisiones)
    std::cout << "\n🔍 ANÁLISIS DE CORRELACIÓN ESTRATÉGICA:\n";
    
    // Calcular rendimiento por tipo de dificultad
    int aciertos_facil = 0, intentos_facil = 0;
    int aciertos_dificil = 0, intentos_dificil = 0;
    
    for (size_t i = 0; i < std::min(historialDificultades.size(), historialResultados.size()); i++) {
        if (historialDificultades[i] == FACIL) {
            intentos_facil++;
            if (historialResultados[i]) aciertos_facil++;
        } else {
            intentos_dificil++;
            if (historialResultados[i]) aciertos_dificil++;
        }
    }
    
    std::cout << "• Rendimiento en modo FÁCIL: " << aciertos_facil << "/" << intentos_facil;
    if (intentos_facil > 0) {
        std::cout << " (" << (aciertos_facil * 100 / intentos_facil) << "%)";
    }
    std::cout << "\n";
    
    std::cout << "• Rendimiento en modo DIFÍCIL: " << aciertos_dificil << "/" << intentos_dificil;
    if (intentos_dificil > 0) {
        std::cout << " (" << (aciertos_dificil * 100 / intentos_dificil) << "%)";
    }
    std::cout << "\n";
    
    // Probabilidades bayesianas estimadas (datos estáticos)
    std::cout << "\n📈 PROBABILIDADES BAYESIANAS ESTIMADAS:\n";
    double prob_exito_facil = (intentos_facil > 0) ? (double)aciertos_facil / intentos_facil : 0.75;
    double prob_exito_dificil = (intentos_dificil > 0) ? (double)aciertos_dificil / intentos_dificil : 0.35;
    
    std::cout << "• P(Éxito|Fácil) = " << (prob_exito_facil * 100) << "%\n";
    std::cout << "• P(Éxito|Difícil) = " << (prob_exito_dificil * 100) << "%\n";
    std::cout << "• Diferencial de dificultad: " << ((prob_exito_facil - prob_exito_dificil) * 100) << " puntos porcentuales\n";
    
    // Análisis de equilibrio Nash (datos estáticos)
    std::cout << "\n⚖️ ANÁLISIS DE EQUILIBRIO NASH:\n";
    if (decisionesFaciles > decisionesDificiles && aciertos > fallos) {
        std::cout << "• Estado: EQUILIBRIO COOPERATIVO alcanzado\n";
        std::cout << "• J1 facilitó el juego y J2 respondió positivamente\n";
        std::cout << "• Resultado: Beneficio mutuo moderado\n";
    } else if (decisionesDificiles > decisionesFaciles && fallos > aciertos) {
        std::cout << "• Estado: EQUILIBRIO COMPETITIVO alcanzado\n";
        std::cout << "• J1 maximizó dificultad, J2 tuvo bajo rendimiento\n";
        std::cout << "• Resultado: J1 obtiene ventaja significativa\n";
    } else {
        std::cout << "• Estado: EQUILIBRIO MIXTO\n";
        std::cout << "• Estrategias variables llevaron a resultados fluctuantes\n";
        std::cout << "• Resultado: Competencia balanceada\n";
    }
    
    // Recomendaciones estratégicas
    std::cout << "\n💡 RECOMENDACIONES ESTRATÉGICAS:\n";
    std::cout << "Para Jugador 1:\n";
    if (jugadores[0].getPuntaje() < jugadores[1].getPuntaje()) {
        std::cout << "• Considera aumentar la proporción de decisiones DIFÍCILES\n";
        std::cout << "• Tu estrategia actual no está maximizando tus ganancias\n";
    } else {
        std::cout << "• Tu estrategia actual está siendo efectiva\n";
        std::cout << "• Mantén el equilibrio entre cooperación y competencia\n";
    }
    
    std::cout << "\nPara Jugador 2:\n";
    if (aciertos < fallos) {
        std::cout << "• Practica técnicas de memorización a corto plazo\n";
        std::cout << "• Considera usar patrones mnemotécnicos\n";
    } else {
        std::cout << "• Excelente capacidad de memorización demostrada\n";
        std::cout << "• Mantén la concentración en secuencias largas\n";
    }
    
    std::cout << "\n" << std::string(60, '=') << "\n";
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
    
    std::cout << "¿Están listos para comenzar? (S/N): ";
    bool comenzar = juego.validarEntradaSiNo("");
    
    if (!comenzar) {
        std::cout << "¡Hasta luego!\n";
        return 0;
    }
    
    // Limpiar pantalla después de elegir iniciar
    juego.limpiarPantalla();
    
    juego.inicializarJuego();
    
    // Ejecutar el juego
    juego.jugarRonda();
    
    std::cout << "\nPresiona cualquier tecla para salir...";
    juego.getchMultiplataforma();

    return 0;
}
