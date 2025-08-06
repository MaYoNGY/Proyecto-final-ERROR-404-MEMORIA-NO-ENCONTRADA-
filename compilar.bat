@echo off
echo Compilando ERROR 404: Memoria no encontrada...
echo.

REM Limpiar archivos objeto anteriores
if exist *.o del *.o

REM Compilar el proyecto
g++ -std=c++17 -Wall -Wextra -O2 juego.cpp jugador.cpp secuencia.cpp matriz_ganancias.cpp calculador_bayes.cpp equilibrio_nash.cpp grafo_juego.cpp -o juego404

if %ERRORLEVEL% == 0 (
    echo.
    echo ✓ Compilacion exitosa!
    echo ✓ Ejecutable generado: juego404.exe
    echo.
    echo Para jugar, ejecuta: .\juego404
    echo.
) else (
    echo.
    echo ✗ Error en la compilacion
    echo Revisa los mensajes de error arriba
    echo.
)

pause
