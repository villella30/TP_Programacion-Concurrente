#include "worker.h"
#include "registro.h"

extern RegistroLog log_global;

void funcion_worker(int id_worker, MessageQueue& queue, VramPool& pool, int total_jobs) {
    while (true) {
        Job job = queue.pop(); // 1. Toma un job de la queue, si no hay jobs se queda esperando
        // 2. (Si es poison pill / job especial de fin, break)
        if (job.id == -1){ //el job especial sería -1 porqué no existe.
            break;
        }
        pool.assign(job);
        registrarEvento(log_global, job);   // registra ASIGNADO_VRAM
        pool.release(job);
        registrarEvento(log_global, job);   // registra FINALIZADO
    }
}
