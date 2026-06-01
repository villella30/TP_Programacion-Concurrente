#include "productor.h"
#include "worker.h"
#include "message_queue.h"
#include "vram_pool.h"
#include "registro.h"
#include "constantes.h"
#include "contador.h"
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

// Log global compartido por todos los hilos
RegistroLog log_global;


// Configuracion del escenario

const int NUM_PRODUCTORES      = 3;  
const int NUM_WORKERS          = 3;   
const int JOBS_POR_PRODUCTOR   = 10;  

int main() {
    // Abrir el log antes de lanzar cualquier hilo
    abrirLog(log_global, "sistema.log");

    int total_jobs = NUM_PRODUCTORES * JOBS_POR_PRODUCTOR;

    std::cout << "  Sistema de Renderizado en la Nube" << std::endl;
    std::cout << "Productores : " << NUM_PRODUCTORES << std::endl;
    std::cout << "Workers     : " << NUM_WORKERS     << std::endl;
    std::cout << "Jobs totales: " << total_jobs      << std::endl;
    std::cout << "Buffer cap. : " << CAPACIDAD_COLA  << std::endl;
    std::cout << "VRAM slots  : " << SLOTS_VRAM      << std::endl;

    auto t_inicio = std::chrono::steady_clock::now();

    // Recursos compartidos
    MessageQueue queue(CAPACIDAD_COLA);
    VramPool     pool;


    // Lanzar hilos productores
    std::vector<std::thread> hilos_prod;
    for (int i = 1; i <= NUM_PRODUCTORES; i++) {
        hilos_prod.emplace_back(funcion_productor, i, JOBS_POR_PRODUCTOR, std::ref(queue));
    }

    
    // Lanzar hilos workers (consumidores)
    std::vector<std::thread> hilos_worker;
    for (int i = 1; i <= NUM_WORKERS; i++) {
        hilos_worker.emplace_back(funcion_worker, i, std::ref(queue), std::ref(pool), total_jobs);
    }

    
    // Esperar a que todos los productores terminen
    for (auto& t : hilos_prod) t.join();

    std::cout << "[Main] Todos los productores finalizaron." << std::endl;

    
    // Poison pills: un job con id=-1 por cada worker para que terminen
    for (int i = 0; i < NUM_WORKERS; i++) {
        Job pill;
        pill.id        = -1;
        pill.prioridad = FREE;
        pill.evento    = CREADO;
        pill.ts_creado = std::chrono::system_clock::now();
        pill.ts_encolado = pill.ts_creado;
        queue.push(pill);
    }

    
    // Esperar a que todos los workers terminen
    for (auto& t : hilos_worker) t.join();

    auto t_fin = std::chrono::steady_clock::now();
    double ms  = std::chrono::duration<double, std::milli>(t_fin - t_inicio).count();

    
    // final
    
    
    std::cout << "Todos los workers finalizaron."    << std::endl;
    std::cout << "Jobs procesados exitosamente: ";
    {
        std::lock_guard<std::mutex> lk(mtx_finalizados);
        std::cout << jobs_finalizados;
    }
    std::cout << " / " << total_jobs << std::endl;
    std::cout << "Tiempo total: " << ms << " ms" << std::endl;
    std::cout << "Log guardado en: sistema.log" << std::endl;
    

    cerrarLog(log_global);
    return 0;
}