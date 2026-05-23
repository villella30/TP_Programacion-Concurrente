#include "message_queue.h"
#include "semaforo.h"
#include "constantes.h"
#include <mutex>
#include <vector>
#include <chrono>

// Retorna true si 'a' tiene mayor prioridad efectiva que 'b'.
// Aplica aging: un job FREE que lleva mas de UMBRAL_STARVATION_MS esperando
// se trata como PREMIUM en la comparacion, sin modificar su campo prioridad.
bool tieneEfectivamentePrioridad(const Job& a, const Job& b) {
    auto ahora = std::chrono::system_clock::now();

    int prioA = a.prioridad;
    if (a.prioridad == FREE) {
        auto espera = std::chrono::duration_cast<std::chrono::milliseconds>(
            ahora - a.ts_encolado).count();
        if (espera >= UMBRAL_STARVATION_MS) prioA = PREMIUM;
    }

    int prioB = b.prioridad;
    if (b.prioridad == FREE) {
        auto espera = std::chrono::duration_cast<std::chrono::milliseconds>(
            ahora - b.ts_encolado).count();
        if (espera >= UMBRAL_STARVATION_MS) prioB = PREMIUM;
    }

    return prioA > prioB;
}

MessageQueue::MessageQueue(int capacidad) {
    init(hay_espacio, capacidad);
    init(hay_datos, 0);
}

void MessageQueue::push(const Job& job) {
    // 1. Esperar que haya espacio en la cola
    wait(hay_espacio);

    // 2. Exclusion mutua para insertar
    mtx_cola.lock();
    cola.push_back(job);
    mtx_cola.unlock();

    // 3. Avisar que hay un nuevo dato disponible
    signal(hay_datos);
}

Job MessageQueue::pop() {
    // 1. Esperar que haya datos en la cola
    wait(hay_datos);

    // 2. Exclusion mutua para extraer el job de mayor prioridad efectiva
    mtx_cola.lock();
    auto mejor = cola.begin();
    for (auto it = cola.begin(); it != cola.end(); ++it) {
        if (tieneEfectivamentePrioridad(*it, *mejor)) {
            mejor = it;
        }
    }
    Job job = *mejor;
    cola.erase(mejor);
    mtx_cola.unlock();

    // 3. Avisar que se libero un espacio
    signal(hay_espacio);

    return job;
}
