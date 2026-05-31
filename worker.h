#include "message_queue.h"
#include "vram_pool.h"

void funcion_worker(int id_worker, MessageQueue& queue, VramPool& pool, int total_jobs);