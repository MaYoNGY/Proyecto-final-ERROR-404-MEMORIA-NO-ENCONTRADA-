#include "juego.h"

void ColoresConsola::configurarConsola() {
#ifdef _WIN32
    // Habilitar colores ANSI en Windows 10+
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
    
    // Configurar codificación UTF-8
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
}

void ColoresConsola::establecerColor(Color color) {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    switch (color) {
        case ROJO:
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
            break;
        case VERDE:
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            break;
        case AZUL:
            SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            break;
        case AMARILLO:
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            break;
    }
#else
    switch (color) {
        case ROJO:
            std::cout << "\033[91m";
            break;
        case VERDE:
            std::cout << "\033[92m";
            break;
        case AZUL:
            std::cout << "\033[94m";
            break;
        case AMARILLO:
            std::cout << "\033[93m";
            break;
    }
#endif
}

void ColoresConsola::restablecerColor() {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#else
    std::cout << "\033[0m";
#endif
}

void ColoresConsola::limpiarPantalla() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void ColoresConsola::mostrarColorConTexto(Color color, const std::string& texto) {
    establecerColor(color);
    std::cout << "█████ " << texto;
    restablecerColor();
}

void ColoresConsola::pausar() {
#ifdef _WIN32
    std::cout << "\nPresiona ENTER para continuar...";
    esperarSoloEnterGlobal();
#else

    std::cout << "\nPresiona ENTER para continuar...";
    std::cin.ignore();
    esperarSoloEnterGlobal();
#endif
}
