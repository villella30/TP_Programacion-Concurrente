#include "vram_pool.h"


VramPool::VramPool() {

    init(slots_libres, SLOTS_VRAM);
}


void VramPool::assign(Job& job) {
    
    wait(slots_libres); //bloquea si los 5 slots estan ocupados
    mtx_asignacion.lock();
    job.ts_asignado = std::chrono::system_clock::now(); //job.ts_asignado = ahora
    job.evento = ASIGNADO_VRAM; //Logear evento ASIGNADO_VRAM
    std::this_thread::sleep_for(std::chrono::milliseconds(RETARDO_ASIGNACION_VRAM_MS)); //Duerme el hilo por el tiempo asignado
    mtx_asignacion.unlock();
}

void VramPool::release(Job& job) {
    // 1. Verifica que pasaron al menos MIN_PROCESAMIENTO_MS desde ts_asignado
    //    Si no pasaron, sleep_for el tiempo restante
    if ((std::chrono::system_clock::now() - job.ts_asignado) < std::chrono::milliseconds(TIEMPO_MINIMO_VRAM_MS)){ //Se busca el tiempo que ha pasado y se  compara con el tiempo minimo.
        std::this_thread::sleep_for(std::chrono::milliseconds(TIEMPO_MINIMO_VRAM_MS)- (std::chrono::system_clock::now() - job.ts_asignado)); //Se busca el tiempo restante al restar el minimo luego restar el tiempo actual y el tiempo en el que se asigno.
    };
    mtx_liberacion.lock();
    job.ts_finalizado = std::chrono::system_clock::now(); // 3. job.ts_finalizado = ahora
    job.evento = FINALIZADO; // 4. Logear evento FINALIZADO
    jobs_finalizados++; // 5. Incrementar jobs_finalizados (con mtx_finalizados de contador.h)
    std::this_thread::sleep_for(std::chrono::milliseconds(RETARDO_LIBERACION_SLOT_MS)); // 6. sleep_for(DELAY_LIBERACION_MS)
    mtx_liberacion.unlock();
    signal(slots_libres);
}