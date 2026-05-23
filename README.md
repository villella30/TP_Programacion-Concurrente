# Granja de Renderizado en la Nube

**Trabajo Práctico Integrador — Programación Concurrente | UNLA 2026**

Sistema multihilo en C++17 que modela una granja de renderizado cloud. Múltiples nodos productores generan jobs de renderizado y múltiples workers los procesan usando slots de GPU simulados (VRAM), coordinando el acceso concurrente a dos buffers compartidos sin deadlocks, livelocks, condiciones de carrera ni inanición.

---

## Arquitectura

```
[Productores] --(100ms/job)--> [MessageQueue] --(pop por prioridad)--> [VramPool, max 5 slots] --> FINALIZADO
                                 semaforos                               mutex asignacion (450ms)
                                 hay_espacio                             mutex liberacion  (250ms)
                                 hay_datos                               min 600ms en VRAM
                                 + aging anti-starvation
```

Cada evento del ciclo de vida de un job se registra en `sistema.log`:
```
[HH:MM:SS] - <id> - <PREMIUM|FREE> - <CREADO|EN_COLA|ASIGNADO_VRAM|FINALIZADO>
```

---

## Módulos

| Archivo | Responsable | Descripción |
|---|---|---|
| `constantes.h` | Integrante 1 | Todas las constantes del sistema |
| `job.h` | Integrante 1 | Struct `Job` con timestamps y enum `Prioridad` |
| `semaforo.h/cpp` | Integrante 1 | Semáforo manual de la cátedra (`init`, `wait`, `signal`) |
| `registro.h/cpp` | Integrante 1 | Logger atómico a `sistema.log` |
| `contador.h` | Integrante 1 | ID atómico global y contador de jobs finalizados |
| `message_queue.h/cpp` | Integrante 2 | Cola con semáforos, prioridad y aging anti-starvation |
| `productor.h/cpp` | Integrante 2 | Hilo productor: genera N jobs con prioridad aleatoria 50/50 |
| `vram_pool.h/cpp` | Integrante 3 | Pool de 5 slots VRAM con exclusión mutua y delays |
| `worker.h/cpp` | Integrante 3 | Hilo worker: toma jobs, procesa mínimo 600ms, finaliza |
| `main.cpp` | Integrante 4 | Lanza configuraciones A/B/C y los 4 escenarios de prueba |

---

## Constantes del sistema

| Constante | Valor | Descripción |
|---|---|---|
| `SLOTS_VRAM` | 5 | Capacidad máxima del pool de GPU |
| `CAPACIDAD_COLA` | 20 | Capacidad máxima de la MessageQueue |
| `RETARDO_INGRESO_COLA_MS` | 100 ms | Latencia de ingreso por job |
| `RETARDO_ASIGNACION_VRAM_MS` | 450 ms | Delay post-asignación de slot |
| `TIEMPO_MINIMO_VRAM_MS` | 600 ms | Tiempo mínimo de procesamiento en VRAM |
| `RETARDO_LIBERACION_SLOT_MS` | 250 ms | Delay post-liberación de slot |
| `UMBRAL_STARVATION_MS` | 2000 ms | Espera máxima antes de promover job FREE a PREMIUM |

---

## Compilación

```bash
g++ -std=c++17 -o sistema main.cpp semaforo.cpp registro.cpp message_queue.cpp productor.cpp vram_pool.cpp worker.cpp -lpthread
```

---

## Configuraciones de carga

| Config | Productores | Workers |
|---|---|---|
| A | 1 | 2 |
| B | 3 | 1 |
| C | 3 | 3 |

---

## Escenarios de prueba obligatorios

| Prueba | Descripción | Criterio de éxito |
|---|---|---|
| Carga masiva | 1500 jobs | Contador global == 1500, buffer vacío al terminar |
| Vacuidad | 0 jobs | Workers terminan sin busy waiting |
| Saturación | 8 jobs PREMIUM con 5 slots | Nunca más de 5 jobs activos en VRAM |
| Anti-starvation | 5000ms flujo Premium + jobs Free | Al menos 1 FREE finalizado dentro del umbral |

---

## Primitivas de sincronización utilizadas

- `std::thread` — hilos productores y workers
- `std::mutex` — exclusión mutua en cola, pool, logger y contador
- `std::condition_variable` — implementación del semáforo manual
- `std::atomic<int>` — ID único global de jobs
- `Semaforo` (manual de la cátedra) — control de flujo en MessageQueue y VramPool

> No se utilizan `std::atomic` para el VramPool ni librerías externas de testing, conforme al enunciado.

---

## Integrantes

| Integrante | Módulos |
|---|---|
| Valentino Villella | `constantes.h`, `job.h`, `semaforo`, `registro`, `contador.h` |
| Favio Alonso | `message_queue`, `productor` |
| [Nombre Integrante 3] | `vram_pool`, `worker` |
| [Nombre Integrante 4] | `main` |

---

**Entrega:** 06/06/2026 — **Defensa:** 08/06/2026
