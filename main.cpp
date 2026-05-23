// Prueba aislada Fase 2A — Productor + MessageQueue
// Verifica que 2 productores y 1 consumidor dummy procesan exactamente 20 jobs
// sin deadlock ni condicion de carrera.
// El consumidor dummy solo extrae jobs e imprime; no usa el VRAM Pool.

#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

#include "constantes.h"
#include "job.h"
#include "semaforo.h"
#include "registro.h"
#include "contador.h"
#include "message_queue.h"
#include "productor.h"

// Recurso compartido global — accedido via extern desde productor.cpp
RegistroLog log_global;

// Consumidor dummy: extrae jobs de la cola hasta completar el total esperado
void consumidor_dummy(MessageQueue& queue, int total_jobs) {
    int consumidos = 0;
    while (consumidos < total_jobs) {
        // 1. Extraer el job de mayor prioridad efectiva
        Job job = queue.pop();

        // 2. Imprimir para verificar el orden de prioridad
        std::cout << "[Consumidor] Job " << job.id
                  << " | " << (job.prioridad == PREMIUM ? "PREMIUM" : "FREE")
                  << std::endl;

        consumidos++;
    }
}

int main() {
    abrirLog(log_global, "sistema.log");

    const int N_PRODUCTORES      = 2;
    const int JOBS_POR_PRODUCTOR = 10;
    const int TOTAL_JOBS         = N_PRODUCTORES * JOBS_POR_PRODUCTOR;

    MessageQueue queue(CAPACIDAD_COLA);

    // Lanzar productores (patron de main_aging.cpp)
    std::vector<std::thread> productores;
    for (int i = 0; i < N_PRODUCTORES; i++) {
        productores.emplace_back(funcion_productor, i + 1, JOBS_POR_PRODUCTOR, std::ref(queue));
    }

    // Lanzar consumidor dummy
    std::thread consumidor(consumidor_dummy, std::ref(queue), TOTAL_JOBS);

    for (auto& t : productores) t.join();
    consumidor.join();

    std::cout << "\nPrueba Fase 2A completada." << std::endl;
    std::cout << "Jobs procesados: " << TOTAL_JOBS << std::endl;
    std::cout << "Revisar sistema.log: debe tener " << TOTAL_JOBS * 2
              << " lineas (CREADO + EN_COLA por job)." << std::endl;

    cerrarLog(log_global);
    return 0;
}
