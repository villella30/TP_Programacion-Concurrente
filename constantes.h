#ifndef CONSTANTES_H
#define CONSTANTES_H

// Tiempos (en milisegundos)
const int RETARDO_INGRESO_COLA_MS    = 100;
const int RETARDO_ASIGNACION_VRAM_MS = 450;
const int TIEMPO_MINIMO_VRAM_MS      = 600;
const int RETARDO_LIBERACION_SLOT_MS = 250;
const int UMBRAL_STARVATION_MS       = 2000;

// Capacidades
const int SLOTS_VRAM      = 5;
const int CAPACIDAD_COLA  = 20;

#endif