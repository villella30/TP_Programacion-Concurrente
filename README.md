# Granja de Renderizado en la Nube

**Trabajo Práctico Integrador — Programación Concurrente | UNLA 2026**

Sistema multihilo en C++17 que modela una granja de renderizado cloud. Múltiples nodos productores generan jobs de renderizado y múltiples workers los procesan usando slots de GPU simulados (VRAM), coordinando el acceso concurrente a dos buffers compartidos sin deadlocks, livelocks, condiciones de carrera ni inanición.

---

Cada evento del ciclo de vida de un job se registra en `sistema.log`:
```
[HH:MM:SS] - <id> - <PREMIUM|FREE> - <CREADO|EN_COLA|ASIGNADO_VRAM|FINALIZADO>
```

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

## Escenarios de prueba

| Prueba | Descripción | Criterio de éxito |
|---|---|---|
| Carga masiva | 1500 jobs | Contador global == 1500, buffer vacío al terminar |
| Vacuidad | 0 jobs | Workers terminan sin busy waiting |
| Saturación | 8 jobs PREMIUM con 5 slots | Nunca más de 5 jobs activos en VRAM |
| Anti-starvation | 5000ms flujo Premium + jobs Free | Al menos 1 FREE finalizado dentro del umbral |

---

## Integrantes

| Integrante | Módulos |
|---|---|
| Valentino Villella | `constantes.h`, `job.h`, `semaforo`, `registro`, `contador.h` |
| Favio Alonso | `message_queue`, `productor` |
| Lucas Castellví | `vram_pool`, `worker` |
| Valentina Herrera | `main`, `escenarios de prueba` |
