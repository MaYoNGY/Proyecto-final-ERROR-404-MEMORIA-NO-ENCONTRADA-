#include "juego.h"

GrafoJuego::GrafoJuego() : siguienteId(0) {
    nodos.clear();
    adyacencias.clear();
}

int GrafoJuego::agregarNodo(const Nodo& nodo) {
    Nodo nuevoNodo = nodo;
    nuevoNodo.id = siguienteId++;
    
    nodos.push_back(nuevoNodo);
    adyacencias.push_back(std::vector<Arista>());
    
    return nuevoNodo.id;
}

void GrafoJuego::agregarArista(int origen, int destino, double utilidad, const std::string& accion) {
    if (origen >= 0 && origen < static_cast<int>(adyacencias.size())) {
        Arista nuevaArista = {utilidad, destino, accion};
        adyacencias[origen].push_back(nuevaArista);
    }
}

std::vector<Arista> GrafoJuego::obtenerTransiciones(int nodoId) const {
    if (nodoId >= 0 && nodoId < static_cast<int>(adyacencias.size())) {
        return adyacencias[nodoId];
    }
    return std::vector<Arista>();
}

Nodo GrafoJuego::obtenerNodo(int id) const {
    for (const Nodo& nodo : nodos) {
        if (nodo.id == id) {
            return nodo;
        }
    }
    
    // Retornar nodo vacío si no se encuentra
    Nodo nodoVacio = {{}, FACIL, 0, 0, -1};
    return nodoVacio;
}

void GrafoJuego::mostrarGrafo() const {
    std::cout << "\n=== GRAFO DEL JUEGO ===\n";
    std::cout << "Nodos: " << nodos.size() << "\n";
    
    for (size_t i = 0; i < nodos.size(); i++) {
        const Nodo& nodo = nodos[i];
        std::cout << "\nNodo " << nodo.id << ":\n";
        std::cout << "  Secuencia: ";
        
        if (nodo.secuencia.empty()) {
            std::cout << "[vacía]";
        } else {
            for (size_t j = 0; j < nodo.secuencia.size(); j++) {
                std::string colorStr;
                switch (nodo.secuencia[j]) {
                    case ROJO: colorStr = "ROJO"; break;
                    case VERDE: colorStr = "VERDE"; break;
                    case AZUL: colorStr = "AZUL"; break;
                    case AMARILLO: colorStr = "AMARILLO"; break;
                }
                std::cout << colorStr;
                if (j < nodo.secuencia.size() - 1) std::cout << "->";
            }
        }
        
        std::cout << "\n  Dificultad: " << (nodo.dificultad == FACIL ? "FÁCIL" : "DIFÍCIL");
        std::cout << "\n  Puntajes: J1=" << nodo.puntajeJugador1 << ", J2=" << nodo.puntajeJugador2;
        
        std::cout << "\n  Transiciones:";
        if (adyacencias[i].empty()) {
            std::cout << " [ninguna]";
        } else {
            for (const Arista& arista : adyacencias[i]) {
                std::cout << "\n    -> Nodo " << arista.destino 
                          << " (Utilidad: " << arista.utilidad 
                          << ", Acción: " << arista.accion << ")";
            }
        }
        std::cout << "\n";
    }
    std::cout << "======================\n";
}

std::vector<int> GrafoJuego::encontrarCaminoOptimo(int origen, int destino) const {
    if (origen == destino) {
        return {origen};
    }
    
    // Implementación simplificada de búsqueda del camino con mayor utilidad
    std::vector<int> camino;
    std::vector<bool> visitado(nodos.size(), false);
    std::vector<double> maxUtilidad(nodos.size(), -1.0);
    std::vector<int> predecesor(nodos.size(), -1);
    
    maxUtilidad[origen] = 1.0;
    
    // Algoritmo similar a Dijkstra pero maximizando utilidad
    for (size_t iteraciones = 0; iteraciones < nodos.size(); iteraciones++) {
        int nodoActual = -1;
        double mejorUtilidad = -1.0;
        
        // Encontrar el nodo no visitado con mayor utilidad
        for (size_t i = 0; i < nodos.size(); i++) {
            if (!visitado[i] && maxUtilidad[i] > mejorUtilidad) {
                mejorUtilidad = maxUtilidad[i];
                nodoActual = i;
            }
        }
        
        if (nodoActual == -1) break;
        
        visitado[nodoActual] = true;
        
        if (nodoActual == destino) break;
        
        // Actualizar utilidades de nodos adyacentes
        for (const Arista& arista : adyacencias[nodoActual]) {
            if (!visitado[arista.destino]) {
                double nuevaUtilidad = maxUtilidad[nodoActual] * arista.utilidad;
                if (nuevaUtilidad > maxUtilidad[arista.destino]) {
                    maxUtilidad[arista.destino] = nuevaUtilidad;
                    predecesor[arista.destino] = nodoActual;
                }
            }
        }
    }
    
    // Reconstruir camino
    if (maxUtilidad[destino] > -1.0) {
        int actual = destino;
        while (actual != -1) {
            camino.push_back(actual);
            actual = predecesor[actual];
        }
        // Invertir el camino manualmente
        for (size_t i = 0; i < camino.size() / 2; i++) {
            int temp = camino[i];
            camino[i] = camino[camino.size() - 1 - i];
            camino[camino.size() - 1 - i] = temp;
        }
    }
    
    return camino;
}
