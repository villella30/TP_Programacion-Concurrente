#ifndef JOB_H
#define JOB_H

#include <chrono>

enum Prioridad {
    FREE    = 0,
    PREMIUM = 1
};

enum EventoJob {
    CREADO,
    EN_COLA,
    ASIGNADO_VRAM,
    FINALIZADO
};

struct Job {
    int       id;
    Prioridad prioridad;
    EventoJob evento;
    std::chrono::system_clock::time_point ts_creado;
    std::chrono::system_clock::time_point ts_encolado;
    std::chrono::system_clock::time_point ts_asignado;
    std::chrono::system_clock::time_point ts_finalizado;
};

#endif