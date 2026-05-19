# Sistema de Renderizado en la Nube (Cloud Computing)

## Trabajo Práctico **Programación Concurrente**.

## Objetivo General
Desarrollar un sistema en C++ para la gestión de carga de una "Granja de Renderizado".
El proyecto modela un entorno donde múltiples actividades interactúan concurrentemente sobre recursos compartidos de memoria y procesamiento.

## ⚙️ Características y Requerimientos Implementados
* **Programación Concurrente:** Uso de múltiples hilos independientes para la generación y procesamiento de Jobs.
* **Patrón Productor-Consumidor:** Sincronización entre Nodos API Gateway (productores) y Worker Nodes (consumidores).
* **Exclusión Mutua y Race Conditions:** Acceso seguro a la memoria de video activa (Pool de VRAM) y gestión de contadores globales.
* **Gestión de Prioridades:** Algoritmo Anti-Starvation para garantizar un procesamiento justo entre Jobs "Premium" y "Free".
* **Trazabilidad:** Registro atómico de eventos en un archivo log del sistema.

## 👥 Integrantes del Equipo
* **Valentino Villella:** (Producción, Ingreso)
* **[Nombre Integrante 2]:** (Consumo y Gestión del Pool)
* **[Nombre Integrante 3]:** (Planificador y Sincronización)
* **[Nombre Integrante 4]:** (Registro, Escenarios de Carga y pruebas)

## 🔗 Enlace al Video
*(Pegar acá el link de YouTube con la narración de las pruebas en funcionamiento)*.