#include "registro.h"
#include <chrono>
#include <iomanip>

std::string prioridadToString(Prioridad p) {
    return (p == PREMIUM) ? "PREMIUM" : "FREE";
}

std::string eventoToString(EventoJob evento) {
    switch (evento) {
        case CREADO:        return "CREADO";
        case EN_COLA:       return "EN_COLA";
        case ASIGNADO_VRAM: return "ASIGNADO_VRAM";
        case FINALIZADO:    return "FINALIZADO";
        default:            return "DESCONOCIDO";
    }
}

void abrirLog(RegistroLog& l, const std::string& nombreArchivo) {
    l.archivoLog.open(nombreArchivo, std::ios::out | std::ios::trunc);
}

void registrarEvento(RegistroLog& l, const Job& job) {
    auto now      = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);

    l.mtx_log.lock();
    if (l.archivoLog.is_open()) {
        l.archivoLog << "[" << std::put_time(std::localtime(&t), "%H:%M:%S") << "]"
                     << " - " << job.id
                     << " - " << prioridadToString(job.prioridad)
                     << " - " << eventoToString(job.evento)
                     << "\n";
        l.archivoLog.flush();
    }
    l.mtx_log.unlock();
}

void cerrarLog(RegistroLog& l) {
    if (l.archivoLog.is_open()) {
        l.archivoLog.close();
    }
}
