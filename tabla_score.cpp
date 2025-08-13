#include "juego.h"
#include <iomanip>
#include <fstream>
#include <iostream>
#include <algorithm>

TablaScore::TablaScore(const std::string& archivo) : archivoScore(archivo) {
    cargarScores();
}

void TablaScore::cargarScores() {
    std::ifstream archivo(archivoScore);
    if (!archivo.is_open()) {
        return; // Si no existe, crear tabla vacía
    }
    
    registros.clear();
    std::string linea;
    
    while (std::getline(archivo, linea)) {
        if (linea.empty()) continue;
        
        RegistroScore record;
        size_t pos = 0;
        std::vector<std::string> datos;
        
        // Parsear línea separada por comas
        while ((pos = linea.find(',')) != std::string::npos) {
            datos.push_back(linea.substr(0, pos));
            linea.erase(0, pos + 1);
        }
        datos.push_back(linea); // Último elemento
        
        if (datos.size() >= 6) {
            record.nombreJ1 = datos[0];
            record.nombreJ2 = datos[1];
            record.puntajeJ1 = std::stoi(datos[2]);
            record.puntajeJ2 = std::stoi(datos[3]);
            record.fecha = datos[4];
            record.rondasJugadas = std::stoi(datos[5]);
            registros.push_back(record);
        }
    }
    
    archivo.close();
}

void TablaScore::guardarScores() {
    std::ofstream archivo(archivoScore);
    if (!archivo.is_open()) {
        std::cout << "Error: No se pudo guardar los scores.\n";
        return;
    }
    
    for (const auto& record : registros) {
        archivo << record.nombreJ1 << "," << record.nombreJ2 << ","
                << record.puntajeJ1 << "," << record.puntajeJ2 << ","
                << record.fecha << "," << record.rondasJugadas << "\n";
    }
    
    archivo.close();
}

void TablaScore::agregarScore(const std::string& nombreJ1, const std::string& nombreJ2, 
                              int puntajeJ1, int puntajeJ2, int rondas) {
    RegistroScore nuevoRecord;
    nuevoRecord.nombreJ1 = nombreJ1;
    nuevoRecord.nombreJ2 = nombreJ2;
    nuevoRecord.puntajeJ1 = puntajeJ1;
    nuevoRecord.puntajeJ2 = puntajeJ2;
    nuevoRecord.fecha = obtenerFechaActual();
    nuevoRecord.rondasJugadas = rondas;
    
    registros.push_back(nuevoRecord);
    guardarScores();
}

void TablaScore::mostrarTablaScore() {
    ColoresConsola::limpiarPantalla();
    std::cout << "╔══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                    TABLA DE PUNTUACIONES                     ║\n";
    std::cout << "╠══════════════════════════════════════════════════════════════╣\n";
    
    if (registros.empty()) {
        std::cout << "║                   No hay registros aun                     ║\n";
        std::cout << "╚════════════════════════════════════════════════════════════╝\n";
        return;
    }
    
    std::cout << "║ Jugador 1      │ Jugador 2      │ Puntos │ Rondas │ Fecha      ║\n";
    std::cout << "╠────────────────┼────────────────┼────────┼────────┼────────────╣\n";
    
    for (const auto& record : registros) {
        std::cout << "║ " << std::setw(14) << std::left << record.nombreJ1.substr(0, 14)
                  << " │ " << std::setw(14) << std::left << record.nombreJ2.substr(0, 14)
                  << " │ " << std::setw(3) << record.puntajeJ1 << "-" << std::setw(2) << record.puntajeJ2
                  << " │ " << std::setw(6) << record.rondasJugadas
                  << " │ " << std::setw(8) << record.fecha.substr(0, 8) << "   ║\n";
    }
    
    std::cout << "╚════════════════════════════════════════════════════════════╝\n";
}

void TablaScore::mostrarTop10() {
    // Crear vector de jugadores con sus puntuaciones totales
    std::vector<std::pair<std::string, int>> rankings;
    
    for (const auto& record : registros) {
        // Buscar si el jugador ya existe
        bool encontrado = false;
        for (auto& ranking : rankings) {
            if (ranking.first == record.nombreJ1) {
                ranking.second += record.puntajeJ1;
                encontrado = true;
                break;
            }
        }
        if (!encontrado) {
            rankings.push_back({record.nombreJ1, record.puntajeJ1});
        }
        
        // Lo mismo para el jugador 2
        encontrado = false;
        for (auto& ranking : rankings) {
            if (ranking.first == record.nombreJ2) {
                ranking.second += record.puntajeJ2;
                encontrado = true;
                break;
            }
        }
        if (!encontrado) {
            rankings.push_back({record.nombreJ2, record.puntajeJ2});
        }
    }
    
    // Ordenar por puntuación descendente
    std::sort(rankings.begin(), rankings.end(), 
              [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
                  return a.second > b.second;
              });
    
    std::cout << "╔══════════════════════════════════════╗\n";
    std::cout << "║            TOP 10 JUGADORES          ║\n";
    std::cout << "╠══════════════════════════════════════╣\n";
    std::cout << "║ Pos │ Jugador           │ Puntos     ║\n";
    std::cout << "╠─────┼───────────────────┼────────────╣\n";
    
    for (int i = 0; i < std::min(10, (int)rankings.size()); i++) {
        std::cout << "║ " << std::setw(3) << (i + 1) << " │ "
                  << std::setw(17) << std::left << rankings[i].first.substr(0, 17) << " │ "
                  << std::setw(8) << rankings[i].second << " ║\n";
    }
    
    std::cout << "╚══════════════════════════════════════╝\n";
}

std::string TablaScore::obtenerFechaActual() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    
    std::string fecha = std::to_string(ltm->tm_mday) + "/" +
                       std::to_string(1 + ltm->tm_mon) + "/" +
                       std::to_string(1900 + ltm->tm_year);
    return fecha;
}

