#ifndef PRODUCTOR_H
#define PRODUCTOR_H

#include "message_queue.h"

void funcion_productor(int id_productor, int n_jobs, MessageQueue& queue);

#endif
