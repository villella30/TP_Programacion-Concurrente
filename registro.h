#ifndef REGISTRO_H
#define REGISTRO_H

#include <string>
#include <mutex> 
#include <fstream>
#include "job.h"

struct RegistroLog{
    std::mutex mtx_log;
    std::ofstream archivoLog;
};

void abrirLog(RegistroLog& l, const std::string& nombreArchivo);
void registrarEvento(RegistroLog& l, const Job& job);
void cerrarLog(RegistroLog& l);

#endif