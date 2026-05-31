#include <mutex>
#include <thread>
#include <chrono>

#include "job.h"
#include "semaforo.h"
#include "constantes.h"
#include "contador.h"
#pragma once

class VramPool {
public:
    VramPool();
    void assign(Job& job);    // bloquea si pool lleno; aplica 450ms delay
    void release(Job& job);   // bloquea si otra liberacion en curso; aplica 250ms delay
private:
    Semaforo   slots_libres; //QUien maneja slots libres(?)
    std::mutex mtx_asignacion;
    std::mutex mtx_liberacion;
};