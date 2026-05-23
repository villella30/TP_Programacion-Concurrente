#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include "job.h"
#include "semaforo.h"
#include <mutex>
#include <vector>

struct MessageQueue {
    std::vector<Job> cola;
    Semaforo hay_espacio;
    Semaforo hay_datos;
    std::mutex mtx_cola;

    MessageQueue(int capacidad);
    void push(const Job& job);
    Job  pop();
};

#endif
