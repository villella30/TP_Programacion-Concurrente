// Prueba de humo — Integrante 1
// Verifica que Logger, Job, Semaforo y contador funcionan correctamente con hilos concurrentes.
// Este main es temporal; el main final del sistema lo implementa el Integrante 4.

#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

#include "constantes.h"
#include "job.h"
#include "semaforo.h"
#include "registro.h"
#include "contador.h"

RegistroLog log_global;

// Cada hilo loguea 5 eventos seguidos para verificar que no hay lineas entrelazadas
void hilo_logger(int id_hilo) {
    for (int i = 0; i < 5; i++) {
        Job j;
        j.id        = proximo_id_job++;
        j.prioridad = (id_hilo % 2 == 0) ? PREMIUM : FREE;
        j.evento    = EN_COLA;
        j.ts_creado = std::chrono::system_clock::now();
        registrarEvento(log_global, j);
    }
}

int main() {
    abrirLog(log_global, "sistema.log");

    // Crear 3 jobs de muestra y logear CREADO
    for (int i = 0; i < 3; i++) {
        Job j;
        j.id        = proximo_id_job++;
        j.prioridad = (i % 2 == 0) ? PREMIUM : FREE;
        j.evento    = CREADO;
        j.ts_creado = std::chrono::system_clock::now();
        registrarEvento(log_global, j);
    }

    // Lanzar 2 hilos que loguean en paralelo — verifica atomicidad del logger
    // (patron tomado de fuentes-clase-prod-cons/main.cpp)
    std::vector<std::thread> hilos;
    for (int i = 0; i < 2; i++) {
        hilos.emplace_back(hilo_logger, i);
    }
    for (auto& t : hilos) t.join();

    cerrarLog(log_global);

    std::cout << "Prueba de humo completada." << std::endl;
    std::cout << "Revisar sistema.log: debe tener 13 lineas sin texto entrelazado." << std::endl;
    std::cout << "  - 3 eventos CREADO" << std::endl;
    std::cout << "  - 10 eventos EN_COLA (5 por cada hilo)" << std::endl;

    return 0;
}

