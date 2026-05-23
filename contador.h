#ifndef CONTADOR_H
#define CONTADOR_H

#include <atomic>
#include <mutex>

// ID unico global — cada productor incrementa este contador al crear un job.
// atomic garantiza que dos productores no obtienen el mismo ID aunque corran en paralelo.
inline std::atomic<int> proximo_id_job{1};

// Cantidad de jobs finalizados con exito.
// Se usa mutex (no atomic) para demostrar exclusion mutua explicita, como pide la cursada.
inline int           jobs_finalizados = 0;
inline std::mutex    mtx_finalizados;

#endif
