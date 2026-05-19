#include "registro.h"
#include <chrono>
#include <iomanip>



void abrirLog(RegistroLog& l, const std::string& nombreArchivo){
    l.archivoLog.open(nombreArchivo, std::ios_base::app);
};

std::string estadoToString(EventoJob evento) {
    switch (evento) {
        case CREADO: return "CREADO";
        case EN_COLA: return "EN_COLA";
        case ASIGNADO_VRAM: return "ASIGNADO_VRAM";
        case FINALIZADO: return "FINALIZADO";
        default: return "ERROR";
    }
}

void registrarEvento(RegistroLog& l, const Job& job){
    l.mtx_log.lock();
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    if(l.archivoLog.is_open()) {
        l.archivoLog << "[" << std::put_time(std::localtime(&time), "%H:%M:%S") << "]"
        << "- Job ID: " << job.id << " "
        << "- Prioridad: " << (job.prioridad == 1 ? "Premiun" : "Free") << " "
        << "- Evento: " << estadoToString(job.evento) << "\n";
        l.archivoLog.flush();
    }
    l.mtx_log.unlock();
};
void cerrarLog(RegistroLog& l){
    if(l.archivoLog.is_open()) {
        l.archivoLog.close();
    }
};
