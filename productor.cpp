#include "productor.h"
#include "job.h"
#include "registro.h"
#include "contador.h"
#include "constantes.h"
#include <thread>
#include <chrono>
#include <random>

extern RegistroLog log_global;

void funcion_productor(int id_productor, int n_jobs, MessageQueue& queue) {
    // Semilla unica por hilo para que cada productor genere prioridades independientes
    std::mt19937 gen(std::hash<std::thread::id>{}(std::this_thread::get_id()));
    std::uniform_int_distribution<int> dist(0, 1);

    for (int i = 0; i < n_jobs; i++) {
        // 1. Crear el job con ID unico atomico y prioridad aleatoria 50/50
        Job job;
        job.id        = proximo_id_job++;
        job.prioridad = (dist(gen) == 1) ? PREMIUM : FREE;
        job.ts_creado = std::chrono::system_clock::now();
        job.evento    = CREADO;
        registrarEvento(log_global, job);

        // 2. Simular retardo de ingreso al sistema
        std::this_thread::sleep_for(std::chrono::milliseconds(RETARDO_INGRESO_COLA_MS));

        // 3. Encolar el job y registrar el evento
        job.ts_encolado = std::chrono::system_clock::now();
        job.evento      = EN_COLA;
        queue.push(job);
        registrarEvento(log_global, job);
    }
}
