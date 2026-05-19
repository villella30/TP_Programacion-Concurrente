#ifndef JOB_H
#define JOB_H

enum EventoJob {
    CREADO,
    EN_COLA,
    ASIGNADO_VRAM,
    FINALIZADO,
};

struct Job
{   
    int id;
    int prioridad;
    EventoJob evento;
};

#endif