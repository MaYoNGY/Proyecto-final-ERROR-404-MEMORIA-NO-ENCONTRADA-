#ifndef JUEGO_H
#define JUEGO_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>
#include <fstream>
#include <iomanip>
#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <unistd.h>
#endif

// Estados posibles
enum EstadoJuego {
    INICIO, DECISION_DIFICULTAD, REPETICION_SECUENCIA, VERIFICACION, FIN
};

// Tipos de dificultad
enum TipoDificultad {
    FACIL, DIFICIL
};

// Colores disponibles
enum Color {
    ROJO, VERDE, AZUL, AMARILLO
};

// Nodo del grafo
struct Nodo {
    std::vector<Color> secuencia;
    TipoDificultad dificultad;
    int puntajeJugador1;
    int puntajeJugador2;
    int id;
};

struct Arista {
    double utilidad; // Probabilidad de éxito ponderada
    int destino;     // Índice del nodo destino
    std::string accion; // Descripción de la acción
};

// Forward declarations
class Jugador;
class Secuencia;
class MatrizGanancias;
class CalculadorBayes;
class EquilibrioNash;
class GrafoJuego;
class TablaScore;
class MenuPrincipal;

// Funciones globales para control de entrada
void esperarSoloEnterGlobal(); // Función global para esperar solo ENTER

// Estructura para almacenar records de puntuaciones
struct RegistroScore {
    std::string nombreJ1;
    std::string nombreJ2;
    int puntajeJ1;
    int puntajeJ2;
    std::string fecha;
    int rondasJugadas;
};

// Estructura para almacenar estadísticas de cada ronda
struct EstadisticasRonda {
    int numeroRonda;
    TipoDificultad dificultadElegida;
    std::vector<Color> secuenciaCompleta;
    bool exitoMemorizacion;
    int puntosGanadosJ1;
    int puntosGanadosJ2;
    double probabilidadEsperadaExito;
    std::string estrategiaRecomendada;
};

// Clase para manejar colores en consola Windows
class ColoresConsola {
public:
    static void configurarConsola();
    static void establecerColor(Color color);
    static void restablecerColor();
    static void limpiarPantalla();
    static void mostrarColorConTexto(Color color, const std::string& texto);
    static void pausar();
};

// Clase Jugador
class Jugador {
private:
    int id;
    int puntaje;
    std::vector<bool> historialAciertos;
    std::vector<TipoDificultad> historialDificultades;

public:
    Jugador(int idJugador);
    void agregarAcierto(bool acierto);
    void agregarDificultad(TipoDificultad dif);
    void sumarPuntos(int puntos);
    int getPuntaje() const;
    int getId() const;
    double getTasaExito() const;
    std::vector<bool> getHistorialAciertos() const;
    void mostrarEstadisticas() const;
};

// Clase Secuencia
class Secuencia {
private:
    std::vector<Color> colores;
    TipoDificultad dificultadActual;

public:
    Secuencia();
    void agregarColor(Color color);
    void establecerDificultad(TipoDificultad dif);
    std::vector<Color> getSecuencia() const;
    TipoDificultad getDificultad() const;
    Color generarSiguienteColor(TipoDificultad dif) const;
    std::string colorToString(Color color) const;
    void mostrarSecuencia() const;
    bool validarSecuencia(const std::vector<Color>& intento) const;
};

// Clase MatrizGanancias
class MatrizGanancias {
private:
    std::vector<std::vector<std::pair<int, int>>> matriz;

public:
    MatrizGanancias();
    void calcularGanancias(TipoDificultad difJ1, bool exito);
    std::pair<int, int> obtenerGanancias(TipoDificultad difJ1, bool exito) const;
    void mostrarMatriz() const;
    std::pair<TipoDificultad, bool> encontrarEquilibrio() const;
};

