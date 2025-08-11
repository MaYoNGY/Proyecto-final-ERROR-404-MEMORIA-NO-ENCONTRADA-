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
#include <limits>
#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <unistd.h>
#include <termios.h>
#include <sys/select.h>
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

// Clase principal del juego
class Juego404 {
private:
    GrafoJuego* grafoJuego;
    std::vector<Jugador> jugadores;
    Secuencia* secuenciaActual;
    MatrizGanancias* matrizGanancias;
    CalculadorBayes* calculadorBayes;
    EquilibrioNash* equilibrioNash;
    EstadoJuego estadoActual;
    std::unordered_map<std::string, std::vector<std::string>> transicionesNFA;
    int rondaActual;
    int nodoActual;
    std::vector<TipoDificultad> historialDificultades;
    std::vector<bool> historialResultados;

public:
    Juego404();
    ~Juego404();
    void mostrarPantallaInicio();
    void inicializarJuego();
    void actualizarEstado();
    void mostrarMatrizGanancias();
    void simularNFA(std::string entrada);
    void jugarRonda();
    void procesarTurnoJugador1();
    void procesarTurnoJugador2();
    void verificarResultado();
    void mostrarEstadisticasJuego() const;
    void analizarEstrategias() const;
    void mostrarResumenEstrategico() const;
    void mostrarMenuFinal();
    void limpiarPantalla() const;
    int validarEntradaNumerica(const std::string& mensaje, int minimo, int maximo) const;
    bool validarEntradaSiNo(const std::string& mensaje) const;
    char getchMultiplataforma() const;
};

#endif
