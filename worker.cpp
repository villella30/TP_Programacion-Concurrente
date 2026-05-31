#include "worker.h"

void funcion_worker(int id_worker, MessageQueue& queue, VramPool& pool, int total_jobs) {
    // El worker corre hasta que el sistema le indica que no hay mas jobs.
    // Estrategia de terminacion: el main usa una poison pill (senal extra en
    // el semaforo hay_datos) o una variable atomica booleana de "seguir".
    // Coordinar con Integrante 4 cual de las dos se usa.

    while (true) {
    
        Job job = queue.pop(); // 1. Toma un job de la queue, si no hay jobs se queda esperando

        // 2. (Si es poison pill / job especial de fin, break)
        if (job.id == -1){ //el job especial sería -1 porqué no existe.
            break;
        }
        pool.assign(job); // 3. Asigna al VRAM Pool
        pool.release(job); // 3. Libera del VRAM Pool (incluye esperar 600ms y logear FINALIZADO)
    }
}