// Clase CalculadorBayes
class CalculadorBayes {
private:
    double probabilidadExitoFacil;
    double probabilidadExitoDificil;

public:
    CalculadorBayes();
    double calcularProbabilidadCondicional(const Jugador& jugador, TipoDificultad dificultad) const;
    void actualizarProbabilidades(const std::vector<Jugador>& jugadores);
    double calcularProbabilidadPosterior(double probabilidadPrevia, bool evidencia, TipoDificultad dificultad) const;
    void mostrarProbabilidades() const;
};

// Clase EquilibrioNash
class EquilibrioNash {
private:
    MatrizGanancias* matrizGanancias;

public:
    EquilibrioNash(MatrizGanancias* matriz);
    std::pair<TipoDificultad, bool> encontrarEquilibrio() const;
    bool esEquilibrio(TipoDificultad estrategiaJ1, bool estrategiaJ2) const;
    void analizarEstrategias() const;
};

// Clase GrafoJuego
class GrafoJuego {
private:
    std::vector<Nodo> nodos;
    std::vector<std::vector<Arista>> adyacencias;
    int siguienteId;

public:
    GrafoJuego();
    int agregarNodo(const Nodo& nodo);
    void agregarArista(int origen, int destino, double utilidad, const std::string& accion);
    std::vector<Arista> obtenerTransiciones(int nodoId) const;
    Nodo obtenerNodo(int id) const;
    void mostrarGrafo() const;
    std::vector<int> encontrarCaminoOptimo(int origen, int destino) const;
};

// Clase TablaScore para manejar puntuaciones
class TablaScore {
private:
    std::vector<RegistroScore> registros;
    std::string archivoScore;

public:
    TablaScore(const std::string& archivo = "scores.txt");
    void cargarScores();
    void guardarScores();
    void agregarScore(const std::string& nombreJ1, const std::string& nombreJ2, 
                      int puntajeJ1, int puntajeJ2, int rondas);
    void mostrarTablaScore();
    void mostrarTop10();
    std::string obtenerFechaActual();
};

// Clase MenuPrincipal
class MenuPrincipal {
private:
    TablaScore* tablaScore;
    
public:
    MenuPrincipal();
    ~MenuPrincipal();
    void mostrarMenuPrincipal();
    void mostrarMenuEstadisticas();
    void mostrarTeoremasBayes();
    void mostrarGrafosDirigidos();
    void mostrarEquilibrioNash();
    int procesarOpcionMenu();
    int procesarOpcionMenuEstadisticas(); // Agregar esta función
    void ejecutarJuego();
    void manejarEstadisticas();
    void salir();
    void run();
};

// Clase principal del juego
class Juego404 {
private:
    GrafoJuego* grafoJuego;
    std::vector<Jugador> jugadores;
    Secuencia* secuenciaActual;
    MatrizGanancias* matrizGanancias;
    CalculadorBayes* calculadorBayes;
    EquilibrioNash* equilibrioNash;
    TablaScore* tablaScore;
    EstadoJuego estadoActual;
    std::unordered_map<std::string, std::vector<std::string>> transicionesNFA;
    int rondaActual;
    int nodoActual;
    std::string nombreJ1;
    std::string nombreJ2;
    std::vector<EstadisticasRonda> estadisticasRondas; // Almacenar estadísticas de cada ronda

public:
    Juego404();
    ~Juego404();
    void configurarJugadores();
    void mostrarPantallaInicio();
    void inicializarJuego();
    void actualizarEstado();
    void mostrarMatrizGanancias();
    void simularNFA(std::string entrada) const;
    void jugarRonda();
    void procesarTurnoJugador1();
    void procesarTurnoJugador2();
    void verificarResultado();
    int leerColorValido(int min, int max); // Función para leer solo teclas válidas
    void esperarSoloEnter(); // Función para esperar solo ENTER bloqueando otros caracteres
    char leerSiNo(); // Función para leer solo S/N bloqueando otros caracteres
    void mostrarEstadisticasJuego() const;
    void mostrarEstadisticasDetalladas() const; // Nueva función para estadísticas detalladas
    void analizarEstrategias() const;
    void guardarPartida();
    bool continuarJuego();
};

#endif
